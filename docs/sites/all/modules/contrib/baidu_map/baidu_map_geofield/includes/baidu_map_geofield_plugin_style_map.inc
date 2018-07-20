<?php

/**
 * @file
 * This file holds the style plugin for Baidu Map Geofield.
 *
 * @ingroup baidu_map_geofield
 */

/**
 * @class
 * Extension of the Views Plugin Syle for Baidu Map Geofield.
 */
class baidu_map_geofield_plugin_style_map extends views_plugin_style {
  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['data_source'] = array('default' => '');
    $options['popup_source'] = array('default' => '');
    $options['alt_text'] = array('default' => '');
    // See: baidu_map_geofield_field_formatter_info.
    $options['baidu_map_geofield_width'] = array('default' => '100%');
    $options['baidu_map_geofield_height'] = array('default' => '300px');
    $options['baidu_map_geofield_zoom'] = array('default' => 'auto');
    $options['baidu_map_geofield_type'] = array('default' => 'normal');
    $options['baidu_map_geofield_style'] = array('default' => 'normal');
    $options['baidu_map_geofield_showtraffic'] = array('default' => FALSE);
    $options['baidu_map_geofield_navigationcontrol'] = array('default' => '');
    $options['baidu_map_geofield_scrollwheel'] = array('default' => FALSE);
    $options['baidu_map_geofield_draggable'] = array('default' => TRUE);
    $options['baidu_map_geofield_maptypecontrol'] = array('default' => FALSE);
    $options['baidu_map_geofield_scalecontrol'] = array('default' => FALSE);
    return $options;
  }


  /**
   * Provide additional settings for the options form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $handlers = $this->display->handler->get_handlers('field');

    $data_source_options = $popup_source_options = array(
      '' => '<none>',
    );

    foreach ($handlers as $handle) {
      $popup_source_options[$handle->options['id']] = (!empty($handle->options['label'])) ? $handle->options['label'] : $handle->options['id'];

      if (!empty($handle->field_info['type']) && $handle->field_info['type'] == 'geofield') {
        $data_source_options[$handle->options['id']] = (!empty($handle->options['label'])) ? $handle->options['label'] : $handle->options['id'];
      }
    }

    if (count($data_source_options) == 1) {
      $form['error'] = array(
        '#markup' => t('Please add at least 1 geofield to the view'),
      );
    }
    else {

      // Map Preset.
      $form['data_source'] = array(
        '#type' => 'select',
        '#title' => t('Data Source'),
        '#description' => t('Which field contains geodata?'),
        '#options' => $data_source_options,
        '#default_value' => $this->options['data_source'] ? $this->options['data_source'] : '',
      );

      $form['popup_source'] = array(
        '#type' => 'select',
        '#title' => t('Popup Text'),
        '#options' => $popup_source_options,
        '#default_value' => $this->options['popup_source'] ? $this->options['popup_source'] : '',
      );

      $form['alt_text'] = array(
        '#type' => 'textarea',
        '#title' => t('Alternate Text'),
        '#description' => t('This text shows up when a user does not have javascript enabled'),
        '#default_value' => $this->options['alt_text'] ? $this->options['alt_text'] : '',
      );

      $form = baidu_map_geofield_settings_form($this->options, $form);
    }
  }

  /**
   * Renders views as a Baidu Map.
   */
  function render() {
    // The Baidu Map API key is required to load the Maps.
    $api_key = variable_get('baidu_map_api_key');
    if (empty($api_key)) {
      // Let users know the Baidu Map API key was not properly configured.
      return t('Unable to display Baidu Map: no value found for the API Key.');
    }
    else {
      geophp_load();
      $style_options = $this->view->style_plugin->options;

      $geo_data = (!empty($style_options['data_source'])) ? 'field_' . $style_options['data_source'] : NULL;
      $popup_data = (!empty($style_options['popup_source'])) ? $style_options['popup_source'] : NULL;

      if ($geo_data) {
        $this->render_fields($this->view->result);
        $data = array();

        foreach ($this->view->result as $id => $result) {
          $geofield_handler = $this->view->field[$style_options['data_source']];
          $geofields = $geofield_handler->get_value($result);

          // For a new field value with no geocoding results geom could be NULL.
          if (!empty($geofields)) {
            // Support muilt field.
            foreach ($geofields as $geofield) {
              if (empty($geofield['geom'])) {
                continue;
              }
              $description = ($popup_data ? $this->rendered_fields[$id][$popup_data] : '');
              $geometry = geoPHP::load($geofield['geom']);
              $datum = json_decode($geometry->out('json'));
              $datum->properties = array(
                'description' => $description,
              );
              $data[] = $datum;
            }
          }
        }

        if (count($data) == 1) {
          $data = $data[0];
        }
        elseif (count($data) > 1) {
          $tmp = $data;
          $data = array(
            'type' => 'GeometryCollection',
            'geometries' => $tmp,
          );
        }

        $map_settings = baidu_map_geofield_settings_do($style_options);
        $container_id = drupal_html_id($this->view->name . '_' . $this->view->current_display);

        $js_settings = array(
          $container_id => array(
            'map_id' => $container_id,
            'map_settings' => $map_settings,
            'data' => $data,
          ),
        );

        drupal_add_js(array('geofieldBaiduMap' => $js_settings), 'setting');
      }

      // Supports Baidu Map JS API V2. At this stage, Baidu API key is defined.
      drupal_add_js('//api.map.baidu.com/api?v=2.0&ak=' . $api_key, 'external');
      drupal_add_js(drupal_get_path('module', 'baidu_map_geofield') . '/js/GeoJSON.js');
      drupal_add_js(drupal_get_path('module', 'baidu_map_geofield') . '/js/baidu_map_geofield.js');
      drupal_add_css(drupal_get_path('module', 'baidu_map_geofield') . '/css/baidu_map_geofield.css');

      // Default settings.
      $width = '100%';
      $height = '300px';
      // Dynamically set style settings based on configuration.
      if ($style_options['baidu_map_geofield_width']) {
        $width = $style_options['baidu_map_geofield_width'];
      }
      if ($style_options['baidu_map_geofield_height']) {
        $height = $style_options['baidu_map_geofield_height'];
      }

      return format_string('<div style="width: @width; height: @height" id="@container_id" class="geofieldBaiduMap">@alt_text</div>', array(
        '@width' => $width,
        '@height' => $height,
        '@container_id' => $container_id,
        '@alt_text' => $style_options['alt_text'],
      ));
    }
  }
}
