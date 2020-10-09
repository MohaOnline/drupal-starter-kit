<?php

/**
 * @file
 * Enables Taxonomy Term Reference field type to be source of synonyms.
 */

/**
 * Definition of TaxonomySynonymsBehavior class.
 */
class TaxonomySynonymsBehavior extends AbstractFieldSynonymsBehavior implements AutocompleteSynonymsBehavior, SelectSynonymsBehavior {

  public function extractSynonyms($entity, $langcode = NULL) {
    $synonyms = array();

    $terms = array();
    foreach ($this->entityItems($entity, $langcode) as $item) {
      $terms[] = $item['tid'];
    }
    $terms = taxonomy_term_load_multiple($terms);
    foreach ($terms as $term) {
      $synonyms[] = entity_label('taxonomy_term', $term);
    }
    return $synonyms;
  }

  public function mergeEntityAsSynonym($trunk_entity, $synonym_entity, $synonym_entity_type) {
    // Taxonomy term reference supports only referencing of entity types
    // 'taxonomy_term'.. duh.
    if ($synonym_entity_type != 'taxonomy_term') {
      return;
    }

    $items = $this->entityItems($trunk_entity);

    // Checking that $field is configured to reference the vocabulary of
    // $synonym_entity term.
    $is_allowed = FALSE;
    foreach ($this->field['settings']['allowed_values'] as $setting) {
      if ($synonym_entity->vocabulary_machine_name == $setting['vocabulary']) {
        if ($setting['parent'] == 0) {
          // No need to check parent property as there is no limitation on it.
          $is_allowed = TRUE;
          break;
        }
        else {
          foreach (taxonomy_get_parents_all($synonym_entity->tid) as $parent) {
            if ($parent->tid == $setting['parent']) {
              $is_allowed = TRUE;
              break(2);
            }
          }
        }
      }
    }
    if (!$is_allowed) {
      // Synonym term is from a vocabulary that is not expected by this field,
      // or under unexpected parent.
      return;
    }
    $items[] = array(
      'tid' => $synonym_entity->tid,
    );
    $trunk_entity->{$this->field['field_name']}[LANGUAGE_NONE] = $this->uniqueItems($items, array('tid'));
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
    $column = $this->field['storage']['details']['sql'][FIELD_LOAD_CURRENT][$table]['tid'];

    $query = db_select($table, 'field');
    $term_alias = $query->innerJoin('taxonomy_term_data', 'term', 'field.' . $column . ' = term.tid');
    $query->addField($term_alias, 'name', 'synonym');
    $query->fields('field', array('entity_id'));
    $query->condition('field.entity_type', $this->instance['entity_type']);
    $query->condition('field.bundle', $this->instance['bundle']);

    $this->synonymsFindProcessCondition($condition, $term_alias . '.name', 'field.entity_id');
    $query->condition($condition);
    return $query->execute();
  }
}
