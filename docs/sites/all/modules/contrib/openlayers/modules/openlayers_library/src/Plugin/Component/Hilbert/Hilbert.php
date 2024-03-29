<?php

namespace Drupal\openlayers_library\Plugin\Component\Hilbert;

use Drupal\openlayers\Types\Component;
use Drupal\openlayers\Types\ObjectInterface;

/**
 * FIX - insert comment here.
 *
 * @OpenlayersPlugin(
 *   id = "Hilbert"
 * )
 */
class Hilbert extends Component {

  /**
   * {@inheritdoc}
   */
  public function postBuild(array &$build, ObjectInterface $context = NULL) {
    $build['parameters'][$this->getPluginId()] = array(
      '#type' => 'fieldset',
      '#title' => 'Example Hilbert Fractal component',
      'swipe' => array(
        '#type' => 'rangefield',
        '#title' => 'Depth',
        '#min' => 1,
        '#max' => 8,
        '#step' => 1,
        '#value' => 5,
        '#attributes' => array(
          'id' => 'depth',
          'style' => 'width: 100%;',
        ),
      ),
      'count' => array(
        '#type' => 'item',
        '#title' => 'Points',
        '#markup' => '<span id="count"></span>',
      ),
      'length' => array(
        '#type' => 'item',
        '#title' => 'Length',
        '#markup' => '<span id="length"></span>',
      ),
    );

    $build['description']['#description'] = 'This example is based on the <a href="http://openlayers.org/en/master/examples/fractal.html">offical fractal example</a>. You need the <em><a href="https://drupal.org/project/elements">elements</a></em> module to get it working properly.';
  }

}
