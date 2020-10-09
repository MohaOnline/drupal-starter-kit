<?php

/**
 * @file
 * Interfaces of synonyms behaviors that are shipped with Synonyms module.
 */

/**
 * General interface of a synonyms behavior.
 *
 * All synonyms behaviors must extend this interface.
 */
interface SynonymsBehavior {

  /**
   * Extract synonyms from an entity within a specific behavior implementation.
   *
   * @param object $entity
   *   Entity from which to extract synonyms
   * @param string $langcode
   *   Language code for which to extract synonyms from the entity, if one is
   *   known
   *
   * @return array Array of synonyms extracted from $entity
   * Array of synonyms extracted from $entity
   */
  public function extractSynonyms($entity, $langcode = NULL);

  /**
   * Add an entity as a synonym into another entity.
   *
   * Basically this method should be called when you want to add some entity as
   * a synonym to another entity (for example when you merge one entity into
   * another and besides merging want to add synonym of the merged entity into
   * the trunk entity). You should update $trunk_entity in such a way that it
   * holds $synonym_entity as a synonym (it all depends on how data is stored in
   * your behavior implementation, but probably you will store entity label or
   * its ID as you cannot literally store an entity inside of another entity).
   * If entity of type $synonym_entity_type cannot be converted into a format
   * expected by your behavior implementation, just do nothing.
   *
   * @param object $trunk_entity
   *   Entity into which another one should be added as synonym
   * @param object $synonym_entity
   *   Fully loaded entity object which has to be added as synonym
   * @param string $synonym_entity_type
   *   Entity type of $synonym_entity
   */
  public function mergeEntityAsSynonym($trunk_entity, $synonym_entity, $synonym_entity_type);

  /**
   * Look up entities by their synonyms within a behavior implementation.
   *
   * You are provided with a SQL condition that you should apply to the storage
   * of synonyms within the provided behavior implementation. And then return
   * result: what entities match by the provided condition through what
   * synonyms.
   *
   * @param QueryConditionInterface $condition
   *   Condition that defines what to search for. Apart from normal SQL
   *   conditions as known in Drupal, it may contain the following placeholders:
   *   - AbstractSynonymsBehavior::COLUMN_SYNONYM_PLACEHOLDER: to denote
   *     synonyms column which you should replace with the actual column name
   *     where the synonyms data for your provider is stored in plain text.
   *   - AbstractSynonymsBehavior::COLUMN_ENTITY_ID_PLACEHOLDER: to denote
   *     column that holds entity ID. You are supposed to replace this
   *     placeholder with actual column name that holds entity ID in your case.
   *   For ease of work with these placeholders, you may extend the
   *   AbstractSynonymsBehavior class and then just invoke the
   *   AbstractSynonymsBehavior->synonymsFindProcessCondition() method, so you
   *   won't have to worry much about it. Important note: if you plan on
   *   re-using the same $condition object for multiple invocations of this
   *   method you must pass in here a clone of your condition object, since the
   *   internal implementation of this method will change the condition (will
   *   swap the aforementioned placeholders with actual column names)
   *
   * @return Traversable
   *   Traversable result set of found synonyms and entity IDs to which those
   *   belong. Each element in the result set should be an object and will have
   *   the following structure:
   *   - synonym: (string) Synonym that was found and which satisfies the
   *     provided condition
   *   - entity_id: (int) ID of the entity to which the found synonym belongs
   */
  public function synonymsFind(QueryConditionInterface $condition);

  /**
   * Collect info on features pipe during invocation of hook_features_export().
   *
   * If your synonyms provider depends on some other features components, this
   * method should return them.
   *
   * @return array
   *   Array of features pipe as per hook_features_export() specification
   */
  public function featuresExportPipe();
}

/**
 * Exception thrown by implementations of SynonymsBehavior interface.
 */
class SynonymsBehaviorException extends Exception {}

/**
 * Starting point for implementing SynonymsBehavior interface.
 */
abstract class AbstractSynonymsBehavior implements SynonymsBehavior {

  /**
   * Constant which denotes placeholder of a synonym column.
   *
   * @var string
   */
  const COLUMN_SYNONYM_PLACEHOLDER = '***COLUMN***';

  /**
   * Constant which denotes placeholder of an entity ID column.
   *
   * @var string
   */
  const COLUMN_ENTITY_ID_PLACEHOLDER = '***ENTITY_ID***';

  /**
   * Behavior implementation on which this class was initialized.
   *
   * @var array
   */
  protected $behavior_implementation;

  public function __construct($behavior_implementation) {
    $this->behavior_implementation = $behavior_implementation;
  }

  public function featuresExportPipe() {
    return array();
  }

  /**
   * Process condition in 'synonymsFind' method.
   *
   * Process condition in 'synonymsFind' method replacing all references of
   * synonym and entity ID columns with the real names of those columns.
   *
   * @param QueryConditionInterface $condition
   *   Condition that should be processed
   * @param string $column_synonym
   *   Real name of the synonym column
   * @param string $column_entity_id
   *   Real name of the entity ID column
   */
  protected function synonymsFindProcessCondition(QueryConditionInterface $condition, $column_synonym, $column_entity_id) {
    $condition_array = &$condition->conditions();
    foreach ($condition_array as &$v) {
      if (is_array($v) && isset($v['field'])) {
        if ($v['field'] instanceof QueryConditionInterface) {
          // Recursively process this condition too.
          $this->synonymsFindProcessCondition($v['field'], $column_synonym, $column_entity_id);
        }
        else {
          $replace = array(
            self::COLUMN_SYNONYM_PLACEHOLDER => $column_synonym,
            self::COLUMN_ENTITY_ID_PLACEHOLDER => $column_entity_id,
          );
          $v['field'] = str_replace(array_keys($replace), array_values($replace), $v['field']);
        }
      }
    }
  }
}

/**
 * Interface of the autocomplete synonyms behavior.
 */
interface AutocompleteSynonymsBehavior extends SynonymsBehavior {
}

/**
 * Interface of the synonyms friendly select behavior.
 */
interface SelectSynonymsBehavior extends SynonymsBehavior {
}
