<?php

namespace Drupal\campaignion_wizard;

/**
 * Wizard step for configuring webforms using form_builder.
 *
 * NOTE: Needs form_builder_webform to work.
 */
class WebformStep extends WizardStep {
  protected $step = 'form';
  protected $title = 'Form';
  protected function loadIncludes() {
    module_load_include('inc', 'form_builder', 'includes/form_builder.admin');
    module_load_include('inc', 'form_builder', 'includes/form_builder.api');
    module_load_include('inc', 'form_builder', 'includes/form_builder.cache');
  }

  public function pageCallback() {
    $build = parent::pageCallback();
    $form =& $build[0];
    $form['actions']['#access'] = FALSE;
    $form['#weight'] = -100;

    $path = drupal_get_path('module', 'webform');
    $build['#attached']['css'][] = $path . '/css/webform.css';
    $build['#attached']['css'][] = $path . '/css/webform-admin.css';
    $build['#attached']['js'][] = $path . '/js/webform.js';
    $build['#attached']['js'][] = $path . '/js/webform-admin.js';
    $build['#attached']['js'][] = $path . '/js/select-admin.js';
    $build['#attached']['js'][] = drupal_get_path('module', 'campaignion_wizard') . '/js/form-builder-submit.js';
    $build['#attached']['library'][] = array('system', 'ui.datepicker');

    // Remove #title from custom submit buttons fieldset.
    if (module_exists('webform_custom_buttons')) {
      $form['submit_buttons']['#title'] = '';
    }

    // Build form for webform_template select box.
    if (module_exists('campaignion_action_template')) {
      $node = $this->wizard->node;
      // Check if there are existing form submissions.
      $sql = "SELECT 1 FROM {webform_submissions} WHERE nid=:nid LIMIT 1";
      $has_submissions = db_query($sql, [':nid' => $node->nid])->fetchField();
      $webform_template_form = drupal_get_form('campaignion_action_template_selector_form', $node);
      $webform_template_form['#weight'] = 1;

      if ($has_submissions) {
        $warning_msg = t('Applying form templates is not possible on forms that have already captured data. ' .
          'If you want to apply a form template you have to <a href="@delete-submissions">delete all form data first</a>.');
        $o['query']['destination'] = url("node/$node->nid/wizard/form");
        $delete_submissions_url = url("node/$node->nid/webform-results/clear", $o);

        $webform_template = &$webform_template_form['webform_template'];
        $webform_template['webform_template_warning'] = [
          '#markup' => '<p id="action-template-warning">' .
          t($warning_msg, ['@delete-submissions' => $delete_submissions_url]) .
          '</p>',
        ];

        $webform_template['source']['#access'] = FALSE;
        $webform_template['submit-template']['#access'] = FALSE;
        $webform_template['#title'] = '';
      }

      $build[] = $webform_template_form;
    }

    // Load all components.
    $components = webform_components();
    foreach ($components as $component_type => $component) {
      webform_component_include($component_type);
    }
    module_load_include('inc', 'form_builder', 'includes/form_builder.admin');
    foreach (form_builder_interface('webform', $form['nid']['#value']) as $k => $f) {
      $build[$k + 2] = $f;
    }

    return $build;
  }

  public function stepForm($form, &$form_state) {
    $form = parent::stepForm($form, $form_state);
    $form = \form_builder_webform_save_form($form, $form_state, $this->wizard->node->nid);
    $form_state['build_info']['base_form_id'] = 'form_builder_webform_save_form';
    return $form;
  }

  public function checkDependencies() {
    return isset($this->wizard->node->nid);
  }

  public function validateStep($form, &$form_state) {
    // form_builder <= 7.x-1.13 did not have this function.
    if (function_exists('form_builder_webform_save_form_validate')) {
      form_builder_webform_save_form_validate($form, $form_state);
    }
  }

  public function submitStep($form, &$form_state) {
    form_builder_webform_save_form_submit($form, $form_state);
  }

  public function status() {
    return NULL;
  }
}
