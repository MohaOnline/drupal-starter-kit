<?php

/**
 * @file
 * Enables Commerce Product Reference field type for synonyms integration.
 */

/**
 * Definition of CommerceProductReferenceSynonymsBehavior class.
 */
class CommerceProductReferenceSynonymsBehavior extends AbstractFieldSynonymsBehavior {

  public function extractSynonyms($entity, $langcode = NULL) {
    $synonyms = array();

    $product_ids = array();
    foreach ($this->entityItems($entity, $langcode) as $item) {
      $product_ids[] = $item['product_id'];
    }
    $entities = commerce_product_load_multiple($product_ids);
    foreach ($entities as $entity) {
      $synonyms[] = entity_label('commerce_product', $entity);
    }

    return $synonyms;
  }

  public function mergeEntityAsSynonym($trunk_entity, $synonym_entity, $synonym_entity_type) {
    if ('commerce_product' != $synonym_entity_type) {
      return;
    }

    $items = $this->entityItems($trunk_entity);
    $synonym_entity_id = entity_extract_ids($synonym_entity_type, $synonym_entity);
    $items[] = array(
      'product_id' => $synonym_entity_id[0],
    );
    $trunk_entity->{$this->field['field_name']}[LANGUAGE_NONE] = $this->uniqueItems($items, array('product_id'));
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
    $column = $this->field['storage']['details']['sql'][FIELD_LOAD_CURRENT][$table]['product_id'];

    $query = db_select($table, 'field');

    $product_entity_type_info = entity_get_info('commerce_product');

    $product_entity_alias = $query->innerJoin($product_entity_type_info['base table'], 'product', 'field.' . $column . ' = %alias.' . $product_entity_type_info['entity keys']['id']);
    $query->addField($product_entity_alias, $product_entity_type_info['entity keys']['label'], 'synonym');
    $query->fields('field', array('entity_id'));
    $query->condition('field.entity_type', $this->instance['entity_type']);
    $query->condition('field.bundle', $this->instance['bundle']);

    $this->synonymsFindProcessCondition($condition, $product_entity_alias . '.' . $product_entity_type_info['entity keys']['label'], 'field.entity_id');
    $query->condition($condition);
    return $query->execute();
  }
}
