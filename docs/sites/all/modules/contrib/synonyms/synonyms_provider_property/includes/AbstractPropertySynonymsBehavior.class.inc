<?php

/**
 * @file
 * Abstract class for enabling entity properties to be source of synonyms.
 */

/**
 * Definition of AbstractPropertySynonymsBehavior class.
 */
class AbstractPropertySynonymsBehavior extends AbstractSynonymsBehavior implements AutocompleteSynonymsBehavior, SelectSynonymsBehavior {

  /**
   * Name of the property on which this provider was initialized.
   *
   * @var string
   */
  protected $property;

  /**
   * Entity info of the entity type on which this provider was initialized.
   *
   * @var array
   */
  protected $entity_info;

  public function __construct($behavior_implementation) {
    parent::__construct($behavior_implementation);

    $this->property = synonyms_provider_property_name($this->behavior_implementation['provider']);
    $this->entity_info = entity_get_info($this->behavior_implementation['entity_type']);
  }

  public function extractSynonyms($entity, $langcode = NULL) {
    $synonyms = array();

    if (isset($entity->{$this->property}) && $entity->{$this->property}) {
      $synonyms[] = $entity->{$this->property};
    }
    return $synonyms;
  }

  public function mergeEntityAsSynonym($trunk_entity, $synonym_entity, $synonym_entity_type) {
    // TODO: what to do ???
  }

  public function synonymsFind(QueryConditionInterface $condition) {
    $query = db_select($this->entity_info['base table'], 'base');
    $query->addField('base', $this->entity_info['entity keys']['id'], 'entity_id');
    $query->addField('base', $this->property, 'synonym');

    $this->synonymsFindProcessCondition($condition, 'base.' . $this->property, 'base.' . $this->entity_info['entity keys']['id']);
    $query->condition($condition);
    return $query->execute();
  }
}
