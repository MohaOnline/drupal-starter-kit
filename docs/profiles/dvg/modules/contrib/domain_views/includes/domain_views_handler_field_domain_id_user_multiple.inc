<?php
/**
 * @file
 * Interface between domain_views.module and views.module.
 *
 * @ingroup domain_views
 */

/**
 * Field handler to provide simple renderer that allows linking to a domain.
 *
 * @ingroup domain_views
 */
class domain_views_handler_field_domain_id_user_multiple extends views_handler_field_prerender_list {
  /**
   * Constructor to provide additional field to add.
   */
  function construct() {
    parent::construct();
  }

  function query() {
    $this->field_alias = 'uid';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_default_domain'] = array('default' => FALSE);
    $options['link_to_original_domain'] = array('default' => FALSE);
    $options['list_all_domains'] = array('default' => FALSE);

    $options['display_method'] = array('default' => 'domain_id');
    $options['link_option'] = array('default' => 'none');
    return $options;
  }

  /**
   * Provide link to default domain option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Added for backward compatibility with older versions
    if ($this->options['link_to_domain_id'] || $this->options['link_to_sitename']) {
      $this->options['link_option'] = 'link_to_domain';

      // Unset old options
      $form['link_to_domain_id'] = array('#type' => 'value', '#value' => FALSE);
      $form['link_to_sitename'] = array('#type' => 'value', '#value' => FALSE);
    }

    $form['display_method'] = array(
        '#title' => t('Display as'),
        '#type' => 'select',
        '#options' => array(
            'domain_id' => t('Domain ID'),
            'sitename' => t('Site Name'),
            'subdomain' => t('Subdomain'),
        ),
        '#default_value' => ($this->options['display_method']) ? $this->options['display_method'] : 'domain_id',
    );

    $form['link_option'] = array(
        '#title' => t('Link options'),
        '#type' => 'select',
        '#options' => array(
            'none' => t('No link'),
            'link_to_domain' => t('Link this field to its domain'),
        ),
        '#default_value' => ($this->options['link_option']) ? $this->options['link_option'] : 'none',
    );
  }

  function pre_render(&$values) {
    $uids = array();
    $this->items = array();

    foreach ($values as $result) {
      $uids[] = $result->uid;
    }

    if ($uids) {
      $result = db_query("SELECT de.uid, de.domain_id, d.sitename, d.subdomain FROM {domain_editor} de INNER JOIN {domain} d ON de.domain_id = d.domain_id WHERE de.uid IN (:uids) ORDER BY de.uid",
        array(':uids' => $uids));
      foreach ($result as $row) {
        $this->items[$row->uid][$row->domain_id]['sitename'] = check_plain($row->sitename);
        $this->items[$row->uid][$row->domain_id]['domain_id'] = $row->domain_id;
        $this->items[$row->uid][$row->domain_id]['subdomain'] = $row->subdomain;
      }
    }
  }

  function render_item($count, $item) {
    switch ($this->options['display_method']) {
      case 'sitename':
        $text = $item['sitename'];
        break;
      case 'subdomain':
        $text = $item['subdomain'];
        break;
      default:
        $text = $item['domain_id'];
        break;
    }

    switch ($this->options['link_option']) {
      case 'link_to_domain':
        $path = $item['subdomain'];
        break;
      default:
        $path = NULL;
        break;
    }

    if ($path) {
      return l($text, 'http://' . $path);
    }
    else {
      return $text;
    }
  }
}
