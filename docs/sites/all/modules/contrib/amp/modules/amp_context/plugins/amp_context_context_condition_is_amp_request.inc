<?php
/**
 * @file
 * Context condition plugins for AMP Context.
 */
// @codingStandardsIgnoreStart
class amp_context_context_condition_is_amp_request extends context_condition {
  function condition_values() {
    return array(1 => t('Enabled'));
  }

  function editor_form($context = NULL) {
    $form = parent::editor_form($context);
    $form[1]['#title'] = t('Enabledx');
    $form['#weight'] = -10;
    return $form;
  }

  function execute() {
    foreach ($this->get_contexts() as $context) {
      if (amp_is_amp_request()) {
        $this->condition_met($context);
      }
    }
  }
}
// @codingStandardsIgnoreEnd
