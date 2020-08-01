<?php

namespace Drupal\campaignion_newsletters;

use \Drupal\little_helpers\Webform\Submission;

class FormSubmissionTest extends \DrupalUnitTestCase {

  public function testSaveAndLoadWithOptinInfo() {
    $mock_node = (object) [
      'webform' => [
        'components' => [
          '11' => [
            'cid' => '11',
            'form_key' => 'one',
            'pid' => 0,
          ],
          '12' => [
            'cid' => '12',
            'form_key' => 'two',
            'pid' => '11',
          ],
        ],
      ],
    ];
    $s['nid'] = 1;
    $s['data'] = [
      11 => [NULL],
      12 => ['test'],
    ];
    $s['remote_addr'] = '127.0.0.1';
    $s['submitted'] = 4711;
    $m = new Submission($mock_node, (object) $s);
    $fs = FormSubmission::fromWebformSubmission($m);
    $this->assertEquals('127.0.0.1', $fs->ip);
    $this->assertEquals(4711, $fs->date);
    $this->assertEquals(['submitted[one][two]' => 'test'], $fs->data);
  }

}
