<?php

namespace Drupal\campaignion_content_security_policy;

use Upal\DrupalUnitTestCase;

/**
 * Test module level functions.
 */
class ModuleTest extends DrupalUnitTestCase {

  /**
   * Test that the config variable is declared properly.
   */
  public function testVariableInfo() {
    $info = variable_get_info('campaignion_content_security_policy_trusted_frame_ancestors');
    $this->assertNotEmpty($info);
  }

}
