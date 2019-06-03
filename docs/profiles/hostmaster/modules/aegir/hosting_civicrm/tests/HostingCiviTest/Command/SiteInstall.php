<?php

namespace HostingCiviTest\Command;

class SiteInstall extends \HostingCiviTest\Command {
  /**
   * Helper function to install a site.
   */
  public static function run($platform_name, $site, $profile_name = 'standard') {
    // FIXME: normally we should use backend_invoke_foo(), but the
    // hostmaster context was not successfully bootstrapped, so the
    // commands aren't found.
    self::exec('drush @hm provision-civicrm-tests-install-site', [$platform_name, $site, $profile_name]);
    self::exec('drush @hm provision-civicrm-tests-run-pending');
  }

}
