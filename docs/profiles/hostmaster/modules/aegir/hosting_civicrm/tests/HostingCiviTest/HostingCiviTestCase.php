<?php

namespace HostingCiviTest;

/**
 * Base class for Hosting CiviCRM unit tests
 *
 * Those tests will run in a bootstrapped Drush environment
 *
 * This should be ran in separate processes, which the following
 * annotation should do in 3.6 and above:
 *
 * This file is inspired from the UnitUnishTestCase.php file from Drush
 * and from CiviCRM's phpunit tests.
 *
 * @runTestsInSeparateProcesses
 */
abstract class HostingCiviTestCase extends \PHPUnit_Framework_TestCase {

  function __construct($name = NULL, array $data = array(), $dataName = '') {
    parent::__construct($name, $data, $dataName);
  }

  /**
   * Minimally bootstrap drush
   *
   * This is equivalent to the level DRUSH_BOOTSTRAP_NONE, as we
   * haven't run drush_bootstrap() yet. To do anything, you'll need to
   * bootstrap to some level using drush_bootstrap().
   *
   * @see drush_bootstrap()
   */
  public static function setUpBeforeClass() {
    parent::setUpBeforeClass();

    require_once('vendor/drush/drush/includes/preflight.inc');
    drush_preflight_prepare();

    // Try to do a full Hostmaster bootstrap
/*
    drush_bootstrap(DRUSH_BOOTSTRAP_DRUPAL_FULL);

    // Need to set DRUSH_COMMAND so that drush will be called and not phpunit
    define('DRUSH_COMMAND', '/usr/local/bin/drush');
*/
  }

  public static function tearDownAfterClass() {
    parent::tearDownAfterClass();
    \drush_postflight();
  }

  function drush_major_version() {
    return DRUSH_MAJOR_VERSION;
  }

}
