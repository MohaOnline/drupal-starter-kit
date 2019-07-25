<?php

/**
 * @file
 * Shurly Views handler for delete links function
 */

/**
 * Field handler to present a link to the short URL entry.
 */
class shurly_handler_field_shurly_link_delete extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = 'uid';
    $this->additional_fields['active'] = 'active';
    $this->additional_fields['rid'] = 'rid';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    global $user;
    $uid = $values->{$this->aliases['uid']};
    $active = $values->{$this->aliases['active']};
    if (!$active) {
      return t('deactivated');
    }
    // only allow the user to view the link if they can actually delete
    if (user_access('Administer short URLs') || (user_access('Delete own URLs') && $uid == $user->uid)) {
      $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
      $rid = $values->rid;
      return l($text, "shurly/delete/$rid", array('query' => drupal_get_destination()));
    }
  }
}
