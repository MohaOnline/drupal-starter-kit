<?php

namespace Drupal\openlayers_ui\UI;

/**
 * Class openlayers_layers_ui.
 */
class OpenlayersLayers extends \OpenlayersObjects {

  /**
   * {@inheritdoc}
   */
  public function hook_menu(&$items) {
    parent::hook_menu($items);
    $items['admin/structure/openlayers/layers']['type'] = MENU_LOCAL_TASK;
    $items['admin/structure/openlayers/layers']['weight'] = -5;
  }

}
