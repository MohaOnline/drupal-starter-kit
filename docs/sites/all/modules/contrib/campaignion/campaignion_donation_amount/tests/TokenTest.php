<?php

namespace Drupal\campaignion_donation_amount;

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
    $this->assertNotEmpty($info['tokens']['submission']['amount-total']);
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
    $components[2] = [
      'type' => 'textfield',
      'form_key' => 'donation_amount_second',
      'extra' => [
        'wrapper_classes' => 'donation-amount'
      ],
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

    $form_state['values']['submitted'][1][] = 10;
    $form_state['values']['submitted'][2][] = 5;
    $submission_1 = webform_submission_create($node, $GLOBALS['user'], $form_state);

    $replaced_1 = webform_replace_tokens('[submission:amount-total]', $node, $submission_1);
    $this->assertSame('15', $replaced_1);

    /**
     * Test token replacement when one donation amount field is empty.
     */
    unset($form_state['values']['submitted'][2]);
    $submission_2 = webform_submission_create($node, $GLOBALS['user'], $form_state);

    $replaced_2 = webform_replace_tokens('[submission:amount-total]', $node, $submission_2);
    $this->assertSame('10', $replaced_2);
  }
}
