<?php
/**
 * @file
 * Node Export related functionality.
 */

/**
 * Process file field on a node.
 *
 * @param string $entity_type
 *   The type of entity.
 * @param object $entity
 *   Entity object.
 * @param callable $process
 *   Processing callback.
 */
function file_md5_node_export_process_file_fields($entity_type, $entity, callable $process) {
  list(,, $bundle) = entity_extract_ids($entity_type, $entity);
  $fields_info = field_info_fields();
  $langcodes = field_language($entity_type, $entity);
  // @see node_export_file_field_export()
  $types = array_map('trim', explode(',', variable_get('node_export_file_supported_fields', 'file, image')));

  foreach (field_info_instances($entity_type, $bundle) as $field_name => $instance) {
    // By some reasons, for field of "file" and "image" types language
    // could be undefined.
    $langcode = isset($langcodes[$field_name]) ? $langcodes[$field_name] : LANGUAGE_NONE;

    if (isset($entity->{$field_name}) && isset($entity->{$field_name}[$langcode])) {
      // Processing non-empty field collections only.
      if ('field_collection' === $fields_info[$field_name]['type']) {
        // Walk through the items of collection.
        foreach ($entity->{$field_name}[$langcode] as $delta => &$item) {
          // The structure of existing items:
          // @code
          // [
          //   'value' => 12,
          //   'revision_id' => 14,
          // ];
          // @endcode
          if (is_array($item) && !array_key_exists('value', $item) && !array_key_exists('revision_id', $item)) {
            // An "entity" key will be present when host entity is set. In this
            // case nothing should be done.
            if (empty($item['entity'])) {
              /* @var \FieldCollectionItemEntity $new_entity */
              $new_entity = entity_create('field_collection_item', $item);
              // Field collection entity depends on another entity that must be
              // a parent for it.
              $new_entity->setHostEntity($entity_type, $entity, $langcode);
              // Make an import of nested collections come true.
              call_user_func_array(__FUNCTION__, [$new_entity->entityType(), $new_entity, $process]);
              // Completely remove existing item.
              unset($entity->{$field_name}[$langcode][$delta]);
            }
          }
          else {
            /* @var \FieldCollectionItemEntity $item */
            $item = field_collection_field_get_entity($item);

            // Prevent cases, when:
            // @code
            // [
            //   'value' => NULL,
            //   'revision_id' => NULL,
            // ];
            // @endcode
            if (FALSE !== $item) {
              // Make an export of nested collections come true.
              call_user_func_array(__FUNCTION__, [$item->entityType(), $item, $process]);
              // Redundant information should not be exported.
              unset($item->item_id, $item->revision_id, $item->default_revision);
              // Make an array from the object for easy export.
              $item = get_object_vars($item);
            }
          }
        }
      }

      if (in_array($fields_info[$field_name]['type'], $types)) {
        foreach ($entity->{$field_name}[$langcode] as $i => $item) {
          $process($entity->{$field_name}[$langcode][$i]);
        }

        $entity->{$field_name}[$langcode] = array_filter($entity->{$field_name}[$langcode]);
      }
    }
  }
}
