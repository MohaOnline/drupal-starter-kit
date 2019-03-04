<?php
/**
 * @file
 * Baconmockup support class.
 */

/**
 * Add support for baconmockup.com.
 */
class BaconmockupProvider extends BaseProvider {

  public function __construct($plugin) {
    parent::__construct($plugin);
    $this->provider_base_url = 'http://baconmockup.com';
  }

}
