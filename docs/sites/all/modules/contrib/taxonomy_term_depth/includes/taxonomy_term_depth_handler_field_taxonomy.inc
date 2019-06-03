<?php

/**
 * @file
 * Field handler to provide the parents of field value terms.
 *
 */
class taxonomy_term_depth_handler_field_taxonomy extends views_handler_field_taxonomy {

  /**
   * Constructor to provide additional field to add.
   *
   * This constructer assumes the taxonomy_term_data table. If using another
   * table, we'll need to be more specific.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['depth'] = 'depth';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['desired_depth'] = array('default' => 1);
    return $options;
  }

  /**
   * Provide desired depth option
   */
  function options_form(&$form, &$form_state) {
    $form['desired_depth'] = array(
      '#title' => t('Desired depth to show for this hierarchy'),
      '#description' => t('Given a term this will try to show a term\'s parent at the desired depth. Starting with 1 (root depth).'),
      '#type' => 'textfield',
      '#default_value' => $this->options['desired_depth'],
      '#element_validate' => array('element_validate_integer_positive'),
    );
    parent::options_form($form, $form_state);
  }

  /**
   * Find the parent at the desired depth if available. Render whatever the
   * data is as a link to the taxonomy.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    $tid = $this->get_value($values, 'tid');

    if (!empty($tid) && $data !== NULL && $data !== '') {
      $desired_depth = $this->options['desired_depth'];
      $parents_tids = taxonomy_term_depth_get_parents($tid, TRUE);
      $parents_tids[] = $tid;
      $parent_tid = isset($parents_tids[$desired_depth - 1]) ? $parents_tids[$desired_depth - 1] : NULL;

      if ($parent_tid) {
        $parent = taxonomy_term_load($parent_tid);
        $term = new stdClass();
        $term->tid = $parent_tid;
        $data = $parent->name;

        if (!empty($this->options['link_to_taxonomy'])) {
          $term->vid = $this->get_value($values, 'vid');
          $term->vocabulary_machine_name = $values->{$this->aliases['vocabulary_machine_name']};
          $this->options['alter']['make_link'] = TRUE;
          $uri = entity_uri('taxonomy_term', $term);
          $this->options['alter']['path'] = $uri['path'];
        }
      }
      else {
        $data = '';
      }
    }

    if (!empty($this->options['convert_spaces'])) {
      $data = str_replace(' ', '-', $data);
    }

    return $data;
  }
}
