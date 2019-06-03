<?php

namespace Drupal\dvg_authentication_digid;

use Drupal\dvg_authentication\SamlUser;

/**
 * Class DigidUser.
 *
 * Adds DigiD specific values and actions to an ExternalUser object.
 */
class DigidUser extends SamlUser {

  /**
   * {@inheritdoc}
   */
  public function getValue($field_name) {
    if ($field_name === 'bsn') {
      return $this->getBsn();
    }
    return parent::getValue($field_name);
  }

  /**
   * Get the BSN of the DigiD User, if available.
   *
   * @return bool|string
   *   The full BSN number or false if the BSN is not available.
   */
  protected function getBsn() {

    if ($this->isDebugUser()) {
      // @todo: Return a debug BSN.
    }

    if (isset($_SESSION['dvg_authentication_digid'][DIGID_SECTOR_BSN])) {
      drupal_page_is_cacheable(FALSE);
      return str_pad($_SESSION['dvg_authentication_digid'][DIGID_SECTOR_BSN], 9, '0', STR_PAD_LEFT);
    }

    return FALSE;
  }

}
