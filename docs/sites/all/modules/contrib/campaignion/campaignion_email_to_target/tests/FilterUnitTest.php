<?php

namespace Drupal\campaignion_email_to_target;

/**
 * Unit tests for the message filters.
 */
class FilterUnitTest extends \DrupalUnitTestCase {

  public function test_match_byName() {
    $f = Filter::fromArray(['type' => 'target-attribute', 'config' => ['attributeName' => 'first_name', 'operator' => '==', 'value' => 'test']]);
    $this->assertTrue($f->match(['first_name' => 'test']));
    $this->assertFalse($f->match(['first_name' => 'notest']));
  }

  public function test_match_nonExistingAttribute_doesNotMatch() {
    $f = Filter::fromArray(['type' => 'target-attribute', 'config' => ['attributeName' => 'contact.first_name', 'operator' => '==', 'value' => 'test']]);
    $this->assertFalse($f->match([]));
  }

  /**
   * Test reading values from nested contact attributes.
   */
  public function testNestedContactValues() {
    $f = Filter::fromArray([
      'type' => 'target-attribute',
      'config' => [
        'attributeName' => 'trust.country',
        'operator' => '==',
        'value' => 'Wales',
      ],
    ]);
    $target1['trust']['country'] = 'Wales';
    $this->assertTrue($f->match($target1));
    $target2 = [];
    $this->assertFalse($f->match($target2));
    $target3['trust'] = 'no-array';
    $this->assertFalse($f->match($target3));
    $target4['trust']['no-country'] = 42;
    $this->assertFalse($f->match($target4));
    $target5['trust']['country'] = 'England';
    $this->assertFalse($f->match($target5));
  }

}

