<?php

namespace Drupal\campaignion_action;

use Drupal\campaignion_action\Redirects\Redirect;
use Drupal\campaignion_opt_in\Values;
use Drupal\little_helpers\System\FormRedirect;
use Drupal\little_helpers\Webform\Submission;
use Drupal\little_helpers\Webform\Webform;
use Upal\DrupalUnitTestCase;

/**
 * Test our hook implementations.
 */
class ModuleTest extends DrupalUnitTestCase {

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
  public function testRedirectAlter1() {
    $redirect = new FormRedirect(['path' => 'unchanged']);
    $submission = $this->createMock(Submission::class);
    // Setting properties needs a bit of extra work because of magic methods.
    $data['nid'] = $this->node->nid;
    $data['sid'] = 4711;
    $submission->method('__get')->will($this->returnCallback(function ($name) use ($data) {
      return isset($data[$name]) ? $data[$name] : NULL;
    }));
    $submission->node = $this->node;
    $submission->webform = new Webform($this->node);
    $submission->opt_in = new Values($submission);

    // No redirect configured.
    campaignion_action_webform_redirect_alter($redirect, $submission);
    $this->assertEquals('unchanged', $redirect->path);

    // Redirect configured but field value not set.
    (new Redirect([
      'nid' => $this->node->nid,
      'delta' => Redirect::THANK_YOU_PAGE,
      'destination' => 'not-default',
    ]))->save();

    campaignion_action_webform_redirect_alter($redirect, $submission);
    $this->assertEquals('unchanged', $redirect->path);

    // Redirect configured with field settings.
    $this->node->field_thank_you_pages[LANGUAGE_NONE][1] = [
      'type' => 'redirect',
      'node_reference_nid' => 42,
    ];
    campaignion_action_webform_redirect_alter($redirect, $submission);
    $o = [
      'query' => ['share' => "node/{$this->node->nid}", 'sid' => 4711],
      'fragment' => '',
    ];
    $this->assertEquals(['not-default', $o], $redirect->toFormStateRedirect());

    // Redirect to node.
    $this->node->field_thank_you_pages[LANGUAGE_NONE][1] = [
      'type' => 'node',
      'node_reference_nid' => 42,
    ];
    campaignion_action_webform_redirect_alter($redirect, $submission);
    $this->assertEquals(['node/42', $o], $redirect->toFormStateRedirect());

  }

  /**
   * Test redirect alter function based on field values.
   */
  public function testRedirectAlter2() {
    $redirect = new FormRedirect(['path' => 'unchanged']);
    $submission = $this->createMock(Submission::class);
    // Setting properties needs a bit of extra work because of magic methods.
    $data['nid'] = $this->node->nid;
    $data['sid'] = 4711;
    $submission->method('__get')->will($this->returnCallback(function ($name) use (&$data) {
      return isset($data[$name]) ? $data[$name] : NULL;
    }));
    $submission->node = $this->node;
    $submission->webform = new Webform($this->node);
    $data['opt_in'] = new Values($submission);

    // Redirects configured but field value not set.
    (new Redirect([
      'nid' => $this->node->nid,
      'delta' => 1,
      'destination' => 'optin',
      'filters' => [['type' => 'opt-in', 'value' => TRUE]],
      'weight' => 1,
    ]))->save();
    (new Redirect([
      'nid' => $this->node->nid,
      'delta' => 1,
      'destination' => 'not-default',
      'weight' => 2,
    ]))->save();

    campaignion_action_webform_redirect_alter($redirect, $submission);
    $this->assertEquals('unchanged', $redirect->path);

    // Redirect configured with field settings.
    $this->node->field_thank_you_pages[LANGUAGE_NONE][1] = [
      'type' => 'redirect',
      'node_reference_nid' => 42,
    ];
    campaignion_action_webform_redirect_alter($redirect, $submission);
    $this->assertEquals('not-default', $redirect->path);

    // Redirect to node.
    $this->node->field_thank_you_pages[LANGUAGE_NONE][1] = [
      'type' => 'node',
      'node_reference_nid' => 42,
    ];
    campaignion_action_webform_redirect_alter($redirect, $submission);
    $this->assertEquals('node/42', $redirect->path);
  }

}
