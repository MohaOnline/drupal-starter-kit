<?php

//namespace Drupal\openlayers_views\views;

use Drupal\openlayers\Openlayers;
use Drupal\openlayers\Types\Error;

/**
 * FIX - Insert short comment here.
 */
// openlayers_views_plugin_map_views extends openlayers_views_plugin_style_source_vector {
//class openlayers_views_plugin_map_views extends OpenlayersViewsPluginStyleSourceVector {
class OpenlayersViewsPluginMapViews extends OpenlayersViewsPluginStyleSourceVector {
//class OpenlayersViewsPluginMapViews extends openlayers_views_plugin_style_source_vector {

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['base_map_layer'] = array('default' => '');
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
    $options = array();
    foreach (Openlayers::loadAll('map') as $map) {
      foreach ($map->getCollection()->getObjects('layer') as $layer) {
        $key = $map->getMachineName() . ':' . $layer->getMachineName();
        $value = sprintf("%s: %s (%s)", $map->getName(), $layer->getName(), $layer->getMachineName());
        $options[$map->getName()][$key] = $value;
      }
    }

    $form['base_map_layer'] = array(
      '#type' => 'select',
      '#title' => 'Base map to use and vector layer to fill with the views features',
      '#description' => 'You may choose any vector layer only.',
      '#options' => $options,
      '#default_value' => $this->options['base_map_layer'],
      '#weight' => 1,
      '#required' => TRUE,
    );

    parent::options_form($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function render() {
    $output = NULL;
    $style_plugin_options = $this->view->style_plugin->options;
    list($mapname, $layername) = explode(':', $style_plugin_options['base_map_layer'], 2);

    if (empty($this->view->style_plugin->options['skipMapRender'])) {
      /** @var \Drupal\openlayers\Types\MapInterface $map */
      if (($map = Openlayers::load('Map', $mapname)) == TRUE && !($map instanceof Error)) {
        /** @var \Drupal\openlayers\Types\LayerInterface $layer */
        if ($layer = $map->getCollection()->getObjectById('layer', $layername)) {
          $source = Openlayers::load('source', 'openlayers_views_source_' . $this->view->name . '_' . $this->view->current_display);
          $map->addLayer($layer->setSource($source));
        }
        $output = array(
          '#type' => 'openlayers',
          '#map' => $map,
        );
      }
    }
    else {
      $grouped_results = $this->render_grouping($this->view->result, $this->options['grouping']);
      $output = $this->mapFeatures($grouped_results);
    }

    return $output;
  }

}
