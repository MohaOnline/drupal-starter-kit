<?php

/**
 * @file
 * Definition of wsfields_views_handler_field.
 */

/**
 * Field handler to present data from wsfields
 *
 * @ingroup views_field_handlers
 */
class wsfields_views_handler_field extends views_handler_field_entity {

  function get_value($values, $field = NULL) {
    $field = $this->field;
    if (isset($this->entities[$this->view->row_index])) {
      $entity = $this->entities[$this->view->row_index];

      // Support to get a certain part of the entity.
      if (isset($field) && isset($entity->{$field})) {
        $ret = array();

        $field_info = field_info_field($field);
        if (isset($field_info['translatable']) && $field_info['translatable']) {
          global $language ;
          $langcode = $language->language;
        } else {
          $langcode = LANGUAGE_NONE;
        }

        foreach($entity->{$field}[$langcode] as $key => $delta) {          
          $val = field_view_value($this->entity_type, $entity, $field, $delta, array(), $langcode);
          $ret[] = $val['#markup'];
        }
        $ret = implode(', ', $ret);
        return $ret;
      }
    }
    return FALSE;
  }

  function render($values) {
    return $this->get_value($values);
  }

}
