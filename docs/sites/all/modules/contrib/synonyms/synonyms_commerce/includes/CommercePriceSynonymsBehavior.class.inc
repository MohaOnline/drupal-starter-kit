<?php

/**
 * @file
 * Enables Commerce Price field type for synonyms integration.
 */

/**
 * Definition of CommercePriceSynonymsBehavior class.
 */
class CommercePriceSynonymsBehavior extends AbstractFieldSynonymsBehavior {

  public function extractSynonyms($entity, $langcode = NULL) {
    $synonyms = array();

    foreach ($this->entityItems($entity, $langcode) as $item) {
      $synonyms[] = commerce_currency_format($item['amount'], $item['currency_code'], $entity);
    }

    return $synonyms;
  }

  public function mergeEntityAsSynonym($trunk_entity, $synonym_entity, $synonym_entity_type) {
    // TODO: remove this thing.
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
    $columns = $this->field['storage']['details']['sql'][FIELD_LOAD_CURRENT][$table];

    $query = db_select($table, 'field');

    $query->fields('field', array('entity_id'));
    $query->addField('field', $columns['amount'], 'amount');
    $query->addField('field', $columns['currency_code'], 'currency_code');
    $query->condition('field.entity_type', $this->instance['entity_type']);
    $query->condition('field.bundle', $this->instance['bundle']);

    $this->synonymsFindProcessCondition($condition, 'field.' . $columns['amount'], 'field.entity_id');
    $query->condition($condition);
    $result = $query->execute();

    $matches = array();

    foreach ($result as $row) {
      $matches[] = (object) array(
        'entity_id' => $row->entity_id,
        'synonym' => commerce_currency_format($row->amount, $row->currency_code),
      );
    }

    return $matches;
  }
}
