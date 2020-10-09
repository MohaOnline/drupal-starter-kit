<?php

/**
 * @file
 * Documentation for Synonyms module.
 */

/**
 * Collect info about available synonyms behavior implementations.
 *
 * If your module ships a synonyms behavior implementation you probably want to
 * implement this hook. However, exercise caution, if your synonyms behavior
 * implementation is a field-based one, you might be better off implementing
 * hook_synonyms_field_behavior_implementation_info().
 *
 * @param string $entity_type
 *   Entity type whose synonyms behavior implementations are requested
 * @param string $bundle
 *   Bundle name whose synonyms behavior implementations are requested
 * @param string $behavior
 *   Behavior name whose implementations are requested
 *
 * @return array
 *   Array of information about synonyms behavior implementations your module
 *   exposes. Each sub array will represent a single synonyms behavior
 *   implementation and should have the following structure:
 *   - provider: (string) machine name of your synonyms behavior implementation.
 *     Prefix it with your module name to make sure no name collision happens.
 *     Also, provider must be unique within the namespace of behavior, entity
 *     type and bundle. Basically, this is what distinguishes one behavior
 *     implementation from another
 *   - label: (string) Human friendly translated name of your synonyms behavior
 *     implementation
 *   - class: (string) Name of PHP class that implements synonyms behavior
 *     interface, which is stated in synonyms behavior definition. This class
 *     will do all the synonyms work. This hook serves pure declarative function
 *     to map entity types, bundles with their synonym behavior implementations
 *     whereas real "synonyms-related" work is implemented in your class
 */
function hook_synonyms_behavior_implementation_info($entity_type, $bundle, $behavior) {
  $providers = array();

  switch ($entity_type) {
    case 'entity_type_i_want':
      switch ($bundle) {
        case 'bundle_i_want':
          switch ($behavior) {
            case 'behavior_i_want':
              $providers[] = array(
                'provider' => 'my_module_synonyms_behavior_implementation_machine_name',
                'label' => t('This is human friendly name of my synonyms behavior implementation. Put something meaningful here'),
                'class' => 'MySynonymsSynonymsBehavior',
              );
              break;
          }
          break;
      }

      break;
  }

  return $providers;
}

/**
 * Example of synonyms behavior implementation class.
 *
 * You are encouraged to extend AbstractSynonymsBehavior class as that one
 * contains a few heuristic that make your implementation easier.
 */
class MySynonymsSynonymsBehavior extends AbstractSynonymsBehavior implements AutocompleteSynonymsBehavior {

  /**
   * Extract synonyms from an entity within a specific behavior implementation.
   *
   * @param object $entity
   *   Entity from which to extract synonyms
   * @param string $langcode
   *   Language code for which to extract synonyms from the entity, if one is
   *   known
   *
   * @return array
   *   Array of synonyms extracted from $entity
   */
  public function extractSynonyms($entity, $langcode = NULL) {
    $synonyms = array();

    // Do something with $entity in order to extract synonyms from it. Add all
    // those synonyms into your $synonyms array.

    return $synonyms;
  }

  /**
   * Add an entity as a synonym into another entity.
   *
   * Basically this method should be called when you want to add some entity
   * as a synonym to another entity (for example when you merge one entity
   * into another and besides merging want to add synonym of the merged entity
   * into the trunk entity). You should update $trunk_entity in such a way that
   * it holds $synonym_entity as a synonym (it all depends on how data is stored
   * in your behavior implementation, but probably you will store entity label
   * or its ID as you cannot literaly store an entity inside of another entity).
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
  public function mergeEntityAsSynonym($trunk_entity, $synonym_entity, $synonym_entity_type) {
    // If you can add $synonym_entity into $trunk_entity, then do so.
    // For example:
    $trunk_entity->synonym_storage[] = $synonym_entity;
  }

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
  public function synonymsFind(QueryConditionInterface $condition) {
    // Here, as an example, we'll query an imaginary table where your module
    // supposedly keeps synonyms. We'll also use helpful
    // AbstractSynonymsBehavior::synonymsFindProcessCondition() to normalize
    // $condition argument.
    $query = db_select('my_synonyms_storage_table', 'table');
    $query->addField('table', 'entity_id', 'entity_id');
    $query->addField('table', 'synonym', 'synonym');
    $this->synonymsFindProcessCondition($condition, 'table.synonym', 'table.entity_id');
    $query->condition($condition);
    return $query->execute();
  }

  /**
   * Collect info on features pipe during invocation of hook_features_export().
   *
   * If your synonyms provider depends on some other features components, this
   * method should return them.
   *
   * @return array
   *   Array of features pipe as per hook_features_export() specification
   */
  public function featuresExportPipe() {
    $pipe = parent::featuresExportPipe();
    // Here you can add any additional features components your provider
    // depends on.
    return $pipe;
  }
}
