<?php

namespace Drupal\campaignion_opt_in;

/**
 * Test the webform component plugin.
 */
class ComponentTest extends \DrupalUnitTestCase {

  /**
   * Backup for $_GET values.
   *
   * @var array
   */
  protected $backupGet;

  /**
   * Backup global $_GET values.
   */
  public function setUp() {
    parent::setUp();
    $this->backupGet = $_GET;
    $_GET = ['q' => $_GET['q']];
  }

  /**
   * Restore global $_GET values.
   */
  public function tearDown() {
    $_GET = $this->backupGet;
    parent::tearDown();
  }

  /**
   * Test that the edit form works with default values.
   */
  public function testEditDefaults() {
    $node_stub = (object) ['nid' => 0, 'webform' => ['components' => []]];
    $_GET += [
      'name' => 'Opt-in Test',
      'required' => FALSE,
      'pid' => 0,
      'weight' => 0,
    ];
    $component = webform_menu_component_load('new', 0, 'opt_in');
    $form = drupal_get_form('webform_component_edit_form', $node_stub, $component);
    $this->assertEqual('Opt-in Test', $form['name']['#default_value']);
    $display_id = "#{$form['extra']['display']['#id']}";
    $states['invisible'][$display_id]['value'] = 'radios';
    $this->assertEqual($states, $form['extra']['checkbox_label']['#states']);
  }

  /**
   * Test that the render function works with the default values.
   */
  public function testRenderDefaults() {
    $component = webform_component_invoke('opt_in', 'defaults');
    $form = webform_component_invoke('opt_in', 'render', $component);
    $this->assertEqual('Opt-in', $form['#title']);
  }

  /**
   * Test rendering an inverted checkbox.
   */
  public function testRenderCheckboxInverted() {
    $component['type'] = 'opt_in';
    $component['extra'] = ['display' => 'checkbox-inverted'];
    webform_component_defaults($component);
    $form = webform_component_invoke('opt_in', 'render', $component);
    $this->assertEqual(['no-change'], array_keys($form['#options']));
  }

  /**
   * Test rendering a radio with "no is opt-out".
   */
  public function testRenderRadioNoIsOptOut() {
    $component['type'] = 'opt_in';
    $component['extra'] = ['display' => 'radios', 'no_is_optout' => TRUE];
    webform_component_defaults($component);
    $form = webform_component_invoke('opt_in', 'render', $component);
    $this->assertEqual(['opt-in', 'opt-out'], array_keys($form['#options']));
  }

  /**
   * Test normalizing input values from a checkbox.
   */
  public function testSubmitCheckbox() {
    $c['extra']['display'] = 'checkbox';

    // Not checked checkbox.
    $v['opt-in'] = 0;
    $this->assertEqual(['checkbox:no-change'], _webform_submit_opt_in($c, $v));

    // Checked checkbox.
    $v['opt-in'] = 'opt-in';
    $this->assertEqual(['checkbox:opt-in'], _webform_submit_opt_in($c, $v));

    // Private checkbox always gives the default.
    $v = ['not-selected' => 'not-selected', 'opt-in' => 0];
    $this->assertEqual(['checkbox:not-selected'], _webform_submit_opt_in($c, $v));
    $v = ['opt-out' => 'opt-out', 'opt-in' => 0];
    $this->assertEqual(['checkbox:opt-out'], _webform_submit_opt_in($c, $v));
    $v = ['no-change' => 'no-change', 'opt-in' => 0];
    $this->assertEqual(['checkbox:no-change'], _webform_submit_opt_in($c, $v));
  }

  /**
   * Test normalizing input values from an inverted checkbox.
   */
  public function testSubmitInvertedCheckbox() {
    $c['extra']['display'] = 'checkbox-inverted';

    // Not checked checkbox.
    $v['no-change'] = 0;
    $this->assertEqual(['checkbox-inverted:opt-in'], _webform_submit_opt_in($c, $v));

    // Checked checkbox.
    $v['no-change'] = 'no-change';
    $this->assertEqual(['checkbox-inverted:no-change'], _webform_submit_opt_in($c, $v));

    // Private checkbox always gives the default.
    $v = ['not-selected' => 'not-selected', 'opt-out' => 0];
    $this->assertEqual(['checkbox-inverted:not-selected'], _webform_submit_opt_in($c, $v));
    $v = ['opt-in' => 'opt-in', 'opt-out' => 0];
    $this->assertEqual(['checkbox-inverted:opt-in'], _webform_submit_opt_in($c, $v));
    $v = ['no-change' => 'no-change', 'opt-out' => 0];
    $this->assertEqual(['checkbox-inverted:no-change'], _webform_submit_opt_in($c, $v));
  }

  /**
   * Test normalizing input values from radios.
   */
  public function testSubmitRadios() {
    $c['extra']['display'] = 'radios';

    // Radio no.
    $v = 'opt-out';
    $this->assertEqual(['radios:opt-out'], _webform_submit_opt_in($c, $v));

    // Radio no change.
    $v = 'no-change';
    $this->assertEqual(['radios:no-change'], _webform_submit_opt_in($c, $v));

    // Radio yes.
    $v = 'opt-in';
    $this->assertEqual(['radios:opt-in'], _webform_submit_opt_in($c, $v));

    // Not selected radio.
    $v = NULL;
    $this->assertEqual(['radios:not-selected'], _webform_submit_opt_in($c, $v));
  }

  /**
   * Test rendering data for the table display.
   */
  public function testTable() {
    $export = function ($v) {
      return _webform_table_opt_in(NULL, $v);
    };
    $this->assertEqual(t('Unknown value'), $export(NULL));
    $this->assertEqual(t('Unknown value'), $export(['0']));
    $this->assertEqual(t('Checkbox hidden (no change)'), $export(['checkbox:not-selected']));
    $this->assertEqual(t('Checkbox opt-in'), $export(['checkbox:opt-in']));
    $this->assertEqual(t('Checkbox no change'), $export(['checkbox:no-change']));
    $this->assertEqual(t('Checkbox opt-out'), $export(['checkbox:opt-out']));
    $this->assertEqual(t('Radio opt-in'), $export(['radios:opt-in']));
    $this->assertEqual(t('Radio opt-out'), $export(['radios:opt-out']));
    $this->assertEqual(t('Radio no change'), $export(['radios:no-change']));
    $this->assertEqual(t('Radio not selected (no change)'), $export(['radios:not-selected']));
    $this->assertEqual(t('Inverted checkbox hidden (no change)'), $export(['checkbox-inverted:not-selected']));
    $this->assertEqual(t('Inverted checkbox opt-in'), $export(['checkbox-inverted:opt-in']));
    $this->assertEqual(t('Inverted checkbox no change'), $export(['checkbox-inverted:no-change']));
    $this->assertEqual(t('Inverted checkbox opt-out'), $export(['checkbox-inverted:opt-out']));
    $this->assertEqual(t('Private or hidden by conditionals (no change)'), $export(['']));
  }

  /**
   * Test rendering data for CSV output.
   */
  public function testCsvData() {
    $export = function ($v) {
      return _webform_csv_data_opt_in(NULL, [], $v);
    };
    $this->assertEqual(t('Unknown value'), $export(NULL));
    $this->assertEqual(t('Unknown value'), $export(['0']));
    $this->assertEqual(t('Checkbox hidden (no change)'), $export(['checkbox:not-selected']));
    $this->assertEqual(t('Checkbox opt-in'), $export(['checkbox:opt-in']));
    $this->assertEqual(t('Checkbox no change'), $export(['checkbox:no-change']));
    $this->assertEqual(t('Checkbox opt-out'), $export(['checkbox:opt-out']));
    $this->assertEqual(t('Radio opt-in'), $export(['radios:opt-in']));
    $this->assertEqual(t('Radio opt-out'), $export(['radios:opt-out']));
    $this->assertEqual(t('Radio no change'), $export(['radios:no-change']));
    $this->assertEqual(t('Radio not selected (no change)'), $export(['radios:not-selected']));
    $this->assertEqual(t('Inverted checkbox hidden (no change)'), $export(['checkbox-inverted:not-selected']));
    $this->assertEqual(t('Inverted checkbox opt-in'), $export(['checkbox-inverted:opt-in']));
    $this->assertEqual(t('Inverted checkbox no change'), $export(['checkbox-inverted:no-change']));
    $this->assertEqual(t('Inverted checkbox opt-out'), $export(['checkbox-inverted:opt-out']));
    $this->assertEqual(t('Private or hidden by conditionals (no change)'), $export(['']));
  }

  /**
   * Get the conditional form callback.
   */
  protected function getConditionalFormCallback() {
    $info = campaignion_opt_in_webform_conditional_operator_info();
    return $info['opt_in']['equal']['form callback'];
  }

  /**
   * Test conditional options with radios.
   */
  public function testConditionalOptionsRadios() {
    $fake_node['webform']['components'][1] = [
      'type' => 'opt_in',
      'extra' => [
        'display' => 'radios',
        'channel' => 'email',
        'no_is_optout' => FALSE,
      ],
    ];
    $fake_node = (object) $fake_node;
    $form_callback = $this->getConditionalFormCallback();
    $forms = $form_callback($fake_node);
    $expected_select = '<select class="form-select"><option value="radios:opt-in">Radio opt-in</option><option value="radios:no-change">Radio no change</option><option value="radios:not-selected">Radio not selected (no change)</option></select>';
    $this->assertContains($expected_select, $forms[1]);
  }

}
