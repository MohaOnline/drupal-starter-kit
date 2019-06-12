<?php

namespace Drupal\little_helpers\Test\Webform;

use Drupal\little_helpers\Webform\Webform;

/**
 * Test creating a submission object from various form states.
 */
class FormStateTest extends \DrupalWebTestCase {
  protected $webformNode = NULL;

  /**
   * Return test metadata for simpletest.
   *
   * @return string[]
   *   Test metadata.
   */
  public static function getInfo() {
    return array(
      'name'        => t('Create a submission from a form_state.'),
      'description' => t('Test creating a Submission instance from several $form_state setups.'),
      'group'       => t('little_helpers'),
    );
  }

  /**
   * Enable dependencies, load includes and create a node stub.
   */
  public function setUp() {
    // Enable any modules required for the test. This should be an array of
    // module names.
    parent::setUp(array('little_helpers'));
    $this->nodeStub();
    module_load_include('submissions.inc', 'webform', 'includes/webform');
  }

  /**
   * Remove the node stub.
   */
  public function tearDown() {
    node_delete($this->webformNode->nid);
  }

  /**
   * Get a form_state stub that was observed after submitting the first step.
   */
  protected function formStateFirstPageProcessedStub() {
    $form_state = array(
      'values' => array(
        'details' => array(
          'nid' => $this->webformNode->nid,
          'sid' => NULL,
          'uid' => '1',
          'page_num' => 1,
          'page_count' => 3,
          'finished' => 0,
        ),
        'submitted' => array(
          1 => 'Myfirstname',
          3 => 'myemail@address.at',
          15 => '01/1234568',
        ),
        'op' => 'Next',
      ),
      'webform' => array(
        'component_tree' => array(
          'children' => $this->webformNode->webform['components'],
        ),
        'page_num' => 1,
        'page_count' => 3,
      ),
      'clicked_button' => array(
        '#parents' => array(
          0 => 'next',
        ),
      ),
    );

    return $form_state;
  }

  /**
   * Get a form_state stub that was observed after submitting the second step.
   */
  protected function formStateSecondPageProcessedStub() {
    $form_state = array(
      'values' => array(
        'details' => array(
          'nid' => $this->webformNode->nid,
          'sid' => NULL,
          'uid' => '1',
          'page_num' => 2,
          'page_count' => 3,
          'finished' => 0,
        ),
        'op' => 'Next',
        'submitted' => array(
          1 => 'Myfirstname',
          3 => 'myemail@address.at',
          15 => '01/1234568',
          7 => 'Page break',
          18 => '987654321',
          14 => 'Mylastname',
          13 => 'some text for the textfield',
        ),
      ),
      'webform' => array(
        'component_tree' => array(
          'children' => $this->webformNode->webform['components'],
        ),
        'page_num' => 1,
        'page_count' => 3,
      ),
      'clicked_button' => array(
        '#parents' => array(
          0 => 'next',
        ),
      ),
    );

    return $form_state;
  }

  /**
   * Create a webform stub node.
   */
  protected function nodeStub() {
    $settings = array(
      'type' => 'webform',
      'language'  => LANGUAGE_NONE,
      'uid' => '1',
      'status' => '1',
      'promote' => '1',
      'moderate' => '0',
      'sticky' => '0',
      'tnid' => '0',
      'translate' => '0',
      'title' => 'FormState class unit test',
      'body' => array(LANGUAGE_NONE => array(array('value' => 'Donec placerat. Nullam nibh dolor, blandit sed, fermentum id, imperdiet sit amet, neque. Nam mollis ultrices justo. Sed tempor. Sed vitae tellus. Etiam sem arcu, eleifend sit amet, gravida eget, porta at, wisi. Nam non lacus vitae ipsum viverra pretium. Phasellus massa. Fusce magna sem, gravida in, feugiat ac, molestie eget, wisi. Fusce consectetuer luctus ipsum. Vestibulum nunc. Suspendisse dignissim adipiscing libero. Integer leo. Sed pharetra ligula a dui. Quisque ipsum nibh, ullamcorper eget, pulvinar sed, posuere vitae, nulla. Sed varius nibh ut lacus. Curabitur fringilla. Nunc est ipsum, pretium quis, dapibus sed, varius non, lectus. Proin a quam. Praesent lacinia, eros quis aliquam porttitor, urna lacus volutpat urna, ut fermentum neque mi egestas dolor.'))),
      'teaser' => array(LANGUAGE_NONE => array(array('value' => 'Donec placerat. Nullam nibh dolor, blandit sed, fermentum id, imperdiet sit amet, neque. Nam mollis ultrices justo. Sed tempor. Sed vitae tellus. Etiam sem arcu, eleifend sit amet, gravida eget, porta at, wisi. Nam non lacus vitae ipsum viverra pretium. Phasellus massa. Fusce magna sem, gravida in, feugiat ac, molestie eget, wisi. Fusce consectetuer luctus ipsum. Vestibulum nunc. Suspendisse dignissim adipiscing libero. Integer leo. Sed pharetra ligula a dui. Quisque ipsum nibh, ullamcorper eget, pulvinar sed, posuere vitae, nulla. Sed varius nibh ut lacus. Curabitur fringilla.'))),
      'log' => '',
      'format' => '1',
      'webform' => array(
        'confirmation' => 'Thanks!',
        'confirmation_format' => filter_default_format(),
        'redirect_url' => '<confirmation>',
        'teaser' => '0',
        'allow_draft' => '1',
        'submit_text' => '',
        'submit_limit' => '-1',
        'submit_interval' => '-1',
        'submit_notice' => '1',
        'roles' => array('1', '2'),
        'components' => array(),
        'emails' => array(),
        'preview' => FALSE,
      ),
    );
    $settings['webform']['components'] = array(
      6 => array(
        'cid' => '6',
        'pid' => '0',
        'form_key' => 'first_test_fieldset',
        'name' => 'First Test Fieldset',
        'type' => 'fieldset',
        'value' => '',
        'extra' => array(
          'title_display' => 0,
          'private' => 0,
          'collapsible' => 0,
          'collapsed' => 0,
          'conditional_operator' => '=',
          'exclude_cv' => 0,
          'line_items' => NULL,
          'description' => '',
          'conditional_component' => '',
          'conditional_values' => '',
        ),
        'mandatory' => '0',
        'weight' => '0',
        'page_num' => 1,
      ),
      1 => array(
        'cid' => '1',
        'pid' => '6',
        'form_key' => 'first_name',
        'name' => 'First name',
        'type' => 'textfield',
        'value' => '%get[p3]',
        'extra' => array(
          'width' => '',
          'maxlength' => '',
          'field_prefix' => '',
          'field_suffix' => '',
          'disabled' => 0,
          'unique' => FALSE,
          'title_display' => 'before',
          'description' => '',
          'attributes' => array(),
          'private' => 0,
          'conditional_component' => '',
          'conditional_operator' => '=',
          'conditional_values' => '',
          'line_items' => NULL,
        ),
        'mandatory' => '1',
        'weight' => '0',
        'page_num' => 1,
      ),
      3 => array(
        'cid' => '3',
        'pid' => '6',
        'form_key' => 'email',
        'name' => 'Email address',
        'type' => 'email',
        'value' => '%get[p5]',
        'extra' => array(
          'width' => '',
          'unique' => TRUE,
          'disabled' => 0,
          'title_display' => 'before',
          'description' => '',
          'attributes' => array(),
          'private' => 0,
          'conditional_component' => '',
          'conditional_operator' => '=',
          'conditional_values' => '',
          'line_items' => NULL,
        ),
        'mandatory' => '1',
        'weight' => '1',
        'page_num' => 1,
      ),
      15 => array(
        'cid' => '15',
        'pid' => '0',
        'form_key' => 'phone_number',
        'name' => 'Phone number',
        'type' => 'textfield',
        'value' => '%get[p11]',
        'extra' => array(
          'width' => '',
          'maxlength' => '',
          'field_prefix' => '',
          'field_suffix' => '',
          'disabled' => 0,
          'unique' => FALSE,
          'title_display' => 'before',
          'description' => '',
          'attributes' => array(),
          'private' => 0,
          'line_items' => NULL,
          'conditional_component' => '',
          'conditional_operator' => '=',
          'conditional_values' => '',
        ),
        'mandatory' => '0',
        'weight' => '1',
        'page_num' => 1,
      ),
      7 => array(
        'cid' => '7',
        'pid' => '0',
        'form_key' => 'new_1400574048840',
        'name' => 'Page break',
        'type' => 'pagebreak',
        'value' => '',
        'extra' => array(
          'private' => FALSE,
          'next_page_label' => '',
          'prev_page_label' => '',
          'line_items' => NULL,
          'conditional_component' => '',
          'conditional_operator' => '=',
          'conditional_values' => '',
        ),
        'mandatory' => '0',
        'weight' => '2',
        'page_num' => 2,
      ),
      16 => array(
        'cid' => '16',
        'pid' => '0',
        'form_key' => 'second_test_fieldset',
        'name' => 'Second Test Fieldset',
        'type' => 'fieldset',
        'value' => '',
        'extra' => array(
          'title_display' => 0,
          'private' => 0,
          'collapsible' => 0,
          'collapsed' => 0,
          'conditional_component' => '1',
          'conditional_operator' => '=',
          'exclude_cv' => 0,
          'line_items' => NULL,
          'description' => '',
          'conditional_values' => '',
        ),
        'mandatory' => '0',
        'weight' => '3',
        'page_num' => 2,
      ),
      17 => array(
        'cid' => '17',
        'pid' => '16',
        'form_key' => 'third_test_fieldset',
        'name' => 'Third Test Fieldset',
        'type' => 'fieldset',
        'value' => '',
        'extra' => array(
          'title_display' => 0,
          'private' => 0,
          'collapsible' => 0,
          'collapsed' => 0,
          'conditional_component' => '1',
          'conditional_operator' => '=',
          'exclude_cv' => 0,
          'line_items' => NULL,
          'description' => '',
          'conditional_values' => '',
        ),
        'mandatory' => '0',
        'weight' => '0',
        'page_num' => 2,
      ),
      18 => array(
        'cid' => '18',
        'pid' => '17',
        'form_key' => 'new_1400576593706',
        'name' => 'New number',
        'type' => 'number',
        'value' => '',
        'extra' => array(
          'type' => 'textfield',
          'field_prefix' => '',
          'field_suffix' => '',
          'disabled' => 0,
          'unique' => FALSE,
          'title_display' => 'before',
          'description' => '',
          'attributes' => array(),
          'private' => 0,
          'min' => '',
          'max' => '',
          'step' => '',
          'decimals' => '',
          'point' => '.',
          'separator' => ',',
          'integer' => 0,
          'excludezero' => 0,
          'line_items' => NULL,
          'conditional_component' => '',
          'conditional_operator' => '=',
          'conditional_values' => '',
        ),
        'mandatory' => '0',
        'weight' => '0',
        'page_num' => 2,
      ),
      14 => array(
        'cid' => '14',
        'pid' => '16',
        'form_key' => 'last_name',
        'name' => 'Last name',
        'type' => 'textfield',
        'value' => '%get[p4]',
        'extra' => array(
          'width' => '',
          'maxlength' => '',
          'field_prefix' => '',
          'field_suffix' => '',
          'disabled' => 0,
          'unique' => FALSE,
          'title_display' => 'before',
          'description' => '',
          'attributes' => array(),
          'private' => 0,
          'line_items' => NULL,
          'conditional_component' => '',
          'conditional_operator' => '=',
          'conditional_values' => '',
        ),
        'mandatory' => '0',
        'weight' => '1',
        'page_num' => 2,
      ),
      13 => array(
        'cid' => '13',
        'pid' => '0',
        'form_key' => 'new_1400574602889',
        'name' => 'New textfield',
        'type' => 'textfield',
        'value' => '',
        'extra' => array(
          'width' => '',
          'maxlength' => '',
          'field_prefix' => '',
          'field_suffix' => '',
          'disabled' => 0,
          'unique' => FALSE,
          'title_display' => 'before',
          'description' => '',
          'attributes' => array(),
          'private' => 0,
          'line_items' => NULL,
          'conditional_component' => '',
          'conditional_operator' => '=',
          'conditional_values' => '',
        ),
        'mandatory' => '0',
        'weight' => '4',
        'page_num' => 2,
      ),
      10 => array(
        'cid' => '10',
        'pid' => '0',
        'form_key' => 'new_1400574093875',
        'name' => 'Page break',
        'type' => 'pagebreak',
        'value' => '',
        'extra' => array(
          'private' => FALSE,
          'next_page_label' => '',
          'prev_page_label' => '',
          'line_items' => NULL,
          'conditional_component' => '',
          'conditional_operator' => '=',
          'conditional_values' => '',
        ),
        'mandatory' => '0',
        'weight' => '5',
        'page_num' => 3,
      ),
      19 => array(
        'cid' => '19',
        'pid' => '0',
        'form_key' => 'date_of_birth',
        'name' => 'Date of birth',
        'type' => 'textfield',
        'value' => '%get[p6]',
        'extra' => array(
          'width' => '',
          'maxlength' => '',
          'field_prefix' => '',
          'field_suffix' => '',
          'disabled' => 0,
          'unique' => FALSE,
          'title_display' => 'before',
          'description' => 'Bitte folgendermaßen eintragen: 16/9/1983',
          'attributes' => array(),
          'private' => 0,
          'line_items' => NULL,
          'conditional_component' => '',
          'conditional_operator' => '=',
          'conditional_values' => '',
        ),
        'mandatory' => '0',
        'weight' => '6',
        'page_num' => 3,
      ),
    );
    $this->webformNode = $this->drupalCreateNode($settings);
  }

  /* ------------------------------ Tests ------------------------- */

  /**
   * Test reading values after the first step was submitted.
   */
  public function testFormStateFirstPageProcessed() {
    $form_state = $this->formStateFirstPageProcessedStub();
    $webform    = new Webform($this->webformNode);
    $submission = $webform->formStateToSubmission($form_state);
    $this->assertEqual('Myfirstname', $submission->valueByKey('first_name'));
    $this->assertEqual('01/1234568', $submission->valueByCid(15));
    $value_reference = array(
      1 => 'Myfirstname',
      15 => '01/1234568',
      14 => NULL,
      13 => NULL,
      19 => NULL,
    );
    $this->assertEqual($value_reference, $submission->valuesByType('textfield'));
  }

}
