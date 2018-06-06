<?php

/**
 * @file
 * Add a link to view a Recurly account.
 */

class recurly_account_code_handler extends views_handler_field {

  /**
   * Override init function to provide generic option to link to Recurly acct.
   */
  public function init(&$view, &$data) {
    parent::init($view, $data);
    if (!empty($this->options['link_to_recurly'])) {
      $this->additional_fields['account_code'] = 'account_code';
    }
  }

  /**
   * Override options_form().
   */
  public function options_form(&$form, &$form_state) {
    $form['link_to_recurly'] = array(
      '#title' => t('Link this field to the account at Recurly'),
      '#description' => t("Enable to link this field to the account in Recurly's user interface"),
      '#type' => 'checkbox',
      '#default_value' => $this->options['link_to_recurly'],
    );
    parent::options_form($form, $form_state);
  }

  /**
   * Override option_definition().
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['link_to_recurly'] = array('default' => TRUE, 'bool' => TRUE);
    return $options;
  }

  /**
   * Override render().
   */
  public function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }

  /**
   * Render a link for a Recurly account.
   *
   * @param string $data
   *   Sanitized data to render.
   * @param array $values
   *   An array of values associated with this row.
   *
   * @return mixed
   *   A rendered account string.
   */
  public function render_link($data, $values) {
    if (!empty($this->options['link_to_recurly']) && ($account_code = $this->get_value($values, 'account_code')) && $data !== NULL && $data !== '') {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = recurly_hosted_url('accounts/' . $account_code);
    }
    return $data;
  }

}
