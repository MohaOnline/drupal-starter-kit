<?php

/**
 * @file
 * Shurly Views handler for short URL
 */

/**
 * Field handler to present a link to the short URL entry.
 */
class shurly_handler_field_short_url extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['source'] = 'source';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);
    $options['longshort'] = array('default' => 0, 'translatable' => FALSE);
    $options['link'] = array('default' => FALSE, 'translatable' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // this field will never be empty
    unset($form['empty']);
    unset($form['hide_empty']);
    unset($form['hide_empty']);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );

    $form['longshort'] = array(
      '#type' => 'radios',
      '#options' => array(
        0 => t('Output full URL including base path'),
        1 => t('Output only the short path'),
      ),
      '#default_value' => $this->options['longshort'],
    );
    $form['link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Output as link'),
      '#default_value' => $this->options['link'],
      '#description' => t('Wrap output with a link to the short URL. Use <em>Output this field as a link</em> above for more complex options.'),
    );
  }

  function render($values) {
    global $base_url;
    // rawurldecode() allows URLs to be shown with UTF8 characters
    // however, this could cause other problems...
    if (!empty($this->options['text'])) {
      $text = $this->options['text'];
    }
    else {
      if ($this->options['longshort']) {
        $text = $values->{$this->aliases['source']};
      }
      else {
        $text = rawurldecode(_surl($values->{$this->aliases['source']}, array('absolute' => TRUE, 'base_url' => variable_get('shurly_base', $base_url))));
      }
    }

    if ($this->options['link']) {
      $text = '<a href="' . _surl($values->{$this->aliases['source']}, array('absolute' => TRUE, 'base_url' => variable_get('shurly_base', $base_url))) . '">' . $text . '</a>';
    }

    return $text;
  }
}
