<?php

namespace Drupal\campaignion_manage\Filter;

/**
 * Filter contacts by redhen_contact.redhen_state.
 */
class SupporterState extends Base implements FilterInterface {

  /**
   * {@inheritdoc}
   */
  public function defaults() {
    return ['value' => REDHEN_STATE_ACTIVE];
  }

  /**
   * {@inheritdoc}
   */
  public function formElement(array &$element, array &$form_state, array &$values) {
    $element['value'] = [
      '#type' => 'radios',
      '#title' => t('Status is …'),
      '#options' => redhen_state_options(),
      '#default_value' => !empty($values['value']),
    ];
  }

  /**
   * Return the title of this filter used in the filter listing.
   */
  public function title() {
    return t('Status');
  }

  /**
   * Apply the filter to a query.
   */
  public function apply($query, array $values) {
    if (isset($values['value'])) {
      $query->condition('r.redhen_state', $values['value']);
    }
  }

}
