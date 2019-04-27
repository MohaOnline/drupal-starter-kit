<?php

/**
 * @file
 * Hooks provided by the Office_hours module.
 */

/**
 * Alter the field items before formatting it.
 *
 * Use case: To use one OH widget to store times and to alter this to unset
 * some of these values, in my case, to show public holidays as closed.
 *
 * @param $items
 *   The items for this field.
 * @param array $context
 *   The variables passed to the formatter.
 *   - entity_type: The $entity_type.
 *   - entity: The $entity object.
 *   - field: The $field array.
 *   - instance: The $instance array.
 *   - langcode: The $langcode.
 *   - display: The $display array.
 */
function hook_office_hours_field_formatter_view_alter($items, $context) {
  $entity = $context['entity'];
  $entity_type = $context['entity_type'];
  $field = $context['field'];
  if ($entity_type == 'bean' && $entity->type == 'office_hours') {
    switch ($field['field_name']) {
      case 'field_office_hours':
        // The array of holidays that we are closed for.
        $holidays = drupal_map_assoc(array('2015-06-20', '2015-12-25', '2015-12-26'));
        
        $tz = new DateTimeZone('Australia/Brisbane');
        $this_week = new DateTime('now', $tz);
        $this_week->setISODate($this_week->format('Y'), $this_week->format('W'));
        foreach ($items as $delta => $item) {
          $key = $this_week->format('Y-m-d');
          if (isset($holidays[$key])) {
            unset($items[$delta]);
          }
          $this_week->modify('+1 day');
        }
        // Resets the delta values to 0, 1, 2, ...
        $items = array_values($items);
        $entity->field_office_hours[LANGUAGE_NONE] = $items;
        break;
    }
  }
}
