<?php

namespace Drupal\campaignion_supporter_tags;

/**
 * Tagger tests.
 */
class TaggerTest extends \DrupalWebTestCase {

  /**
   * Set-up a parent term for testing.
   */
  public function setUp() {
    parent::setUp(['campaignion_test']);
    $this->parentTag = entity_create('taxonomy_term', [
      'name' => 'Test parent',
      'vid' => taxonomy_vocabulary_machine_name_load('supporter_tags')->vid,
    ]);
    entity_save('taxonomy_term', $this->parentTag);
  }

  /**
   * Turn field data into a sorted list of term names.
   */
  protected function termNames($field) {
    $tids = array_map(function ($i) {
      return $i['tid'];
    }, $field[LANGUAGE_NONE]);
    $terms = entity_load('taxonomy_term', $tids);
    $names = array_map(function ($t) {
      return $t->name;
    }, $terms);
    sort($names);
    return $names;
  }

  /**
   * Test that adding tags works as expected.
   */
  public function testTag() {
    $t = Tagger::byNameAndParentUuid('supporter_tags', $this->parentTag->uuid);
    $field[LANGUAGE_NONE] = [];
    $t->tag($field, ['a'], FALSE);
    $this->assertEqual([], $this->termNames($field));
    $t->tag($field, ['a'], TRUE);
    $this->assertEqual(['a'], $this->termNames($field));
    $t->tag($field, ['a', 'b']);
    $this->assertEqual(['a'], $this->termNames($field));
    // Test that no new term has been created.
    $this->assertEqual(1, count(taxonomy_get_children($this->parentTag->tid)));
    $t->tag($field, ['a', 'c'], TRUE);
    $this->assertEqual(['a', 'c'], $this->termNames($field));
  }

  /**
   * Test that tagging with an unknown parent throws an exception.
   */
  public function testUnknownParentTag() {
    $this->expectException(\InvalidArgumentException::class);
    Tagger::byNameAndParentUuid('supporter_tags', 'no-uuid');
  }

  /**
   * Delete parent term and all its children.
   */
  public function tearDown() {
    $delete = array_keys(taxonomy_get_children($this->parentTag->tid));
    $delete[] = $this->parentTag->tid;
    entity_delete_multiple('taxonomy_term', $delete);
  }

}
