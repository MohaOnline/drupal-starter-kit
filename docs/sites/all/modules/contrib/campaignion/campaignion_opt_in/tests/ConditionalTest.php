<?php

namespace Drupal\campaignion_opt_innew;

/**
 * Test the webform conditionals integration.
 */
class ConditionalTest extends \DrupalUnitTestCase {

  /**
   * Include the component file.
   */
  public function setUp() : void {
    parent::setUp();
    webform_component_include('opt_in');
  }

  /**
   * Test operator with values from the form-API radios.
   */
  public function testOperatorRadios() {
    $eq = '_webform_conditional_comparison_opt_in_equal';
    $ne = '_webform_conditional_comparison_opt_in_not_equal';
    $radios = ['extra' => ['display' => 'radios', 'no_is_optout' => FALSE]];

    // Possible input values:
    // - ['opt-in']: 'Yes' radio is selected.
    // - ['opt-out']: 'No' radio is selected and no_is_optout is set.
    // - ['no-change']: 'No' radio is selected but not no_is_optout.
    // - []: No radio is selected.
    // Input value, rule value, component.
    $this->assertTrue($eq(['opt-in'], 'radios:opt-in', $radios));
    $this->assertFalse($eq(['opt-in'], 'radios:opt-out', $radios));
    $this->assertFalse($eq(['opt-in'], 'radios:no-change', $radios));
    $this->assertFalse($eq(['opt-in'], 'radios:not-selected', $radios));
    $this->assertFalse($eq(['opt-in'], 'checkbox:opt-in', $radios));
    $this->assertFalse($eq(['opt-in'], 'checkbox:no-change', $radios));
    $this->assertFalse($eq(['opt-out'], 'radios:opt-in', $radios));
    $this->assertTrue($eq(['opt-out'], 'radios:opt-out', $radios));
    $this->assertFalse($eq(['opt-out'], 'radios:no-change', $radios));
    $this->assertFalse($eq(['opt-out'], 'radios:not-selected', $radios));
    $this->assertFalse($eq(['opt-out'], 'checkbox:opt-in', $radios));
    $this->assertFalse($eq(['opt-out'], 'checkbox:no-change', $radios));
    $this->assertFalse($eq(['no-change'], 'radios:opt-in', $radios));
    $this->assertFalse($eq(['no-change'], 'radios:opt-out', $radios));
    $this->assertTrue($eq(['no-change'], 'radios:no-change', $radios));
    $this->assertFalse($eq(['no-change'], 'radios:not-selected', $radios));
    $this->assertFalse($eq(['no-change'], 'checkbox:opt-in', $radios));
    $this->assertFalse($eq(['no-change'], 'checkbox:no-change', $radios));
    $this->assertFalse($eq([], 'radios:opt-in', $radios));
    $this->assertFalse($eq([], 'radios:opt-out', $radios));
    $this->assertFalse($eq([], 'radios:no-change', $radios));
    $this->assertTrue($eq([], 'radios:not-selected', $radios));
    $this->assertFalse($eq([], 'checkbox:opt-in', $radios));
    $this->assertFalse($eq([], 'checkbox:no-change', $radios));

    $this->assertFalse($ne(['opt-in'], 'radios:opt-in', $radios));
    $this->assertTrue($ne(['opt-in'], 'radios:opt-out', $radios));
    $this->assertTrue($ne(['opt-in'], 'radios:no-change', $radios));
    $this->assertTrue($ne(['opt-in'], 'radios:not-selected', $radios));
    $this->assertTrue($ne(['opt-in'], 'checkbox:opt-in', $radios));
    $this->assertTrue($ne(['opt-in'], 'checkbox:no-change', $radios));
    $this->assertTrue($ne(['opt-out'], 'radios:opt-in', $radios));
    $this->assertFalse($ne(['opt-out'], 'radios:opt-out', $radios));
    $this->assertTrue($ne(['opt-out'], 'radios:no-change', $radios));
    $this->assertTrue($ne(['opt-out'], 'radios:not-selected', $radios));
    $this->assertTrue($ne(['opt-out'], 'checkbox:opt-in', $radios));
    $this->assertTrue($ne(['opt-out'], 'checkbox:no-change', $radios));
    $this->assertTrue($ne(['no-change'], 'radios:opt-in', $radios));
    $this->assertTrue($ne(['no-change'], 'radios:opt-out', $radios));
    $this->assertFalse($ne(['no-change'], 'radios:no-change', $radios));
    $this->assertTrue($ne(['no-change'], 'radios:not-selected', $radios));
    $this->assertTrue($ne(['no-change'], 'checkbox:opt-in', $radios));
    $this->assertTrue($ne(['no-change'], 'checkbox:no-change', $radios));
    $this->assertTrue($ne([], 'radios:opt-in', $radios));
    $this->assertTrue($ne([], 'radios:opt-out', $radios));
    $this->assertTrue($ne([], 'radios:no-change', $radios));
    $this->assertFalse($ne([], 'radios:not-selected', $radios));
    $this->assertTrue($ne([], 'checkbox:opt-in', $radios));
    $this->assertTrue($ne([], 'checkbox:no-change', $radios));
  }

  /**
   * Test operator with values from the form-API checkbox.
   */
  public function testOperatorCheckbox() {
    $eq = '_webform_conditional_comparison_opt_in_equal';
    $ne = '_webform_conditional_comparison_opt_in_not_equal';
    $checkbox = ['extra' => ['display' => 'checkbox', 'no_is_optout' => FALSE]];

    $this->assertTrue($eq(['opt-in' => 'opt-in'], 'checkbox:opt-in', $checkbox));
    $this->assertFalse($eq(['opt-in' => 'opt-in'], 'checkbox:no-change', $checkbox));
    $this->assertFalse($eq(['opt-in' => 'opt-in'], 'checkbox:opt-out', $checkbox));
    $this->assertFalse($eq(['opt-in' => 'opt-in'], 'radios:opt-in', $checkbox));
    $this->assertFalse($eq(['opt-in' => 'opt-in'], 'radios:opt-out', $checkbox));
    $this->assertFalse($eq(['opt-in' => 'opt-in'], 'radios:no-change', $checkbox));
    $this->assertFalse($eq(['opt-in' => 'opt-in'], 'radios:not-selected', $checkbox));

    $this->assertFalse($eq(['opt-in' => 0], 'checkbox:opt-in', $checkbox));
    $this->assertTrue($eq(['opt-in' => 0], 'checkbox:no-change', $checkbox));
    $this->assertFalse($eq(['opt-in' => 0], 'checkbox:opt-out', $checkbox));
    $this->assertFalse($eq(['opt-in' => 0], 'radios:opt-in', $checkbox));
    $this->assertFalse($eq(['opt-in' => 0], 'radios:opt-out', $checkbox));
    $this->assertFalse($eq(['opt-in' => 0], 'radios:no-change', $checkbox));
    $this->assertFalse($eq(['opt-in' => 0], 'radios:not-selected', $checkbox));

    $this->assertFalse($ne(['opt-in' => 'opt-in'], 'checkbox:opt-in', $checkbox));
    $this->assertTrue($ne(['opt-in' => 'opt-in'], 'checkbox:no-change', $checkbox));
    $this->assertTrue($ne(['opt-in' => 'opt-in'], 'checkbox:opt-out', $checkbox));
    $this->assertTrue($ne(['opt-in' => 'opt-in'], 'radios:opt-in', $checkbox));
    $this->assertTrue($ne(['opt-in' => 'opt-in'], 'radios:opt-out', $checkbox));
    $this->assertTrue($ne(['opt-in' => 'opt-in'], 'radios:no-change', $checkbox));
    $this->assertTrue($ne(['opt-in' => 'opt-in'], 'radios:not-selected', $checkbox));

    $this->assertTrue($ne(['opt-in' => 0], 'checkbox:opt-in', $checkbox));
    $this->assertFalse($ne(['opt-in' => 0], 'checkbox:no-change', $checkbox));
    $this->assertTrue($ne(['opt-in' => 0], 'checkbox:opt-out', $checkbox));
    $this->assertTrue($ne(['opt-in' => 0], 'radios:opt-in', $checkbox));
    $this->assertTrue($ne(['opt-in' => 0], 'radios:opt-out', $checkbox));
    $this->assertTrue($ne(['opt-in' => 0], 'radios:no-change', $checkbox));
    $this->assertTrue($ne(['opt-in' => 0], 'radios:not-selected', $checkbox));
  }

  /**
   * Test operator with values from the form-API inverted-checkbox.
   */
  public function testOperatorCheckboxNoIsOptOut() {
    $eq = '_webform_conditional_comparison_opt_in_equal';
    $ne = '_webform_conditional_comparison_opt_in_not_equal';
    $checkbox = ['extra' => ['display' => 'checkbox', 'no_is_optout' => TRUE]];

    $this->assertTrue($eq(['opt-in' => 'opt-in'], 'checkbox:opt-in', $checkbox));
    $this->assertFalse($eq(['opt-in' => 'opt-in'], 'checkbox:no-change', $checkbox));
    $this->assertFalse($eq(['opt-in' => 'opt-in'], 'checkbox:opt-out', $checkbox));
    $this->assertFalse($eq(['opt-in' => 'opt-in'], 'radios:opt-in', $checkbox));
    $this->assertFalse($eq(['opt-in' => 'opt-in'], 'radios:opt-out', $checkbox));
    $this->assertFalse($eq(['opt-in' => 'opt-in'], 'radios:no-change', $checkbox));
    $this->assertFalse($eq(['opt-in' => 'opt-in'], 'radios:not-selected', $checkbox));

    $this->assertFalse($eq(['opt-in' => 0], 'checkbox:opt-in', $checkbox));
    $this->assertFalse($eq(['opt-in' => 0], 'checkbox:no-change', $checkbox));
    $this->assertTrue($eq(['opt-in' => 0], 'checkbox:opt-out', $checkbox));
    $this->assertFalse($eq(['opt-in' => 0], 'radios:opt-in', $checkbox));
    $this->assertFalse($eq(['opt-in' => 0], 'radios:opt-out', $checkbox));
    $this->assertFalse($eq(['opt-in' => 0], 'radios:no-change', $checkbox));
    $this->assertFalse($eq(['opt-in' => 0], 'radios:not-selected', $checkbox));

    $this->assertFalse($ne(['opt-in' => 'opt-in'], 'checkbox:opt-in', $checkbox));
    $this->assertTrue($ne(['opt-in' => 'opt-in'], 'checkbox:no-change', $checkbox));
    $this->assertTrue($ne(['opt-in' => 'opt-in'], 'checkbox:opt-out', $checkbox));
    $this->assertTrue($ne(['opt-in' => 'opt-in'], 'radios:opt-in', $checkbox));
    $this->assertTrue($ne(['opt-in' => 'opt-in'], 'radios:opt-out', $checkbox));
    $this->assertTrue($ne(['opt-in' => 'opt-in'], 'radios:no-change', $checkbox));
    $this->assertTrue($ne(['opt-in' => 'opt-in'], 'radios:not-selected', $checkbox));

    $this->assertTrue($ne(['opt-in' => 0], 'checkbox:opt-in', $checkbox));
    $this->assertTrue($ne(['opt-in' => 0], 'checkbox:no-change', $checkbox));
    $this->assertFalse($ne(['opt-in' => 0], 'checkbox:opt-out', $checkbox));
    $this->assertTrue($ne(['opt-in' => 0], 'radios:opt-in', $checkbox));
    $this->assertTrue($ne(['opt-in' => 0], 'radios:opt-out', $checkbox));
    $this->assertTrue($ne(['opt-in' => 0], 'radios:no-change', $checkbox));
    $this->assertTrue($ne(['opt-in' => 0], 'radios:not-selected', $checkbox));
  }

  /**
   * Test operator with stored radio values.
   */
  public function testOperatorStoredValuesRadios() {
    $eq = '_webform_conditional_comparison_opt_in_equal';
    $ne = '_webform_conditional_comparison_opt_in_not_equal';
    $radios = ['extra' => ['display' => 'radios']];

    $this->assertTrue($eq(['radios:opt-in'], 'radios:opt-in', $radios));
    $this->assertFalse($eq(['radios:opt-in'], 'radios:opt-out', $radios));
    $this->assertFalse($eq(['radios:opt-in'], 'radios:no-change', $radios));
    $this->assertFalse($eq(['radios:opt-in'], 'radios:not-selected', $radios));
    $this->assertFalse($eq(['radios:opt-in'], 'checkbox:opt-in', $radios));
    $this->assertFalse($eq(['radios:opt-in'], 'checkbox:no-change', $radios));
    $this->assertFalse($eq(['radios:opt-out'], 'radios:opt-in', $radios));
    $this->assertTrue($eq(['radios:opt-out'], 'radios:opt-out', $radios));
    $this->assertFalse($eq(['radios:opt-out'], 'radios:no-change', $radios));
    $this->assertFalse($eq(['radios:opt-out'], 'radios:not-selected', $radios));
    $this->assertFalse($eq(['radios:opt-out'], 'checkbox:opt-in', $radios));
    $this->assertFalse($eq(['radios:opt-out'], 'checkbox:no-change', $radios));
    $this->assertFalse($eq(['radios:no-change'], 'radios:opt-in', $radios));
    $this->assertFalse($eq(['radios:no-change'], 'radios:opt-out', $radios));
    $this->assertTrue($eq(['radios:no-change'], 'radios:no-change', $radios));
    $this->assertFalse($eq(['radios:no-change'], 'radios:not-selected', $radios));
    $this->assertFalse($eq(['radios:no-change'], 'checkbox:opt-in', $radios));
    $this->assertFalse($eq(['radios:no-change'], 'checkbox:no-change', $radios));
    $this->assertFalse($eq(['radios:not-selected'], 'radios:opt-in', $radios));
    $this->assertFalse($eq(['radios:not-selected'], 'radios:opt-out', $radios));
    $this->assertFalse($eq(['radios:not-selected'], 'radios:no-change', $radios));
    $this->assertTrue($eq(['radios:not-selected'], 'radios:not-selected', $radios));
    $this->assertFalse($eq(['radios:not-selected'], 'checkbox:opt-in', $radios));
    $this->assertFalse($eq(['radios:not-selected'], 'checkbox:no-change', $radios));

    $this->assertFalse($ne(['radios:opt-in'], 'radios:opt-in', $radios));
    $this->assertTrue($ne(['radios:opt-in'], 'radios:opt-out', $radios));
    $this->assertTrue($ne(['radios:opt-in'], 'radios:no-change', $radios));
    $this->assertTrue($ne(['radios:opt-in'], 'radios:not-selected', $radios));
    $this->assertTrue($ne(['radios:opt-in'], 'checkbox:opt-in', $radios));
    $this->assertTrue($ne(['radios:opt-in'], 'checkbox:no-change', $radios));
    $this->assertTrue($ne(['radios:opt-out'], 'radios:opt-in', $radios));
    $this->assertFalse($ne(['radios:opt-out'], 'radios:opt-out', $radios));
    $this->assertTrue($ne(['radios:opt-out'], 'radios:no-change', $radios));
    $this->assertTrue($ne(['radios:opt-out'], 'radios:not-selected', $radios));
    $this->assertTrue($ne(['radios:opt-out'], 'checkbox:opt-in', $radios));
    $this->assertTrue($ne(['radios:opt-out'], 'checkbox:no-change', $radios));
    $this->assertTrue($ne(['radios:no-change'], 'radios:opt-in', $radios));
    $this->assertTrue($ne(['radios:no-change'], 'radios:opt-out', $radios));
    $this->assertFalse($ne(['radios:no-change'], 'radios:no-change', $radios));
    $this->assertTrue($ne(['radios:no-change'], 'radios:not-selected', $radios));
    $this->assertTrue($ne(['radios:no-change'], 'checkbox:opt-in', $radios));
    $this->assertTrue($ne(['radios:no-change'], 'checkbox:no-change', $radios));
    $this->assertTrue($ne(['radios:not-selected'], 'radios:opt-in', $radios));
    $this->assertTrue($ne(['radios:not-selected'], 'radios:opt-out', $radios));
    $this->assertTrue($ne(['radios:not-selected'], 'radios:no-change', $radios));
    $this->assertFalse($ne(['radios:not-selected'], 'radios:not-selected', $radios));
    $this->assertTrue($ne(['radios:not-selected'], 'checkbox:opt-in', $radios));
    $this->assertTrue($ne(['radios:not-selected'], 'checkbox:no-change', $radios));
  }

  /**
   * Test operator with stored checkbox values.
   */
  public function testOperatorStoredValuesCheckbox() {
    $eq = '_webform_conditional_comparison_opt_in_equal';
    $ne = '_webform_conditional_comparison_opt_in_not_equal';
    $checkbox = ['extra' => ['display' => 'checkbox']];

    $this->assertTrue($eq(['checkbox:opt-in'], 'checkbox:opt-in', $checkbox));
    $this->assertFalse($eq(['checkbox:opt-in'], 'checkbox:no-change', $checkbox));
    $this->assertFalse($eq(['checkbox:opt-in'], 'radios:opt-in', $checkbox));
    $this->assertFalse($eq(['checkbox:opt-in'], 'radios:opt-out', $checkbox));
    $this->assertFalse($eq(['checkbox:opt-in'], 'radios:no-change', $checkbox));
    $this->assertFalse($eq(['checkbox:opt-in'], 'radios:not-selected', $checkbox));
    $this->assertFalse($eq(['checkbox:no-change'], 'checkbox:opt-in', $checkbox));
    $this->assertTrue($eq(['checkbox:no-change'], 'checkbox:no-change', $checkbox));
    $this->assertFalse($eq(['checkbox:no-change'], 'radios:opt-in', $checkbox));
    $this->assertFalse($eq(['checkbox:no-change'], 'radios:opt-out', $checkbox));
    $this->assertFalse($eq(['checkbox:no-change'], 'radios:no-change', $checkbox));
    $this->assertFalse($eq(['checkbox:no-change'], 'radios:not-selected', $checkbox));

    $this->assertFalse($ne(['checkbox:opt-in'], 'checkbox:opt-in', $checkbox));
    $this->assertTrue($ne(['checkbox:opt-in'], 'checkbox:no-change', $checkbox));
    $this->assertTrue($ne(['checkbox:opt-in'], 'radios:opt-in', $checkbox));
    $this->assertTrue($ne(['checkbox:opt-in'], 'radios:opt-out', $checkbox));
    $this->assertTrue($ne(['checkbox:opt-in'], 'radios:no-change', $checkbox));
    $this->assertTrue($ne(['checkbox:opt-in'], 'radios:not-selected', $checkbox));
    $this->assertTrue($ne(['checkbox:no-change'], 'checkbox:opt-in', $checkbox));
    $this->assertFalse($ne(['checkbox:no-change'], 'checkbox:no-change', $checkbox));
    $this->assertTrue($ne(['checkbox:no-change'], 'radios:opt-in', $checkbox));
    $this->assertTrue($ne(['checkbox:no-change'], 'radios:opt-out', $checkbox));
    $this->assertTrue($ne(['checkbox:no-change'], 'radios:no-change', $checkbox));
    $this->assertTrue($ne(['checkbox:no-change'], 'radios:not-selected', $checkbox));
  }

}
