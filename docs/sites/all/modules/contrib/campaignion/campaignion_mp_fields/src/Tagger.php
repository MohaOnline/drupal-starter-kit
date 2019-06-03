<?php

namespace Drupal\campaignion_mp_fields;

/**
 * Allows conveniently adding terms to taxonomy term reference fields.
 *
 * Each instance of the class is configured for using tags of a specific
 * vocabulary and (optionally) a parent term.
 */
class Tagger {

  /**
   * Static cache for instances of this class.
   *
   * @var static[]
   */
  protected static $instances = [];

  /**
   * Create or get a new Tagger instance.
   *
   * @param string $vocabulary_name
   *   Vocabulary machine name that should be used for tagging.
   * @param string|null $parent_uuid
   *   UUID of the taxonomy term in the vocabulary that should be the parent tag
   *   of newly created terms.
   * @param bool $reset
   *   Forces creation of a new instance if TRUE.
   *
   * @return static
   */
  public static function byNameAndParentUuid($vocabulary_name, $parent_uuid = NULL, $reset = FALSE) {
    if (!isset(static::$instances[$vocabulary_name][$parent_uuid]) || $reset) {
      $ptid = 0;
      if ($parent_uuid) {
        $ids = entity_get_id_by_uuid('taxonomy_term', [$parent_uuid]);
        $ptid = reset($ids);
      }
      $vid = taxonomy_vocabulary_machine_name_load($vocabulary_name)->vid;
      static::$instances[$vocabulary_name][$parent_uuid] = new static($vid, $ptid);
    }
    return static::$instances[$vocabulary_name][$parent_uuid];
  }

  /**
   * Map of tag names to their tids.
   *
   * @var int[]
   */
  protected $map;

  /**
   * The vocabulary id used by this class.
   *
   * @var int
   */
  protected $vid;

  /**
   * The parent term tid used by this class.
   *
   * Only tags that are children of this term are used and new tags are created
   * as children of this term.
   *
   * @var int
   */
  protected $parentTid;

  /**
   * Create new instance.
   *
   * @param int $vid
   *   Taxonomy vocabulary id.
   * @param int $parent_tid
   *   Parent taxonomy term id.
   */
  public function __construct($vid, $parent_tid) {
    $this->map = [];
    $sql = 'SELECT tid, name FROM {taxonomy_term_data} INNER JOIN {taxonomy_term_hierarchy} USING(tid) WHERE vid=:vid AND parent=:parent';
    $result = db_query($sql, [
      ':vid' => $vid,
      ':parent' => $parent_tid,
    ]);
    foreach ($result as $row) {
      $this->map[$row->name] = $row->tid;
    }
    $this->vid = $vid;
    $this->parentTid = $parent_tid;
  }

  /**
   * Map tag to it’s tid and optionally create it if it doesn’t exist.
   *
   * @param string $tag
   *   Name of the tag that should be mapped or created.
   * @param bool $add
   *   If TRUE tags that are not found will be created.
   *
   * @return int|null
   *   The taxonomy term id of the tag with this name or NULL if none was found
   *   and $add is FALSE.
   */
  protected function mapTag($tag, $add) {
    if (!isset($this->map[$tag])) {
      if ($add) {
        $term = entity_create('taxonomy_term', [
          'name' => $tag,
          'vid' => $this->vid,
          'parent' => $this->parentTid,
        ]);
        entity_save('taxonomy_term', $term);
        $this->map[$tag] = $term->tid;
      }
      else {
        return;
      }
    }
    return $this->map[$tag];
  }

  /**
   * Add tags to a multi-value taxonomy term reference field.
   *
   * @param \EntityListWrapper $field
   *   Entity metadata wrapper of the field.
   * @param string[] $tags
   *   List of tag names to add to the $field.
   * @param bool $add
   *   Whether or not to create not yet existing tags.
   *
   * @return bool
   *   TRUE if the field values were changed otherwise FALSE.
   */
  public function tag(\EntityListWrapper $field, array $tags, $add = FALSE) {
    $changed = FALSE;

    $items = [];
    foreach ($field->value() as $term) {
      $items[$term->tid] = $term->tid;
    }

    foreach ($tags as $tag) {
      if ($tid = $this->mapTag($tag, $add)) {
        if (!isset($items[$tid])) {
          $changed = TRUE;
          $items[$tid] = $tid;
        }
      }
    }

    if ($changed) {
      $field->set(array_keys($items));
    }
    return $changed;
  }

  /**
   * Set the value of a single-value taxonomy term reference field.
   *
   * @param \EntityStructureWrapper $field
   *   Entity metadata wrapper of the field.
   * @param string $tag
   *   Name of the taxonomy term.
   * @param bool $add
   *   Whether or not to create not yet existing tags.
   *
   * @return bool
   *   TRUE if the field value was changed otherwise FALSE.
   */
  public function tagSingle(\EntityStructureWrapper $field, $tag, $add = FALSE) {
    $changed = FALSE;

    $tid = NULL;
    if ($term = $field->value()) {
      $tid = $term->tid;
    }

    if ($new_tid = $this->mapTag($tag, $add)) {
      if ($new_tid != $tid) {
        $changed = TRUE;
        $field->set($new_tid);
      }
    }
    return $changed;
  }

}
