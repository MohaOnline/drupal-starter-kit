<?php

namespace Drupal\campaignion_email_to_target\Wizard;

use Drupal\campaignion_action\Loader;
use Drupal\campaignion_wizard\WebformStepUnique;
use Drupal\little_helpers\Webform\Webform;

/**
 * Extends the webform step with additional validations.
 */
class FormStep extends WebformStepUnique {

  /**
   * Validate whether the resulting webform is something we can use.
   *
   * We need:
   *   1. At a webform component for each filter of the chosen selector.
   *   2. The target selector must be on a page after the last filter component.
   */
  public function validateStep($form, &$form_state) {
    parent::validateStep($form, $form_state);
    // $form['#node'] has already been updated by form_builder_webform_save_form_validate().
    $dataset = Loader::instance()->actionFromNode($form['#node'])->dataset();
    $webform = new Webform($form['#node']);

    $selector_pages = 1;
    foreach ($dataset->selectors[0]['filters'] as $form_key => $metadata) {
      if ($component = $webform->componentByKey($form_key)) {
        $selector_pages = max($component['page_num'], $selector_pages);
      }
      else {
        form_error($form, t('Missing a form element for filtering targets: %title (with form key %form_key)', ['%title' => $metadata['title'], '%form_key' => $form_key]));
      }
    }
    $target_selector = NULL;
    foreach ($webform->componentsByType('e2t_selector') as $c) {
      $target_selector = $c;
      break;
    }
    if ($target_selector) {
      if ($target_selector['page_num'] <= $selector_pages) {
        form_error($form, t('The target selector must appear on a page after all the target filters.'));
      }
    }
    else {
      form_error($form, t("The target selector field needs to be in the form. Please drag it in."));
    }
  }

}
