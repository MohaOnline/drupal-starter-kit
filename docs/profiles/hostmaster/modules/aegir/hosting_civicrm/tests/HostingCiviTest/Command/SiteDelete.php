<?php

namespace HostingCiviTest\Command;

class SiteDelete extends \HostingCiviTest\Command {
  /**
   * Helper function to install a platform.
   */
  public static function run($site) {
    // FIXME: normally we should use backend_invoke_foo(), but the
    // hostmaster context was not successfully bootstrapped, so the
    // commands aren't found.
    self::exec('drush @hm provision-civicrm-tests-delete-site', [$site]);
    self::exec('drush @hm provision-civicrm-tests-run-pending');
  }
}
