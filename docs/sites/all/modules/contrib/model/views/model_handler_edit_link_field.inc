<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class model_handler_edit_link_field extends model_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy model to check access against
    $dummy_model = (object) array('type' => $type);
    if (!model_access('edit', $dummy_model)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $model_id = $values->{$this->aliases['model_id']};
    
    return l($text, 'admin/content/models/model/' . $model_id . '/edit');
  }
}
