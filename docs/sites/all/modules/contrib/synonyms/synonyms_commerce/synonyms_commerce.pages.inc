<?php

/**
 * @file
 * Menu page callbacks of the module.
 */

/**
 * Menu page callback for synonyms commerce autocomplete widget.
 */
function synonyms_commerce_autocomplete($field_name, $entity_type, $bundle) {
  // If the request has a '/' in the search text, then the menu system will have
  // split it into multiple arguments, recover the intended $tags_typed.
  $args = func_get_args();
  // Shift off the $field_name argument.
  array_shift($args);
  // Shift off the $entity_type argument.
  array_shift($args);
  // Shift off the $bundle argument.
  array_shift($args);
  $tags_typed = implode('/', $args);

  if (!($field = field_info_field($field_name)) || $field['type'] != 'commerce_product_reference') {
    print t('Commerce product reference field @field_name not found.', array('@field_name' => $field_name));
    exit;
  }

  if (!($instance = field_info_instance($entity_type, $field['field_name'], $bundle))) {
    // Error string. The JavaScript handler will realize this is not JSON and
    // will display it as debugging information.
    print t('There was not found an instance of @field_name in @entity_type.', array(
      '@field_name' => $field_name,
      '@entity_type' => $entity_type,
    ));
    exit;
  }

  module_load_include('inc', 'synonyms', 'synonyms.pages');

  $widget = $instance['widget']['type'] == 'synonyms_commerce_autocomplete' ? $instance['widget']['settings'] : field_info_widget_settings('synonyms_commerce_autocomplete');

  // How many suggestions maximum we are able to output.
  $max_suggestions = $widget['suggestion_size'];

  // Whether we are allowed to suggest more than one entry per term, shall that
  // entry be either term name itself or one of its synonyms.
  $suggest_only_unique = $widget['suggest_only_unique'];

  $tags_typed = drupal_explode_tags($tags_typed);
  $tag_last = drupal_strtolower(array_pop($tags_typed));
  $prefix = count($tags_typed) ? drupal_implode_tags($tags_typed) . ', ' : '';

  $matches = array();

  if ($tag_last) {
    $tags_typed_entity_ids = array();

    if (!empty($tags_typed)) {
      foreach (commerce_product_load_multiple(array(), array('title' => $tags_typed)) as $product) {
        $product_ids = entity_extract_ids('commerce_product', $product);
        $tags_typed_entity_ids[] = $product_ids[0];
      }
    }

    $new_target_ids = array();

    foreach (commerce_product_match_products($field, $instance, $tag_last, 'contains', array(), $max_suggestions) as $product) {
      $product = commerce_product_load_by_sku($product['sku']);
      $product_id = entity_extract_ids('commerce_product', $product);
      $product_id = $product_id[0];
      if (!in_array($product_id, $tags_typed_entity_ids)) {
        $matches[] = array(
          'name' => entity_label('commerce_product', $product),
        );
        $new_target_ids[] = $product_id;
      }
    }

    if (count($matches) < $max_suggestions) {
      $target_bundles = synonyms_bundle_normalize('commerce_product', array_filter($instance['settings']['referenceable_types']));

      $behavior_implementations = synonyms_behavior_get('autocomplete', 'commerce_product', $target_bundles, TRUE);
      foreach ($behavior_implementations as $implementation) {
        $condition = db_and();
        $condition->condition(AbstractSynonymsBehavior::COLUMN_SYNONYM_PLACEHOLDER, '%' . db_like($tag_last) . '%', 'LIKE');

        if (!empty($tags_typed_entity_ids)) {
          $condition->condition(AbstractSynonymsBehavior::COLUMN_ENTITY_ID_PLACEHOLDER, $tags_typed_entity_ids, 'NOT IN');
        }
        if ($suggest_only_unique && !empty($new_target_ids)) {
          $condition->condition(AbstractSynonymsBehavior::COLUMN_ENTITY_ID_PLACEHOLDER, $new_target_ids, 'NOT IN');
        }

        foreach ($implementation['object']->synonymsFind($condition) as $synonym) {
          if (!$suggest_only_unique || !in_array($synonym->entity_id, $new_target_ids)) {
            $matches[] = array(
              'target_id' => $synonym->entity_id,
              'synonym' => $synonym->synonym,
              'behavior_implementation' => $implementation,
            );
            $new_target_ids[] = $synonym->entity_id;
            if (count($matches) == $max_suggestions) {
              break (2);
            }
          }
        }
      }
    }

    $synonym_entities = array();
    foreach ($matches as $match) {
      if (!isset($match['wording']) && isset($match['synonym'])) {
        $synonym_entities[] = $match['target_id'];
      }
    }
    if (!empty($synonym_entities)) {
      $synonym_entities = commerce_product_load_multiple($synonym_entities);
      foreach ($matches as $k => $match) {
        if (!isset($match['name']) && isset($match['synonym'])) {
          $entity_ids = entity_extract_ids('commerce_product', $synonym_entities[$match['target_id']]);
          $matches[$k]['name'] = entity_label('commerce_product', $synonym_entities[$match['target_id']]);
          $matches[$k]['bundle'] = $entity_ids[2];
        }
      }
      $matches = array_values($matches);
    }
  }

  drupal_json_output(synonyms_autocomplete_format($matches, $prefix));
}
