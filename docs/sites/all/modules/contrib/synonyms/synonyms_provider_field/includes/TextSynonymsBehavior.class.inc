<?php

/**
 * @file
 * Enables text and number field types to be source of synonyms.
 */

/**
 * Definition of TextSynonymsBehavior class.
 */
class TextSynonymsBehavior extends AbstractFieldSynonymsBehavior implements AutocompleteSynonymsBehavior, SelectSynonymsBehavior {

  public function extractSynonyms($entity, $langcode = NULL) {
    $synonyms = array();

    foreach ($this->entityItems($entity, $langcode) as $item) {
      $synonyms[] = $item['value'];
    }

    return $synonyms;
  }

  public function mergeEntityAsSynonym($trunk_entity, $synonym_entity, $synonym_entity_type) {
    $synonym = entity_label($synonym_entity_type, $synonym_entity);

    switch ($this->field['type']) {
      case 'text':
        break;

      // We add synonyms for numbers only if $synonym is a number.
      case 'number_integer':
      case 'number_float':
      case 'number_decimal':
        if (!is_numeric($synonym)) {
          return;
        }
        break;

    }
    $items = $this->entityItems($trunk_entity);
    $items[] = array(
      'value' => $synonym,
    );
    $trunk_entity->{$this->field['field_name']}[LANGUAGE_NONE] = $this->uniqueItems($items, array('value'));
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
    $column = $this->field['storage']['details']['sql'][FIELD_LOAD_CURRENT][$table]['value'];

    $this->synonymsFindProcessCondition($condition, $column, 'entity_id');

    $query = db_select($table);
    $query->fields($table, array('entity_id'));
    $query->addField($table, $column, 'synonym');
    return $query->condition($condition)
      ->condition('entity_type', $this->instance['entity_type'])
      ->condition('bundle', $this->instance['bundle'])
      ->execute();
  }

}
