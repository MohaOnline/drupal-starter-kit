<?php

namespace Drupal\campaignion_newsletters;

use \Drupal\little_helpers\Webform\Submission;

class FormSubmissionTest extends \DrupalUnitTestCase {
  public function testSaveAndLoad_withOptinInfo() {
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
    $m = $this->getMockBuilder('\\Drupal\\little_helpers\\Webform\\Submission')
      ->setMethods(['valueByCid'])->disableOriginalConstructor()->getMock();
    $m->node = $mock_node;
    $m->nid = 1;
    $m->remote_addr = '127.0.0.1';
    $m->submitted = 4711;
    $m->method('valueByCid')->will($this->returnValueMap([
      [11, NULL],
      [12, 'test'],
    ]));

    $fs = FormSubmission::fromWebformSubmission($m);
    $this->assertEquals('127.0.0.1', $fs->ip);
    $this->assertEquals(4711, $fs->date);
    $this->assertEquals(['submitted[one][two]' => 'test'], $fs->data);
  }

}
