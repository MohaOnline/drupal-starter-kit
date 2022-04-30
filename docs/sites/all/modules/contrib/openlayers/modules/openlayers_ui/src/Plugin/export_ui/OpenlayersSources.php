<?php

namespace Drupal\openlayers_ui\UI;

/**
 * FIX - insert comment here.
 */
class OpenlayersSources extends \OpenlayersObjects {

  /**
   * {@inheritdoc}
   */
  public function hook_menu(&$items) {
    parent::hook_menu($items);
    $items['admin/structure/openlayers/sources']['type'] = MENU_LOCAL_TASK;
    $items['admin/structure/openlayers/sources']['weight'] = -1;
  }

}
