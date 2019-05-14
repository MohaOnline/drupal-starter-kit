<?php

function glazed_free_rows_from_field_collection($entity_id, $field_array) {
  $entity = field_collection_item_load($entity_id);
  if ($entity == FALSE) {
    return FALSE;
  }
  ${$entity->field_name} = array();
  $wrapper = entity_metadata_wrapper('field_collection_item', $entity);
  foreach($field_array as $field) {
    if (isset($wrapper->$field)) {
      ${$entity->field_name}[$field] = $wrapper->$field->value();
    }
    else {
      ${$entity->field_name}[$field] = NULL;
    }
  }
  return ${$entity->field_name};
}
