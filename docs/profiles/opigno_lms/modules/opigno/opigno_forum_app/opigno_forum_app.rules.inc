<?php

/**
 * @file
 * Rules file. Defines opigno forum rules.
 */

/**
 * Implements hook_rules_condition_info().
 */
function opigno_forum_app_rules_condition_info() {
  return array(
    'opigno_forum_app_is_checked' => array(
      'label' => t('Forum tool enabled'),
      'parameter' => array(
        'node' => array(
          'label' => t('Node'),
          'type' => 'node',
        ),
      ),
      'group' => t('Opigno Forum app'),
    ),
  );
}

/**
 * Custom rules condition callback.
 *
 * Detect if 'Forum' tool was enabled on course.
 *
 * @param object $node
 *   Node object.
 *
 * @return bool
 *   True if tools is enabled.
 */
function opigno_forum_app_is_checked($node) {
  // Get tools field values.
  $tools_field = field_get_items('node', $node, 'opigno_course_tools');
  // Check that field is not empty.
  if (!empty($tools_field)) {
    foreach ($tools_field as $enabled_tool) {
      // Check that 'forum' checkbox is checked.
      if ($enabled_tool['tool'] == 'forum') {
        return TRUE;
      }
    }
  }
  return FALSE;
}