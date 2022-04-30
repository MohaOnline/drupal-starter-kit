<?php

namespace Drupal\openlayers\Types;

use Drupal\openlayers\Openlayers;

/**
 * FIX: Insert short comment here.
 */
abstract class Map extends Base implements MapInterface {

  /**
   * FIX: Insert short comment here.
   *
   * @return MapInterface
   *   The Map object.
   */
  public function addLayer(LayerInterface $layer) {
    return $this->addObject($layer);
  }

  /**
   * {@inheritdoc}
   *
   * @return MapInterface
   *   The Map object.
   */
  public function addControl(ControlInterface $control) {
    return $this->addObject($control);
  }

  /**
   * {@inheritdoc}
   *
   * @return MapInterface
   *   The Map object.
   */
  public function addInteraction(InteractionInterface $interaction) {
    return $this->addObject($interaction);
  }

  /**
   * {@inheritdoc}
   *
   * @return MapInterface
   *   The Map object.
   */
  public function addComponent(ComponentInterface $component) {
    return $this->addObject($component);
  }

  /**
   * {@inheritdoc}
   *
   * @return MapInterface
   *   The Map object.
   */
  public function removeLayer($layer_id) {
    return $this->removeObject($layer_id);
  }

  /**
   * {@inheritdoc}
   *
   * @return MapInterface
   *   The Map object.
   */
  public function removeComponent($component_id) {
    return $this->removeObject($component_id);
  }

  /**
   * {@inheritdoc}
   *
   * @return MapInterface
   *   The Map object.
   */
  public function removeControl($control_id) {
    return $this->removeObject($control_id);
  }

  /**
   * {@inheritdoc}
   *
   * @return MapInterface
   *   The Map object.
   */
  public function removeInteraction($interaction_id) {
    return $this->removeObject($interaction_id);
  }

  /**
   * {@inheritdoc}
   */
  public function attached() {
    $attached = parent::attached();

    $settings = $this->getCollection()->getJs();
    $settings['map'] = array_shift($settings['map']);

    $attached['js'][] = array(
      'data' => array(
        'openlayers' => array(
          'maps' => array(
            $this->getId() => $settings,
          ),
        ),
      ),
      'type' => 'setting',
    );

    return $attached;
  }

  /**
   * {@inheritdoc}
   */
  public function getJs() {
    $js = parent::getJs();
    unset($js['opt']['capabilities']);
    return $js;
  }

  /**
   * {@inheritdoc}
   */
  public function render() {
    $build = $this->build();
    return drupal_render($build);
  }

  /**
   * {@inheritdoc}
   */
  public function build(array $build = array()) {
    $map = $this;

    // If this is an asynchronous map flag it as such.
    if ($asynchronous = $this->isAsynchronous()) {
      $map->setOption('async', $asynchronous);
    }

    if (!$map->getOption('target', FALSE)) {
      $this->setOption('target', $this->getId());
    }

    // Transform the options into objects.
    $map->getCollection()->import($map->optionsToObjects());

    // Run prebuild hook to all objects who implements it.
    $map->preBuild($build, $map);

    $capabilities = array();
    if ((bool) $this->getOption('capabilities', FALSE) === TRUE) {
      $items = array_values($this->getOption(array(
        'capabilities',
        'options',
        'table',
      ), array()));
      array_walk($items, 'check_plain');

      $capabilities = array(
        '#weight' => 1,
        '#type' => $this->getOption(array(
          'capabilities',
          'options',
          'container_type',
        ), 'fieldset'),
        '#collapsed' => TRUE,
        '#collapsible' => TRUE,
        '#attached' => array(
          'library' => array(
            array('system', 'drupal.collapse'),
          ),
        ),
      /*
        '#attributes' => array(
          'class' => array(
            $this->getOption(array(
              'capabilities',
              'options',
              'collapsible',
            ), TRUE) ? 'collapsible' : '',
            $this->getOption(array(
              'capabilities',
              'options',
              'collapsed',
            ), TRUE) ? 'collapsed' : '',
          ),
        ),
       */
        '#title' => $this->getOption(array(
          'capabilities',
          'options',
          'title',
        ), NULL),
        '#description' => $this->getOption(array(
          'capabilities',
          'options',
          'description',
        ), NULL),
        array(
          '#theme' => 'item_list',
          '#items' => $items,
          '#title' => '',
          '#type' => 'ul',
        ),
      );
    }

    $build = array(
      '#theme' => 'openlayers',
      '#map' => $map,
      '#attached' => $map->getCollection()->getAttached(),
      'map_prefix' => array(),
      'map_suffix' => array(),
      'capabilities' => $capabilities,
    );

    $map->postBuild($build, $map);

    return $build;
  }

  /**
   * {@inheritdoc}
   */
  public function optionsToObjects() {
    $import = array();

    // FIX: Simplify this.
    // Add the objects from the configuration.
    foreach (Openlayers::getPluginTypes(array('map')) as $weight_type => $type) {
      foreach ($this->getOption($type . 's', array()) as $weight => $object) {
        if (!$this->getCollection()->getObjectById($type, $object)) {
          if ($merge_object = Openlayers::load($type, $object)) {
            $merge_object->setWeight($weight_type . '.' . $weight);
            $import[$type . '_' . $merge_object->getMachineName()] = $merge_object;
          }
        }
      }
    }

    foreach ($this->getCollection()->getFlatList() as $object) {
      $import[$object->getType() . '_' . $object->getMachineName()] = $object;
    }

    return $import;
  }

  /**
   * {@inheritdoc}
   */
  public function setSize(array $size = array()) {
    list($width, $height) = array_values($size);
    return $this->setOption('width', $width)->setOption('height', $height);
  }

  /**
   * {@inheritdoc}
   */
  public function getSize() {
    return array($this->getOption('width'), $this->getOption('height'));
  }

  /**
   * {@inheritdoc}
   */
  public function setTarget($target) {
    return $this->setOption('target', $target);
  }

  /**
   * {@inheritdoc}
   */
  public function getTarget() {
    return $this->getOption('target');
  }

  /**
   * {@inheritdoc}
   */
  public function isAsynchronous() {
    return array_reduce($this->getDependencies(), function ($res, $obj) {
      return $res + (int) $obj->isAsynchronous();
    }, 0);
  }

}
