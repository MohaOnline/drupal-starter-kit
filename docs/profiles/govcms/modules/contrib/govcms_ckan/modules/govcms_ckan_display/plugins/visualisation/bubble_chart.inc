<?php
/**
 * @file
 * Bubble chart visualisation.
 */

$plugin = array(
  'title' => t('Bubble chart'),
  'settings' => array(
    'keys' => array(),
    'labels' => NULL,
    'split' => NULL,
    'bar_width' => NULL,
    'rotated' => 'false',
    'stacked' => 0,
    'data_order' => 'desc',
    'show_labels' => 0,
    'grid' => NULL,
    'grid_lines' => array('lines' => array()),
    'palette_override' => NULL,
    'show_title' => 1,
    'x_label' => NULL,
    'y_label' => NULL,
    'column_overrides' => array(),
    'disable_chart_interaction' => 0,
    'disable_legend_interaction' => 0,
    'chart_padding' => array(),
    'merge_settings' => array(
      'merge_keys' => NULL,
    ),
    'label_settings' => array(
      'overrides' => NULL,
    ),
    'axis_settings' => array(
      'x_tick_rotate' => 0,
      'x_tick_count' => NULL,
      'y_tick_count' => NULL,
      'y_tick_values' => NULL,
      'x_tick_values' => NULL,
      'y_tick_value_format' => NULL,
      'x_tick_value_format' => NULL,
      'x_tick_cull' => NULL,
      'tick_visibility' => 'show',
      'x_padding' => array(),
      'y_padding' => array(),
      'y_rounding' => NULL,
      'x_type' => NULL,
      'x_date_format' => array(),
    ),
    'ckan_filters' => array(
      'search' => NULL,
      'filters' => NULL,
    ),
    'bubble_settings' => array(
      'tables_to_bubble' => 'false',
      'parents' => array(),
      'label' => NULL,
      'resource' => NULL,
    ),
  ),
);

/**
 * Returns a renderable array that represents the block content.
 */
function govcms_ckan_display_bubble_chart_view($file, $display, $config) {
  $element = array();
  $chart_class = 'ckan-bubble-chart';
  if (isset($file->resource_id)) {
    $ckan_search = govcms_ckan_get_config_value($config, 'ckan_filters/search');
    $ckan_filters = govcms_ckan_get_config_value($config, 'ckan_filters/filters');
    $response = govcms_ckan_client_request_records($file->resource_id, $ckan_search, $ckan_filters);
  }
  elseif (!empty($file->data)) {
    $response = new stdClass();
    $response->data = $file->data;
    $response->valid = TRUE;
  }

  // If failure, provide error message.
  if ($response->valid === FALSE) {
    $element['#theme'] = 'ckan_empty_visualisation';
  }
  else {

    // Setup our configuration.
    $keys = array_filter($config['keys']);
    $column_overrides = govcms_ckan_display_parse_column_overrides(govcms_ckan_get_config_value($config, 'column_overrides'));
    $label_replacements = govcms_ckan_string_to_array(govcms_ckan_get_config_value($config, 'label_settings/overrides'));
    $grid_lines = govcms_ckan_get_config_value($config, 'grid_lines/lines', array());

    // Attributes for the table.
    $attributes = array(
      'class' => array('ckan-chart', $chart_class),
      'data-type' => 'bubble',

      // Entity settings.
      'data-rotated' => govcms_ckan_get_config_value($config, 'rotated'),
      'data-stacked' => (govcms_ckan_get_config_value($config, 'stacked') == 1 ? 'true' : 'false'),
      'data-dataOrder' => govcms_ckan_get_config_value($config, 'data_order', 'desc'),
      'data-labels' => (govcms_ckan_get_config_value($config, 'show_labels') == 1 ? 'true' : 'false'),
      'data-grid' => govcms_ckan_get_config_value($config, 'grid'),
      'data-showTitle' => (govcms_ckan_get_config_value($config, 'show_title') == 1 ? 'true' : 'false'),
      'data-title' => $file->filename,
      'data-xLabel' => govcms_ckan_get_config_value($config, 'x_label'),
      'data-yLabel' => govcms_ckan_get_config_value($config, 'y_label'),
      'data-xTickRotate' => (govcms_ckan_get_config_value($config, 'axis_settings/x_tick_rotate') == 1 ? '90' : '0'),
      'data-xTickCount' => govcms_ckan_get_config_value($config, 'axis_settings/x_tick_count'),
      'data-yTickCount' => govcms_ckan_get_config_value($config, 'axis_settings/y_tick_count'),
      'data-yTickValues' => govcms_ckan_get_config_value($config, 'axis_settings/y_tick_values'),
      'data-xTickValues' => govcms_ckan_get_config_value($config, 'axis_settings/x_tick_values'),
      'data-yTickValueFormat' => govcms_ckan_get_config_value($config, 'axis_settings/y_tick_value_format'),
      'data-xTickValueFormat' => govcms_ckan_get_config_value($config, 'axis_settings/x_tick_value_format'),
      'data-xTickCull' => govcms_ckan_get_config_value($config, 'axis_settings/x_tick_cull'),
      'data-xTickType' => govcms_ckan_get_config_value($config, 'axis_settings/x_type'),
      'data-xDateFormat' => govcms_ckan_json_encode_attribute(govcms_ckan_get_config_value($config, 'axis_settings/x_date_format', array())),
      'data-yRounding' => govcms_ckan_get_config_value($config, 'axis_settings/y_rounding'),
      'data-tickVisibility' => govcms_ckan_get_config_value($config, 'axis_settings/tick_visibility', 'show'),
      'data-chartPadding' => govcms_ckan_json_encode_attribute(govcms_ckan_get_config_value($config, 'chart_padding', array())),
      'data-xPadding' => govcms_ckan_json_encode_attribute(govcms_ckan_get_config_value($config, 'axis_settings/x_padding', array())),
      'data-yPadding' => govcms_ckan_json_encode_attribute(govcms_ckan_get_config_value($config, 'axis_settings/y_padding', array())),
      'data-gridLines' => govcms_ckan_json_encode_attribute(govcms_ckan_array_valid_rows($grid_lines, 'value')),
      'data-disableChartInteraction' => (govcms_ckan_get_config_value($config, 'disable_chart_interaction') == 1 ? 'true' : 'false'),
      'data-disableLegendInteraction' => (govcms_ckan_get_config_value($config, 'disable_legend_interaction') == 1 ? 'true' : 'false'),

      // Display settings.
      'data-palette' => (!empty($config['palette_override']) ? $config['palette_override'] : govcms_ckan_get_config_value($config, 'palette')),
      'data-exportWidth' => govcms_ckan_get_config_value($config, 'export_width'),
      'data-exportHeight' => govcms_ckan_get_config_value($config, 'export_height'),
      'data-xAxisLabelPos' => govcms_ckan_get_config_value($config, 'x_axis_label_position'),
      'data-yAxisLabelPos' => govcms_ckan_get_config_value($config, 'y_axis_label_position'),
      'data-xTickCentered' => (govcms_ckan_get_config_value($config, 'x_axis_tick_centered', 1) == 1 ? 'true' : 'false'),

      // Bubble settings.
      'data-tablesToBubble' => govcms_ckan_get_config_value($config, 'bubble_settings/tables_to_bubble'),
      'data-parents' => govcms_ckan_json_encode_attribute(govcms_ckan_get_config_value($config, 'bubble_settings/parents')),
      'data-bubbleLabel' => govcms_ckan_get_config_value($config, 'bubble_settings/label'),
      'data-bubbleResource' => govcms_ckan_get_config_value($config, 'bubble_settings/resource'),
    );

    // Parse the data.
    $parser = govcms_ckan_dataset_parser($response->data);
    $parser
      ->setKeys($keys)
      ->setLabelKey($config['labels'])
      ->setHeaderSource($config['x_axis_grouping'])
      ->setTableAttributes($attributes);

    if ($column_overrides) {
      $parser
        ->setKeysOrder($column_overrides, 'data-weight')
        ->setColumnAttributes($column_overrides);
    }

    if ($label_replacements) {
      $parser->setLabelReplacements($label_replacements);
    }

    // If splitting on the value of a key.
    if ($config['split']) {
      $parser->setGroupKey($config['split']);
    }

    if ($config['merge_settings']['merge_keys']) {
      $parser->setMergeKeys($config['merge_settings']['merge_keys']);
    }

    // Return the parsed tables in a wrapper.
    $element = array(
      '#theme' => 'ckan_display_table_wrapper',
      '#tables' => $parser->parse(),
      '#show_titles' => !empty($config['split']),
    );

    // Add the JS to the tables.
    govcms_ckan_display_attach_charts($element, '.' . $chart_class);
  }

  return $element;
}

/**
 * Configure form callback.
 */
function govcms_ckan_display_bubble_chart_configure($plugin, $form, &$form_state, $config) {

  // Include default widgets.
  $widgets = array(
    'govcms_ckan_media_visualisation_default_key_config',
    'govcms_ckan_media_visualisation_default_axis_config',
    'govcms_ckan_media_visualisation_default_grid_config',
    'govcms_ckan_media_visualisation_default_chart_config',
  );
  $config_form = govcms_ckan_media_visualisation_include_form_widgets($form, $form_state, $config, $widgets);

  // Custom settings.
  $title = govcms_ckan_media_form_title_element(t('@title settings', array('@title' => $plugin['title'])));
  $config_form['title_custom_settings'] = $title;

  return $config_form;
}
