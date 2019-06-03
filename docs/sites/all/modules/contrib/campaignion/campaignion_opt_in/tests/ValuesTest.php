<?php

namespace Drupal\campaignion_opt_in;

use Drupal\little_helpers\Webform\Submission;

require_once drupal_get_path('module', 'webform') . '/includes/webform.components.inc';

/**
 * Test the values helper class.
 */
class ValuesTest extends \DrupalUnitTestCase {

  /**
   * Generate a new opt_in component according to the parameters.
   */
  protected function getComponent($extra, $component = []) {
    $component['type'] = 'opt_in';
    $component['extra'] = $extra;
    webform_component_defaults($component);
    return $component;
  }

  /**
   * Test getting options for all possible configurations.
   */
  public function testOptionsByComponent() {
    $self = $this;
    $unprefix = function ($x) {
      return explode(':', $x, 2)[1];
    };
    $options = function ($display, $no_is_optout, $disable_optin) use ($self, $unprefix) {
      $c = $self->getComponent([
        'display' => $display,
        'no_is_optout' => $no_is_optout,
        'disable_optin' => $disable_optin,
      ]);
      return array_map($unprefix, array_keys(Values::optionsByComponent($c)));
    };
    list($i, $o, $n, $s) = ['opt-in', 'opt-out', 'no-change', 'not-selected'];
    $this->assertEqual([$i, $n, $s], $options('checkbox', FALSE, FALSE));
    $this->assertEqual([$i, $o, $s], $options('checkbox', TRUE, FALSE));
    $this->assertEqual([$n, $o, $s], $options('checkbox', TRUE, TRUE));
    $this->assertEqual([$i, $n, $s], $options('checkbox', FALSE, TRUE));
    $this->assertEqual([$i, $n, $s], $options('checkbox-inverted', FALSE, FALSE));
    $this->assertEqual([$i, $o, $s], $options('checkbox-inverted', TRUE, FALSE));
    $this->assertEqual([$n, $o, $s], $options('checkbox-inverted', TRUE, TRUE));
    $this->assertEqual([$i, $n, $s], $options('checkbox-inverted', FALSE, TRUE));
    $this->assertEqual([$i, $n, $s], $options('radios', FALSE, FALSE));
    $this->assertEqual([$i, $o, $s], $options('radios', TRUE, FALSE));
    $this->assertEqual([$i, $o, $s], $options('radios', TRUE, TRUE));
    $this->assertEqual([$i, $n, $s], $options('radios', FALSE, TRUE));
  }

  /**
   * Test getting checkbox values for all possible configurations.
   */
  public function testCheckboxValues() {
    $self = $this;
    $values = function ($display, $no_is_optout, $disable_optin) use ($self) {
      $c = $self->getComponent([
        'display' => $display,
        'no_is_optout' => $no_is_optout,
        'disable_optin' => $disable_optin,
      ]);
      return Values::checkboxValues($c);
    };
    list($i, $o, $n) = ['opt-in', 'opt-out', 'no-change'];
    $this->assertEqual([$i, $n], $values('checkbox', FALSE, FALSE));
    $this->assertEqual([$i, $o], $values('checkbox', TRUE, FALSE));
    $this->assertEqual([$n, $o], $values('checkbox', TRUE, TRUE));
    $this->assertEqual([$i, $n], $values('checkbox', FALSE, TRUE));
    $this->assertEqual([$n, $i], $values('checkbox-inverted', FALSE, FALSE));
    $this->assertEqual([$o, $i], $values('checkbox-inverted', TRUE, FALSE));
    $this->assertEqual([$o, $n], $values('checkbox-inverted', TRUE, TRUE));
    $this->assertEqual([$n, $i], $values('checkbox-inverted', FALSE, TRUE));
  }

  /**
   * Test determining whether a channel has an opt-in for a ceartain channel.
   */
  public function testCheckSubmissionOptIn() {
    $node = (object) ['webform' => []];
    $node->webform['components'] = [
      1 => $this->getComponent(['channel' => 'one'], ['cid' => 1, 'form_key' => 'one']),
      2 => $this->getComponent(['channel' => 'two'], ['cid' => 2, 'form_key' => 'two']),
      3 => $this->getComponent(['channel' => 'one'], ['cid' => 3, 'form_key' => 'three']),
    ];
    $submission = (object) ['data' => []];
    $submission->data = [
      1 => ['checkbox:opt-in'],
      2 => [],
      3 => ['radios:opt-out'],
    ];
    $submission = new Submission($node, $submission);

    $this->assertTrue(Values::submissionHasOptIn($submission, 'one'));
    $this->assertFalse(Values::submissionHasOptIn($submission, 'two'));
    $this->assertFalse(Values::submissionHasOptIn($submission, 'other'));
  }

}
