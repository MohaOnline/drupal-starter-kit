<?php

namespace HostingCiviTest\Command;

class SiteMigrate extends \HostingCiviTest\Command {
  /**
   * Helper function to migrate a site to another platform.
   */
  public static function run($site, $target_platform) {
    // FIXME: normally we should use backend_invoke_foo(), but the
    // hostmaster context was not successfully bootstrapped, so the
    // commands aren't found.
    self::exec('drush @hm provision-civicrm-tests-migrate-site', [$site, $target_platform]);
    self::exec('drush @hm provision-civicrm-tests-run-pending');
  }
}
