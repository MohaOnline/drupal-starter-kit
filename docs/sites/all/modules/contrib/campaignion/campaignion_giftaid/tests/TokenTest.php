<?php

namespace Drupal\campaignion_giftaid;

use Upal\DrupalUnitTestCase;

/**
 * Test token replacement.
 */
class TokenTest extends DrupalUnitTestCase {

  /**
   * Test whether our tokens appear in the token info.
   */
  public function testTokenInfo() {
    $info = token_info();
    $this->assertNotEmpty($info['tokens']['submission']['amount-including-giftaid']);
  }

  /**
   * Test token replacement.
   */
  public function testReplaceToken() {
    module_load_include('components.inc', 'webform', 'includes/webform');
    module_load_include('submissions.inc', 'webform', 'includes/webform');

    $components[1] = [
      'type' => 'textfield',
      'form_key' => 'donation_amount',
    ];
    foreach ($components as $cid => &$component) {
      $component += [
        'pid' => 0,
        'cid' => $cid,
      ];
      webform_component_defaults($component);
    }
    $node = (object) ['type' => 'webform', 'nid' => NULL];
    $node->webform = ['components' => $components] + webform_node_defaults();
    node_object_prepare($node);

    module_load_include('submissions.inc', 'webform', 'includes/webform');
    $form_state['values']['submitted'][1][] = 10;
    $submission = webform_submission_create($node, $GLOBALS['user'], $form_state);

    $replaced = webform_replace_tokens('[submission:amount-including-giftaid]', $node, $submission);
    $this->assertSame('12.50', $replaced);
  }

}
