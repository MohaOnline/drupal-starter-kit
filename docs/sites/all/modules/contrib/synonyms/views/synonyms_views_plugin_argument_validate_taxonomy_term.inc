<?php

/**
 * @file
 * An extended version of the 'taxonomy term' argument validator plugin, which
 * supports synonyms as arguments.
 */

/**
 * Validate whether an argument is an acceptable taxonomy term.
 */
class synonyms_views_plugin_argument_validate_taxonomy_term extends views_plugin_argument_validate_taxonomy_term {

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // We just want to add yet another way of validation - synonyms friendly
    // validation.
    $form['type']['#options']['synonym'] = t('Term name or synonym');
    $form['type']['#options']['synonym_tid'] = t('Term name or synonym converted to Term ID');
  }

  function validate_argument($argument) {
    $vocabularies = array_filter($this->options['vocabularies']);
    $type = $this->options['type'];
    $transform = $this->options['transform'];

    switch ($type) {
      case 'synonym':
      case 'synonym_tid':
        // We are requested to do synonyms friendly validation. Firstly we are
        // going to query the database trying to find a term with our argument's
        // name. If we find one, it is great and we stop right there. Otherwise,
        // we look up by synonyms.
        $query = db_select('taxonomy_term_data', 't')
          ->fields('t', array('tid', 'name'));
        if (!empty($vocabularies)) {
          $query->innerJoin('taxonomy_vocabulary', 'v', 't.vid = v.vid');
          $query->condition('v.machine_name', $vocabularies);
        }
        if ($transform) {
          $query->where("REPLACE(t.name, ' ', '-') = :name", array(':name' => $argument));
        }
        else {
          $query->condition('t.name', $argument, '=');
        }
        $result = $query->range(0, 1)
          ->execute();
        if ($result->rowCount()) {
          // We have found a term, we are going to use it.
          $term = taxonomy_term_load($result->fetchField(0));
          $this->argument->argument = $this->synonyms_get_term_property($term);
          $this->argument->validated_title = check_plain(entity_label('taxonomy_term', $term));
          return TRUE;
        }

        $bundles = $vocabularies;
        if (empty($vocabularies)) {
          // At this point we want to convert an empty $vocabularies (implicitly
          // meaning "all") into actually a list of all vocabularies.
          $bundles = synonyms_bundle_normalize('taxonomy_term', $vocabularies);
        }

        foreach ($bundles as $bundle) {
          $condition = db_and();
          if ($transform) {
            $condition->where("REPLACE(" . AbstractSynonymsBehavior::COLUMN_SYNONYM_PLACEHOLDER . ", ' ', '-') = :argument", array(
              ':argument' => $argument,
            ));
          }
          else {
            $condition->condition(AbstractSynonymsBehavior::COLUMN_SYNONYM_PLACEHOLDER, $argument);
          }
          $synonyms = synonyms_synonyms_find($condition, 'taxonomy_term', $bundle);
          if (!empty($synonyms)) {
            $term = taxonomy_term_load($synonyms[0]->entity_id);
            $this->argument->argument = $this->synonyms_get_term_property($term);
            $this->argument->validated_title = check_plain(entity_label('taxonomy_term', $term));
            return TRUE;
          }
        }

        // We haven't found any synonym that matched our argument, so there is
        // no term to return.
        return FALSE;

      default:
        return parent::validate_argument($argument);
    }
  }

  /**
   * Return necessary property (per chosen validator) of encountered term.
   *
   * @param $term object
   *   Fully loaded taxonomy term from which necessary property should be
   *   returned
   *
   * @return mixed
   *   Necessary property (per chosen validator) of the provided term
   */
  function synonyms_get_term_property($term) {
    switch ($this->options['type']) {
      case 'synonym':
        return $term->name;

      case 'synonym_tid':
        return $term->tid;
    }
    return NULL;
  }
}
