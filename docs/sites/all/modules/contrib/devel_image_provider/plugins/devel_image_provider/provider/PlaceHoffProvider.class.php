<?php
/**
 * @file
 * Place-hoff support class.
 */

/**
 * Add support for place-hoff.com.
 */
class PlaceHoffProvider extends BaseProvider {

  public function __construct($plugin) {
    parent::__construct($plugin);
    $this->provider_base_url = 'http://place-hoff.com';
  }

}
