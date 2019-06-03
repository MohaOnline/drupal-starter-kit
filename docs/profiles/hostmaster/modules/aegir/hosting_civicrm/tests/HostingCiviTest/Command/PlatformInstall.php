<?php

namespace HostingCiviTest\Command;

class PlatformInstall extends \HostingCiviTest\Command {

  /**
   * Helper function to install a platform.
   */
  public static function run($platform_name, $platform_alias = NULL) {
    // FIXME: normally we should use backend_invoke_foo(), but the
    // hostmaster context was not successfully bootstrapped, so the
    // commands aren't found.
    if (empty($platform_alias)) {
      $platform_alias = $platform_name;
    }

    if (is_dir("/var/aegir/platforms/$platform_alias")) {
      drush_log(dt('Platform: @platform already exists, skipping build.', array('@platform' => $platform_alias)), 'ok');
      return;
    }

    self::exec('drush @hm provision-civicrm-tests-install-platform', [$platform_name, $platform_alias]);
    self::exec('drush @hm provision-civicrm-tests-run-pending');
  }
}
