<?php

/**
 * Defines a Entityreference behavior handler for Entityqueue.
 */
class EntityReferenceBehavior_EntityQueue extends EntityReference_BehaviorHandler_Abstract {

  /**
   * Overrides EntityReference_BehaviorHandler_Abstract::validate().
   */
  public function validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
    if ($entity_type == 'entityqueue_subqueue') {
      $queue = entityqueue_queue_load($entity->queue);

      $min_size = $queue->settings['min_size'];
      $max_size = $queue->settings['max_size'];
      $act_as_queue = isset($queue->settings['act_as_queue']) ? $queue->settings['act_as_queue'] : 0;

      $eq_items = array_filter($items, function ($value) {
          return (!empty($value["target_id"])) ? TRUE : FALSE;
      });

      if (count($eq_items) < $min_size && $entity->op != t('Add item')) {
        $errors[$field['field_name']][$langcode][0][] = array(
          'error' => 'entityqueue_min_size',
          'message' => t("The minimum number of items in this queue is @min_size.", array('@min_size' => $min_size)),
        );
      }
      elseif (!$act_as_queue && count($eq_items) > $max_size && $max_size > 0) {
        $errors[$field['field_name']][$langcode][count($items) - 1][] = array(
          'error' => 'entityqueue_max_size',
          'message' => t("The maximum number of items in this queue is @max_size.", array('@max_size' => $max_size)),
        );
      }
    }
  }

  /**
   * Overrides EntityReference_BehaviorHandler_Abstract::presave().
   */
  public function presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
    if ($entity_type == 'entityqueue_subqueue') {
      $queue = entityqueue_queue_load($entity->queue);

      $max_size = $queue->settings['max_size'];
      $act_as_queue = isset($queue->settings['act_as_queue']) ? $queue->settings['act_as_queue'] : 0;

      // Not all widgets can add to top, so we check if that option is set,
      // and default "bottom" as is normal for entity reference widgets.
      $add_position = isset($instance['widget']['settings']['add_position']) && $instance['widget']['settings']['add_position'] === 'top' ? 'top' : 'bottom';

      if ($act_as_queue) {
        $eq_items = array_filter($items, function ($value) {
          return (!empty($value["target_id"])) ? TRUE : FALSE;
        });

        // Remove items exceeding the limit.
        if (count($eq_items) > $max_size && $max_size > 0) {
          // Remove from the end if items are added to the top.
          if ($add_position === 'top') {
            $items = array_slice($eq_items, 0, $max_size);
          }
          // Or remove from the beginning if items are added to the bottom.
          else {
            $items = array_slice($eq_items, -$max_size);
          }
        }
      }
    }
  }

}
