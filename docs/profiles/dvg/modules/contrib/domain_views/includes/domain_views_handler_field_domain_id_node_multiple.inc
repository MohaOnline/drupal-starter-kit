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
class domain_views_handler_field_domain_id_node_multiple extends views_handler_field_prerender_list {

  var $allDomains= array();
  
  /**
   * Constructor to provide additional field to add.
   */
  function construct() {
    parent::construct();
  }

  function query() {
    $this->field_alias = 'nid';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_default_domain'] = array('default' => FALSE);
    $options['link_to_original_domain'] = array('default' => FALSE);
    $options['list_all_domains'] = array('default' => FALSE);

    $options['display_method'] = array('default' => 'domain_id');
    $options['link_option'] = array('default' => 'none');

    $options['all_domain_show'] = array('default' => 'domains');
    $options['all_domain_text'] = array('default' => 'All Domains');
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
    $form['all_domain_show'] = array(
      '#title' => t('All affiliates display'),
      '#type' => 'select',
      '#options' => array(
        'domains' => t('Domains'),
        'text' => t('Text'),
      ),
      '#default_value' => ($this->options['all_domain_show']) ? $this->options['all_domain_show'] : 'domains',
    );
    $form['all_domain_text'] = array(
      '#title' => t('All domain text'),
      '#type' => 'textfield',
      '#default_value' => ($this->options['all_domain_text']) ? $this->options['all_domain_text'] : 'All Domains',
      '#description' => t('The text to display when item is available on all domains.'),
      '#states' => array(
        'invisible' => array(
          ':input[name="options[all_domain_show]"]' => array('value' => 'domains'),
        ),
      ),
    );
  }

  function pre_render(&$values) {
    $nids = array();
    $this->items = array();

    foreach ($values as $result) {
      $nids[] = $result->nid;
    }

    if ($nids) {
      //$result = db_query("SELECT da.nid, da.gid AS domain_id, d.sitename, d.subdomain FROM {domain_access} da INNER JOIN {domain} d ON da.gid = d.domain_id WHERE da.realm = 'domain_id' AND da.nid IN (:nids) ORDER BY da.nid",
      //array(':nids' => $nids));
      $result = db_query("SELECT da.nid, da.gid AS domain_id, d.sitename, d.subdomain FROM {domain_access} da LEFT JOIN {domain} d ON da.gid = d.domain_id WHERE da.nid IN (:nids) ORDER BY da.gid",
          array(':nids' => $nids));
      foreach ($result as $row) {
        if($row->domain_id == 0){
          if ($this->options['all_domain_show'] == 'text') {
            $this->items[$row->nid][$row->domain_id]['sitename'] = check_plain($this->options['all_domain_text']);
          } elseif ($this->options['all_domain_show'] == 'domains') {
            if (!count($this->allDomains)) {
              $dresult = db_query("SELECT * FROM {domain} ORDER BY domain_id");
              foreach ($dresult as $drow) {
                $this->allDomains[$drow->domain_id] = $drow;
              }
            }
            foreach ($this->allDomains as $domain) {
              $this->items[$row->nid][$domain->domain_id]['sitename'] = check_plain($domain->sitename);
              $this->items[$row->nid][$domain->domain_id]['domain_id'] = $domain->domain_id;
              $this->items[$row->nid][$domain->domain_id]['subdomain'] = $domain->subdomain;
            }
          }
        } elseif(!isset($this->items[$row->nid][0])) {
          $this->items[$row->nid][$row->domain_id]['sitename'] = check_plain($row->sitename);
          $this->items[$row->nid][$row->domain_id]['domain_id'] = $row->domain_id;
          $this->items[$row->nid][$row->domain_id]['subdomain'] = $row->subdomain;
        }
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
