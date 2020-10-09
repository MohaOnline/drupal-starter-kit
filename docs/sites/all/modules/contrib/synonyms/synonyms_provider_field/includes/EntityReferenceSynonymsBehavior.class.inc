<?php

/**
 * @file
 * Enables Entity Reference field type to be source of synonyms.
 */

/**
 * Definition of EntityReferenceSynonymsBehavior class.
 */
class EntityReferenceSynonymsBehavior extends AbstractFieldSynonymsBehavior implements AutocompleteSynonymsBehavior, SelectSynonymsBehavior {

  public function extractSynonyms($entity, $langcode = NULL) {
    $synonyms = array();

    $target_tids = array();
    foreach ($this->entityItems($entity, $langcode) as $item) {
      $target_tids[] = $item['target_id'];
    }
    $entities = entity_load($this->field['settings']['target_type'], $target_tids);
    foreach ($entities as $entity) {
      $synonyms[] = entity_label($this->field['settings']['target_type'], $entity);
    }

    return $synonyms;
  }

  public function mergeEntityAsSynonym($trunk_entity, $synonym_entity, $synonym_entity_type) {
    // Firstly validating that this entity reference is able to reference to
    // that type of entity.
    $expected_synonym_entity_type = $this->field['settings']['target_type'];

    $items = $this->entityItems($trunk_entity);

    if ($expected_synonym_entity_type != $synonym_entity_type) {
      return;
    }
    $synonym_entity_id = entity_extract_ids($synonym_entity_type, $synonym_entity);
    $synonym_entity_id = $synonym_entity_id[0];
    $items[] = array(
      'target_id' => $synonym_entity_id,
    );
    $trunk_entity->{$this->field['field_name']}[LANGUAGE_NONE] = $this->uniqueItems($items, array('target_id'));
  }

  public function synonymsFind(QueryConditionInterface $condition) {
    if ($this->field['storage']['type'] != 'field_sql_storage') {
      throw new SynonymsBehaviorException(t('Not supported storage engine %type in @method() method.', array(
        '%type' => $this->field['storage']['type'],
        '@method' => __METHOD__,
      )));
    }
    $table = array_keys($this->field['storage']['details']['sql'][FIELD_LOAD_CURRENT]);
    $table = reset($table);
    $column = $this->field['storage']['details']['sql'][FIELD_LOAD_CURRENT][$table]['target_id'];

    $query = db_select($table, 'field');

    $target_entity_type_info = entity_get_info($this->field['settings']['target_type']);
    if (!isset($target_entity_type_info['base table']) || !$target_entity_type_info['base table']) {
      throw new SynonymsBehaviorException(t('Target entity type %entity_type is not stored in database.', array(
        '%entity_type' => $this->field['settings']['target_type'],
      )));
    }
    if (!isset($target_entity_type_info['entity keys']['id'])) {
      throw new SynonymsBehaviorException(t('Target entity type %entity_type does not declare primary key.', array(
        '%entity_type' => $this->field['settings']['target_type'],
      )));
    }
    if (!isset($target_entity_type_info['entity keys']['label'])) {
      throw new SynonymsBehaviorException(t('Target entity type %entity_type does not declare label column.', array(
        '%entity_type' => $this->field['settings']['target_type'],
      )));
    }

    $target_entity_alias = $query->innerJoin($target_entity_type_info['base table'], 'target_entity', 'field.' . $column . ' = target_entity.' . $target_entity_type_info['entity keys']['id']);
    $query->addField($target_entity_alias, $target_entity_type_info['entity keys']['label'], 'synonym');
    $query->fields('field', array('entity_id'));
    $query->condition('field.entity_type', $this->instance['entity_type']);
    $query->condition('field.bundle', $this->instance['bundle']);

    $this->synonymsFindProcessCondition($condition, $target_entity_alias . '.' . $target_entity_type_info['entity keys']['label'], 'field.entity_id');
    $query->condition($condition);
    return $query->execute();
  }
}
