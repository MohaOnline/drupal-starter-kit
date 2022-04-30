<?php

namespace Drupal\openlayers\Types;

/**
 * FIX - Insert short comment here.
 */
abstract class Style extends Base implements StyleInterface {
  /**
   * The array containing the options.
   *
   * @var array
   */
  protected $options;

  /**
   * {@inheritdoc}
   */
  public function getJs() {
    $js = parent::getJs();

    unset($js['opt']['styles']);

    return $js;
  }

}
