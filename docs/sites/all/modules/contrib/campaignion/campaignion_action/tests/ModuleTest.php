<?php

namespace Drupal\campaignion_action;

use Drupal\campaignion_action\Redirects\Redirect;

/**
 * Test our hook implementations.
 */
class ModuleTest extends \DrupalUnitTestCase {

  /**
   * Prepare test data.
   */
  public function setUp() {
    parent::setUp();
    $this->node = (object) ['type' => 'petition'];
    node_object_prepare($this->node);
    $this->node->webform['components'] = [];
    node_save($this->node);
  }

  /**
   * Delete test data.
   */
  public function tearDown() {
    node_delete($this->node->nid);
    db_delete('campaignion_action_redirect');
    db_delete('campaignion_action_redirect_filter');
    parent::tearDown();
  }

  /**
   * Test custom redirect various scenarios for custom redirects.
   */
  public function testCustomRedirect() {
    $stub_s['data'][1][0] = 'foo bar';
    $form['#node'] = $this->node;
    $form_state['webform_completed'] = TRUE;
    $form_state['redirect'] = 'unchanged';
    $form_state['values']['details']['sid'] = 4711;
    $cache = &drupal_static('webform_get_submission', []);
    $cache[4711] = (object) $stub_s;

    // No redirect configured.
    _campaignion_action_custom_redirect($form, $form_state);
    $this->assertEquals('unchanged', $form_state['redirect']);

    // Redirect configured but field value not set.
    $redirect = [
      'nid' => $this->node->nid,
      'delta' => 1,
      'destination' => 'not-default',
    ];
    (new Redirect($redirect))->save();

    _campaignion_action_custom_redirect($form, $form_state);
    $this->assertEquals('unchanged', $form_state['redirect']);

    // Redirect configured with field settings.
    $this->node->field_thank_you_pages[LANGUAGE_NONE][1] = [
      'type' => 'redirect',
      'node_reference_nid' => 42,
    ];
    _campaignion_action_custom_redirect($form, $form_state);
    $o = ['query' => [], 'fragment' => ''];
    $this->assertEquals(['not-default', $o], $form_state['redirect']);

    // Redirect to node.
    $this->node->field_thank_you_pages[LANGUAGE_NONE][1] = [
      'type' => 'node',
      'node_reference_nid' => 42,
    ];
    _campaignion_action_custom_redirect($form, $form_state);
    $this->assertEquals(['node/42', $o], $form_state['redirect']);

  }

  /**
   * Test redirect alter function.
   */
  public function testRedirectAlter() {
    $stub_s['sid'] = 1;
    $stub_s['data'][1][0] = 'foo bar';
    $form['#node'] = $this->node;
    $redirect = ['path' => 'unchanged'];
    $submission = (object) $stub_s;

    // No redirect configured.
    campaignion_action_webform_confirm_email_confirmation_redirect_alter($redirect, $this->node, $submission);
    $this->assertEquals('unchanged', $redirect['path']);

    // Redirects configured but field value not set.
    (new Redirect([
      'nid' => $this->node->nid,
      'delta' => 1,
      'destination' => 'optin',
      'filters' => [[
        'type' => 'opt-in',
        'value' => TRUE,
      ]],
      'weight' => 1,
    ]))->save();
    (new Redirect([
      'nid' => $this->node->nid,
      'delta' => 1,
      'destination' => 'not-default',
      'weight' => 2,
    ]))->save();

    campaignion_action_webform_confirm_email_confirmation_redirect_alter($redirect, $this->node, $submission);
    $this->assertEquals('unchanged', $redirect['path']);

    // Redirect configured with field settings.
    $this->node->field_thank_you_pages[LANGUAGE_NONE][1] = [
      'type' => 'redirect',
      'node_reference_nid' => 42,
    ];
    campaignion_action_webform_confirm_email_confirmation_redirect_alter($redirect, $this->node, $submission);
    $this->assertEquals('not-default', $redirect['path']);

    // Redirect to node.
    $this->node->field_thank_you_pages[LANGUAGE_NONE][1] = [
      'type' => 'node',
      'node_reference_nid' => 42,
    ];
    campaignion_action_webform_confirm_email_confirmation_redirect_alter($redirect, $this->node, $submission);
    $this->assertEquals('node/42', $redirect['path']);

  }

}
