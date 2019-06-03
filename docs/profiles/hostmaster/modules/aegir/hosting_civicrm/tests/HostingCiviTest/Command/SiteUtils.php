<?php

namespace HostingCiviTest\Command;

class SiteUtils extends \HostingCiviTest\Command {
  /**
   * Returns the CiviCRM version.
   */
  public static function getCiviVersion($site) {
    $info = self::getCiviSystemInfo($site);

    if (isset($info['civi']['version'])) {
      return $info['civi']['version'];
    }

    throw new \Exception("Could not find CiviCRM version: " . print_r($info, 1));
  }

  /**
   * Returns the CiviCRM system info.
   */
  public static function getCiviSystemInfo($site) {
    $site .= '.aegir.example.com';

    // Flush the drush cache to avoid 'civicrm must be enable' error.
    self::exec('drush @' . escapeshellcmd($site) . ' cc drush');

    // Run System.get API
    $output = self::execReturn('drush @' . escapeshellcmd($site) . ' cvapi System.get --out=json');
    $info = json_decode($output, TRUE);

    return $info['values'][0];
  }

  /**
   * Helper function to enable a CiviCRM extension on a site.
   */
  public static function enableExtension($site, $extension_name) {
    $site .= '.aegir.example.com';

    // Flush the drush cache to avoid 'civicrm must be enabled' error.
    self::exec('drush @' . escapeshellcmd($site) . ' cc drush');
    self::exec('drush @' . escapeshellcmd($site) . ' cvapi Extension.refresh');

    // Run the Extension.install API
    $output = self::execReturn('drush @' . escapeshellcmd($site) . ' cvapi Extension.install --out=json', ['key=' . $extension_name]);
    $info = json_decode($output, TRUE);

    if (! empty($info['is_error'])) {
      throw new \Exception("Failed to enable extension: " . $info['error_message']);
    }
  }

}
