<?php

namespace HostingCiviTest\Command;

class PlatformDelete extends \HostingCiviTest\Command {

  /**
   * Helper function to remove a platform.
   */
  public static function run($platform_name) {
    // FIXME: normally we should use backend_invoke_foo(), but the
    // hostmaster context was not successfully bootstrapped, so the
    // commands aren't found.
    self::exec('drush @hm hosting-task', ["@platform_$platform_name", 'delete']);
    self::exec('drush @hm provision-civicrm-tests-run-pending');
  }

}
