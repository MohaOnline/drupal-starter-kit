<?php

/**
 * Test rendering a overlay field collection.
 */
class AdminTitleTest extends \DrupalUnitTestCase {

  /**
   * Default public title.
   *
   * @var string
   */
  public $publicTitle = 'Public title &<>';

  /**
   * Default admin title.
   *
   * @var string
   */
  public $adminTitle = 'Internal title &<>';

  /**
   * Create a stub node for testing.
   */
  protected function nodeStub() {
    $node = (object) ['type' => 'petition', 'title' => $this->publicTitle];
    $lang = field_language('node', $node, 'field_admin_title');
    $node->field_admin_title[$lang][0]['value'] = $this->adminTitle;
    $node->field_admin_title[$lang][0]['safe_value'] = check_plain($this->adminTitle);
    return $node;
  }

  /**
   * Test titles are set and public title is used on public paths.
   */
  public function testNodeLoadPublic() {
    campaignion_node_admin_path_is_admin(FALSE);
    $node = $this->nodeStub();

    campaignion_node_admin_title_node_load([$node], 'petition');
    $this->assertEquals($node->title, $this->publicTitle);
    $this->assertObjectHasAttribute('public_title', $node);
    $this->assertEquals($node->public_title, $this->publicTitle);
    $this->assertObjectHasAttribute('admin_title', $node);
    $this->assertEquals($node->admin_title, $this->adminTitle);
  }

  /**
   * Test titles are set and admin title is used on admin paths (but not saved).
   */
  public function testNodeLoadAdmin() {
    campaignion_node_admin_path_is_admin(TRUE);
    $node = $this->nodeStub();

    campaignion_node_admin_title_node_load([$node], 'petition');
    $this->assertEquals($node->title, $this->adminTitle);
    $this->assertObjectHasAttribute('public_title', $node);
    $this->assertEquals($node->public_title, $this->publicTitle);
    $this->assertObjectHasAttribute('admin_title', $node);
    $this->assertEquals($node->admin_title, $this->adminTitle);

    campaignion_node_admin_title_node_presave($node);
    $this->assertEquals($node->title, $this->publicTitle);
  }

  /**
   * Test `path_is_admin` is set to `FALSE` for public paths.
   */
  public function testFormStatePublic() {
    campaignion_node_admin_path_is_admin(FALSE);
    $form_state = form_state_defaults();
    _campaignion_node_admin_title_process_form([], $form_state);
    $this->assertArrayHasKey('path_is_admin', $form_state);
    $this->assertFalse($form_state['path_is_admin']);
  }

  /**
   * Test `path_is_admin` is set to `TRUE` for admin paths.
   */
  public function testFormStateAdmin() {
    campaignion_node_admin_path_is_admin(TRUE);
    $form_state = form_state_defaults();
    _campaignion_node_admin_title_process_form([], $form_state);
    $this->assertArrayHasKey('path_is_admin', $form_state);
    $this->assertTrue($form_state['path_is_admin']);
  }

}
