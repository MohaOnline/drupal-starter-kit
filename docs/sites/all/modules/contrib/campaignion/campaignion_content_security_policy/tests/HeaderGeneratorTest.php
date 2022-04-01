<?php

namespace Drupal\campaignion_content_security_policy;

use Upal\DrupalUnitTestCase;

/**
 * Test the header generator.
 */
class HeaderGeneratorTest extends DrupalUnitTestCase {

  /**
   * Test adding a header to the HTTP response.
   */
  public function testAddHeader() {
    $drupal = $this->createMock(Drupal::class);
    $drupal->expects($this->once())->method('addHeader')->with(
      'Content-Security-Policy',
      'frame-ancestors one.example.com two.example.com \'self\''
    );
    $domain_str = <<<STR
one.example.com
two.example.com

'self'
STR;
    $generator = HeaderGenerator::fromConfig($drupal, $domain_str);
    $generator->addHeaders();
  }

}
