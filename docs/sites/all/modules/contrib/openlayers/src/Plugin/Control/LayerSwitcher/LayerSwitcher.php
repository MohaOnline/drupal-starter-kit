<?php

namespace Drupal\openlayers\Plugin\Control\LayerSwitcher;

use Drupal\openlayers\Openlayers;
use Drupal\openlayers\Types\Control;
use Drupal\openlayers\Types\ObjectInterface;

/**
 * FIX - Insert short comment here.
 *
 * Proof of concept based on http://geocre.github.io/ol3/layerswitcher.html.
 * 
 * This presents the layer options as a reorderable table: upon rendering,
 * layer labels are listed in the corresponding order.
 *
 * Each layer is now associated with an initial selected state (that is
 * preset with the layer's visible option) that determines if the layer
 * is visible or not upon initial rendering.
 *
 * Options internal structure is updated. Old-style structure is supported
 * via default option values for the sake of compatibility.
 *
 * Switcher title can be disabled by specifying it as '<none>'.
 *
 * @OpenlayersPlugin(
 *  id = "LayerSwitcher",
 *  description = "Provides a layer switcher control."
 * )
 */
class LayerSwitcher extends Control {

  /**
   * Get layers from options, converting them from old storage style if
   * needed and decorating them with additional properties.
   */
  private function getOptionLayers() {
    $option_layers = $this->getOption('layers', array());
    $labels = $this->getOption('layer_labels', array());
    $layers = array();
    $weight = 0;

    foreach ($option_layers as $machine_name => $params) {
      $name = $machine_name;
      $selected = TRUE;
      if (($map_layer = Openlayers::load('Layer', $machine_name)) == TRUE) {
        $name = $map_layer->getName();
        $selected = $map_layer->getOption('visible', 1) != 0;
      }

      if (!is_array($params)) {    /* Old-style options. */
        $label = isset($labels[$machine_name])? $labels[$machine_name]: $name;
        $params = array('label' => $label, 'selected' => $selected);
      }

      $params['name'] = $name;
      $params['enabled'] = TRUE;
      $params['weight'] = $weight++;

      $layers[$machine_name] = $params;
    }

    return $layers;
  }

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {

    // Get, decorate and order all layers.
    $all_layers = Openlayers::loadAllAsOptions('Layer');
    foreach ($all_layers as $machine_name => $name) {
      $selected = TRUE;
      if (($layer = Openlayers::load('Layer', $machine_name)) == TRUE)
        $selected = $layer->getOption('visible', 1) != 0;
      $all_layers[$machine_name] = array('name' => $name,
                                         'weight' => PHP_INT_MAX,
                                         'label' => $name,
                                         'selected' => $selected,
                                         'enabled' => FALSE,
                                        );
    }
    $all_layers = array_merge($all_layers, $this->getOptionLayers());
    uasort($all_layers, function($a, $b) {
      if ($a['enabled'] != $b['enabled'])
        return $a['enabled']? -1: 1;
      if ($a['weight'] != $b['weight'])
        return $a['weight'] - $b['weight'];
      if ($a['name'] != $b['name'])
        return strcmp($a['name'], $b['name']);
      return strcmp($a['name'], $b['name']);
    });

    // Build layers table.
    $rows = array();
    $row_elements = array();
    $weight = 0;
    foreach ($all_layers as $id => $layer) {
      $rows[$id] = array(
        'data' => array(
          check_plain($layer['name']),
          array(
            'data' => array(
              '#type' => 'checkbox',
              '#title' => t('Enable'),
              '#title_display' => 'invisible',
              '#default_value' => $layer['enabled'],
              '#parents' => array('options', 'layers', $id, 'enabled'),
            ),
          ),
          array(
            'data' => array(
              '#type' => 'textfield',
              '#title' => t('Label'),
              '#title_display' => 'invisible',
              '#default_value' => $layer['label'],
              '#parents' => array('options', 'layers', $id, 'label'),
            ),
          ),
          array(
            'data' => array(
              '#type' => 'checkbox',
              '#title' => t('Selected'),
              '#title_display' => 'invisible',
              '#default_value' => $layer['selected'],
              '#parents' => array('options', 'layers', $id, 'selected'),
            ),
          ),
          array(
            'data' => array(
              '#type' => 'weight',
              '#title' => t('Weight'),
              '#title_display' => 'invisible',
              '#default_value' => $weight++,
              '#parents' => array('options', 'layers', $id, 'weight'),
              '#attributes' => array(
                'class' => array('layerswitcher-order-weight'),
              ),
            ),
          ),
        ),
        'class' => array('draggable'),
      );
      $row_elements[$id] = array(
        'enabled' => &$rows[$id]['data'][1]['data'],
        'label' => &$rows[$id]['data'][2]['data'],
        'selected' => &$rows[$id]['data'][3]['data'],
        'weight' => &$rows[$id]['data'][4]['data'],
      );
    }

    // Build form.

    $form['options']['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Title of the control'),
      '#default_value' => $this->getOption('label', 'Layers'),
    );
    $form['options']['layers'] = array(
      '#theme' => 'table',
      'elements' => $row_elements,
      '#header' => array(
        array('data' => t('Name')),
        array('data' => t('Enabled')),
        array('data' => t('Label')),
        array('data' => t('Selected')),
        array('data' => t('Weight')),
      ),
      '#rows' => $rows,
      '#empty' => t('There are no entries available.'),
      '#attributes' => array('id' => 'layerswitcher-order'),
    );
    drupal_add_tabledrag('layerswitcher-order',
                         'order', 'sibling', 'layerswitcher-order-weight');
    $form['options']['multiselect'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow selecting multiple layers'),
      '#default_value' => $this->getOption('multiselect', FALSE),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function optionsFormSubmit(array $form, array &$form_state) {

    // Process labels i18n updates, order layers and drop unneeded values.
    if (isset($form_state['values']['options'])) {
      $all_layers = Openlayers::loadAllAsOptions('Layer');
      $options = &$form_state['values']['options'];
      if (isset($options['layers'])) {
        $layers = &$options['layers'];
        uasort($layers, function($a, $b) {
          if ($a['enabled'] != $b['enabled'])
            return $a['enabled']? -1: 1;
          return $a['weight'] - $b['weight'];
        });
        foreach ($layers as $id => $layer) {
          $label = '';
          if (!$layer['enabled'])
            unset($layers[$id]);
          else {
            $label = $layer['label'];
            if ($label == '')
              $label = isset($all_layers[$id])? $all_layers[$id]: $id;
            unset($layers[$id]['weight']);
            unset($layers[$id]['enabled']);
          }
          if ($label != '')
            openlayers_i18n_string_update('openlayers:layerswitcher:' . $this->getMachineName() . ':' . $id . ':label', $layer['label']);
          else
            openlayers_i18n_string_remove('openlayers:layerswitcher:' . $this->getMachineName() . ':' . $id . ':label');
        }
      }
    }

    // Pass updated form state to parent class method.
    parent::optionsFormSubmit($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function preBuild(array &$build, ObjectInterface $context = NULL) {
    $map_id = $context->getId();
    $layers = $this->getOptionLayers();
    $items = array();
    $map_layers = array();
    foreach ($context->getObjects('layer') as $map_layer)
      $map_layers[$map_layer->getMachineName()] = $map_layer;

    $multiselect = $this->getOption('multiselect', FALSE);
    $element_type = $multiselect ? 'checkbox' : 'radio';

    // Only handle layers available in the map and configured in the control.
    // @TODO: use Form API (with form_process_* and stuff)
    $selection = FALSE;
    foreach ($layers as $id => $layer) {
      if (!isset($map_layers[$id]))
        unset($layers[$id]);
      else if ($layer['selected']) {
        if (!$multiselect && $selection)
          $layers[$id]['selected'] = FALSE;
        $selection = TRUE;
      }
    }
    if (!$multiselect && !$selection) {
      foreach ($layers as $id => $layer) {
        $layers[$id]['selected'] = TRUE;
        break;
      }
    }
    foreach ($layers as $id => $layer) {
      $classes = array(drupal_html_class($id));
      $checked = $layer['selected']? 'checked="checked" ': '';
      $label = openlayers_i18n_string('openlayers:layerswitcher:' . $this->getMachineName() . ':' . $id . ':label', $layer['label'], array('sanitize' => TRUE));
      $items[] = array(
        'data' => '<label><input type="' . $element_type . '" name="layer" ' . $checked . 'value="' . $id . '">' . $label . '</label>',
        'id' => drupal_html_id($map_id . '-' . $id),
        'class' => $classes,
      );
    }

    $layerswitcher = array(
      '#theme' => 'item_list',
      '#type' => 'ul',
      '#items' => $items,
      '#attributes' => array(
        'id' => drupal_html_id($this->getMachineName() . '-items'),
      ),
    );
    $title = $this->getOption('label', 'Layers');
    if ($title != '<none>') {
      $title = openlayers_i18n_string('openlayers:layerswitcher:' . $this->getMachineName() . ':title', $title, array('sanitize' => TRUE));
      $layerswitcher['#title'] = $title;
    }
    $this->setOption('element', '<div id="' . drupal_html_id($this->getMachineName()) . '" class="' . drupal_html_class($this->getMachineName()) . ' layerswitcher">' . drupal_render($layerswitcher) . '</div>');

    // Allow the parent class to perform it's pre-build actions.
    parent::preBuild($build, $context);
  }

  /**
   * {@inheritdoc}
   */
  public function i18nStringsRefresh() {
    // Register string in i18n string if possible.
    $title = $this->getOption('label', 'Layers');
    if ($title != '<none>')
      openlayers_i18n_string_update('openlayers:layerswitcher:' . $this->getMachineName() . ':title', $title);
    else
      openlayers_i18n_string_remove('openlayers:layerswitcher:' . $this->getMachineName() . ':title');
  }

}
