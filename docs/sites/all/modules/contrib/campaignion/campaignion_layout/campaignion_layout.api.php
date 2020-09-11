<?php

/**
 * @file
 * Document hooks invoked by this module.
 */

/**
 * Implements hook_campaignion_layout_info().
 *
 * @return array
 *   An associative array of custom layouts made available by this theme.
 *   Each entry is itself an associative array with the following keys:
 *   - title: The translated human readable title of this layout.
 *   - fields: An associative array of fields used by this layout. Each item is
 *     keyed by the field_name. Each value is itself an associative array with
 *     information how it is provided to the page template:
 *     - display: The display settings used for rendering this field for the
 *       page template. This can either be a view_mode or an array of settings.
 *       (default: Default formatter and settings for the field type.)
 *     - variable: Name of the variable used in the page template.
 *       (default: the field name)
 */
function hook_campaignion_layout_info() {
  $info['2col'] = [
    'title' => t('Two-column layout'),
    'fields' => [
      // Hide the background image unless this layout is selected.
      'layout_background_image' => [],
      'field_banner' => [
        'display' => 'full',
        'variable' => 'background_image',
      ],
    ],
  ];
  return $info;
}

/**
 * Implements hook_campaignion_layout_info_alter().
 */
function hook_campaignion_layout_info_alter(&$info) {
}

/**
 * List fields that should be hidden from the editing form by default.
 *
 * The field widgets stay hidden unless they are needed by the active layout.
 *
 * @return bool[]
 *   Boolean values keyed by field names. If a field name is in this array and
 *   has a truthy value then it will be hidden by default.
 */
function hook_campaignion_layout_dependent_fields() {
  $hidden_fields['layout_background_image'] = TRUE;
  return $hidden_fields;
}

/**
 * Alter the list of hidden fields.
 *
 * @param bool[] $hidden_fields
 *   Field names mapped to boolean values.
 *
 * @see hook_campaignion_layout_dependent_fields()
 */
function hook_campaignion_layout_dependent_fields_alter(array &$hidden_fields) {
  $hidden_fields['not_hidden_after_all'] = FALSE;
}
