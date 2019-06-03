<?php

namespace Drupal\campaignion_webform_tokens;

module_load_include('inc', 'webform', 'includes/webform.components');
module_load_include('inc', 'webform', 'includes/webform.submissions');

class TokensTest extends \DrupalUnitTestCase {

  protected function stubData() {
    return [
      'node' => (object) [
        'nid' => 42,
        'type' => 'webform',
        'status' => 1,
        'webform' => [
          'components' => [
            1 => [
              'cid' => 1,
              'type' => 'email',
              'form_key' => 'email',
              'page_num' => 1,
            ] + webform_component_invoke('email', 'defaults'),
            2 => [
              'cid' => 2,
              'type' => 'textfield',
              'form_key' => 'name',
              'page_num' => 1,
            ] + webform_component_invoke('textfield', 'defaults'),
          ],
          'conditionals' => [],
        ],
      ],
      'webform-submission' => (object) [
        'sid' => 42,
        'nid' => 42,
        'completed' => TRUE,
        'data' => [
          1 => ['test@example.com'],
        ],
      ],
    ];
  }

  public function testTokenEmailField() {
    $data = $this->stubData();
    // Replace webform4-style token.
    $this->assertEqual('test@example.com', token_replace('[submission:values:email]', $data));
    // Replace text-val token.
    $this->assertEqual('test@example.com', token_replace('[submission:text-val:email]', $data));
    // Default is not replaced if component doesn't exist.
    $this->assertEqual('[submission:text-val:none/default]', token_replace('[submission:text-val:none/default]', $data));
    // Default is replaced if there is no value.
    $this->assertEqual('None', token_replace('[submission:text-val:name/None]', $data));
  }

  public function testHashToken() {
    $data = $this->stubData();
    $this->assertRegExp('/[a-zA-Z0-9\-_]{42}/', token_replace('[submission:token-hash]', $data));
  }

}

