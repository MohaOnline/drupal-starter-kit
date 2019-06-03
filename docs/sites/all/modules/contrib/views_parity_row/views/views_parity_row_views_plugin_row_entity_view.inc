<?php

// Until I find a better solution...
include('views_parity_row_base.inc');

/**
 * @file
 * Contains the Views Parity Row row style plugin for entities.
 */

/**
 * Plugin which performs a entity_view on the resulting object.
 *
 * @ingroup views_row_plugins
 */
class views_parity_row_views_plugin_row_entity_view extends entity_views_plugin_row_entity_view {
  use views_parity_row_base;
  /**
   * Return the main options, which are shown in the summary title.
   */
  function options_form_summary_options() {
    $entity_info = entity_get_info($this->entity_type);
    $options = array();
    if (!empty($entity_info['view modes'])) {
      foreach ($entity_info['view modes'] as $mode => $settings) {
        $options[$mode] = $settings['label'];
      }
    }
    if (empty($options)) {
      $options = array(
        'teaser' => t('Teaser'),
        'full' => t('Full content')
      );
    }

    return $options;
  }

  public function pre_render($values) {
    if (!empty($values)) {
      list($this->entity_type, $this->entities) = $this->view->query->get_result_entities($values, !empty($this->relationship) ? $this->relationship : NULL, isset($this->field_alias) ? $this->field_alias : NULL);
    }
  }

  public function render($values) {
    if ($entity = $this->get_value($values)) {
      $entity->view = $this->view;
      $view_mode = $this->find_view_mode($this->options, $this->view->row_index);

      $render = entity_view($this->entity_type, array($entity), $view_mode);

      return drupal_render($render);
    }
  }
}

