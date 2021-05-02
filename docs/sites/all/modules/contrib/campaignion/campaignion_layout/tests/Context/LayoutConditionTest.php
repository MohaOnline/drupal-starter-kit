<?php

namespace Drupal\campaignion_layout\Context;

use Drupal\campaignion_layout\Tests\ThemesBaseTest;

/**
 * Test for the layout context condition implementation.
 */
class LayoutConditionTest extends ThemesBaseTest {

  /**
   * Test the available form options for the context condition plugin.
   */
  public function testFormOptions() {
    $themes['foo']['title'] = 'Foo';
    $themes['foo']['layouts']['1col']['title'] = 'Single column';
    $themes['bar']['title'] = 'Bar';
    $themes['bar']['layouts']['2col']['title'] = 'Two columns';
    $this->injectThemes($themes);
    $condition = new LayoutCondition('plugin', []);
    $this->assertEqual([
      '1col' => 'Single column',
      '2col' => 'Two columns',
    ], $condition->condition_values());
  }

  /**
   * Test the behaviour of the execute function.
   */
  public function testExecuteWithMultipleContexts() {
    $mock_condition = $this->getMockBuilder(LayoutCondition::class)
      ->disableOriginalConstructor()
      ->setMethods(['condition_met', 'get_contexts'])
      ->getMock();
    $active_layout = 'active_layout';
    $matching_contexts = ['one', 'two', 'three'];
    $mock_condition->expects($this->once())
      ->method('get_contexts')
      ->with($active_layout)
      ->willReturn($matching_contexts);
    $mock_condition->expects($this->exactly(3))
      ->method('condition_met')
      ->withConsecutive(
        ['one', $active_layout],
        ['two', $active_layout],
        ['three', $active_layout]
      );
    $mock_condition->execute($active_layout);
  }

  /**
   * Test the behaviour of the execute function.
   */
  public function testExecuteWithNoContexts() {
    $mock_condition = $this->getMockBuilder(LayoutCondition::class)
      ->disableOriginalConstructor()
      ->setMethods(['condition_met', 'get_contexts'])
      ->getMock();
    $theme = 'theme_name';
    $matching_contexts = [];
    $mock_condition->expects($this->once())
      ->method('get_contexts')
      ->with('theme_name')
      ->willReturn($matching_contexts);
    $mock_condition->expects($this->exactly(0))
      ->method('condition_met');
    $mock_condition->execute($theme);
  }

}
