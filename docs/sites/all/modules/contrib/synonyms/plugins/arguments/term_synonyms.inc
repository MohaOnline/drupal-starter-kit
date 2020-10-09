<?php

/**
 * @file
 * Plugin to provide a synonyms-friendly argument handler for a Taxonomy term.
 */

if (module_exists('taxonomy')) {
  $plugin = array(
    'title' => t("Taxonomy term: ID (synonyms-friendly)"),
    'keyword' => 'term',
    'description' => t('Creates a single taxonomy term from a taxonomy term name or one of its synonyms.'),
    'context' => 'synonyms_term_synonyms_context',
    'default' => array('breadcrumb' => TRUE, 'transform' => FALSE),
    'settings form' => 'synonyms_term_synonyms_settings_form',
    'placeholder form' => 'synonyms_term_synonyms_ctools_argument_placeholder',
    'breadcrumb' => 'synonyms_term_synonyms_breadcrumb',
  );
}

/**
 * Discover if this argument gives us the term we crave.
 */
function synonyms_term_synonyms_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  // If unset it wants a generic, unfilled context.
  if ($empty) {
    return ctools_context_create_empty('entity:taxonomy_term');
  }

  $conf['vids'] = is_array($conf['vids']) ? array_filter($conf['vids']) : array();

  if (is_object($arg)) {
    $term = $arg;

    if (!empty($conf['vids']) && empty($conf['vids'][$term->vid])) {
      return NULL;
    }
  }
  else {
    if ($conf['transform']) {
      $tids = db_select('taxonomy_term_data', 't')
        ->fields('t', array('tid'))
        ->where("REPLACE(t.name, ' ', '-') = :argument", array(
          ':argument' => $arg,
        ));
      if (!empty($conf['vids'])) {
        $tids->condition('t.vid', $conf['vids']);
      }
      $tids = $tids->execute()->fetchCol();
      $terms = taxonomy_term_load_multiple($tids);
    }
    else {
      $terms = taxonomy_get_term_by_name($arg);
    }

    if (!empty($conf['vids'])) {
      foreach ($terms as $k => $term) {
        if (!isset($conf['vids'][$term->vid])) {
          unset($terms[$k]);
        }
      }
    }

    if (empty($terms)) {
      // We couldn't find the term by name, so we will look it up now by
      // synonyms.
      $vocabularies = taxonomy_vocabulary_load_multiple(empty($conf['vids']) ? FALSE : $conf['vids']);
      foreach ($vocabularies as $vocabulary) {
        $condition = db_and();
        if ($conf['transform']) {
          $condition->where("REPLACE(" . AbstractSynonymsBehavior::COLUMN_SYNONYM_PLACEHOLDER . ", ' ', '-') = :argument", array(
            ':argument' => $arg,
          ));
        }
        else {
          $condition->condition(AbstractSynonymsBehavior::COLUMN_SYNONYM_PLACEHOLDER, $arg);
        }
        $rows = synonyms_synonyms_find($condition, 'taxonomy_term', $vocabulary->machine_name);
        if (!empty($rows)) {
          // We have found a match, no need to search further.
          $terms[] = taxonomy_term_load($rows[0]->entity_id);
          break;
        }
      }
    }

    if (empty($terms)) {
      return NULL;
    }
    $term = array_shift($terms);
  }

  $context = ctools_context_create('entity:taxonomy_term', $term);
  $context->original_argument = $arg;
  return $context;
}

/**
 * Settings form for the argument.
 */
function synonyms_term_synonyms_settings_form(&$form, &$form_state, $conf) {
  $vocabularies = taxonomy_get_vocabularies();
  $options = array();
  foreach ($vocabularies as $vid => $vocab) {
    $options[$vid] = $vocab->name;
  }
  $form['settings']['vids'] = array(
    '#title' => t('Limit to these vocabularies'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => !empty($conf['vids']) ? $conf['vids'] : array(),
    '#description' => t('If no vocabularies are checked, terms from all vocabularies will be accepted.'),
  );

  $form['settings']['breadcrumb'] = array(
    '#title' => t('Inject hierarchy into breadcrumb trail'),
    '#type' => 'checkbox',
    '#default_value' => !empty($conf['breadcrumb']),
    '#description' => t('If checked, taxonomy term parents will appear in the breadcrumb trail.'),
  );

  $form['settings']['transform'] = array(
    '#title' => t('Transform dashes in URL to spaces in term name filter values'),
    '#type' => 'checkbox',
    '#default_value' => !empty($conf['transform']),
  );
}

/**
 * Form fragment to get an argument to convert a placeholder for preview.
 */
function synonyms_term_synonyms_ctools_argument_placeholder($conf) {
  return array(
    '#type' => 'textfield',
    '#description' => t('Enter a taxonomy term name.'),
  );
}

/**
 * Inject the breadcrumb trail if necessary.
 */
function synonyms_term_synonyms_breadcrumb($conf, $context) {
  // Outsource the real implementation of breadcrumb to terms argument plugin.
  $plugin = ctools_get_plugins('ctools', 'arguments', 'term');
  $function = ctools_plugin_get_function($plugin, 'breadcrumb');
  if ($function) {
    call_user_func_array($function, func_get_args());
  }
}
