<?php

namespace Drupal\openlayers_library\Plugin\Component\IconSprites;

use Drupal\openlayers\Types\Component;

/**
 * FIX - insert comment here.
 *
 * @OpenlayersPlugin(
 *   id = "IconSprites"
 * )
 */
class IconSprites extends Component {

  /**
   * {@inheritdoc}
   */
  public function getJs() {
    $js = parent::getJs();
    $js['opt']['url'] = file_create_url(drupal_get_path('module', 'openlayers_examples') . '/assets/Butterfly.png');

    return $js;
  }

}
