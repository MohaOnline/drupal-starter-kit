<?php

namespace Drupal\dvg_authentication_privacy;

use Drupal\dvg_authentication\ConfigInterface;

/**
 * Manages privacy parameters.
 */
class Privacy implements ConfigInterface {

  /**
   * Configuration settings of this plugin.
   *
   * @var array
   */
  protected $configuration;

  /**
   * Privacy constructor.
   */
  public function __construct() {
    // Load the configuration for this plugin.
    $this->configuration = variable_get('dvg_authentication_privacy', $this->getDefaultConfiguration());
  }

  /**
   * {@inheritdoc}
   */
  public function getConfig($setting) {
    return $this->configuration[$setting] ?? NULL;
  }

  /**
   * {@inheritdoc}
   */
  protected function getDefaultConfiguration() {
    return [
      'save_user_data' => '30',
      'anonymize_user_data' => '3',
      'anonymize_ip_address' => '3',
    ];
  }

  /**
   * Get the configuration form structure.
   *
   * @param array $form
   *   An associative array containing the initial structure of the form.
   * @param array $form_state
   *   The current state of the form.
   *
   * @return array
   *   The form structure.
   */
  public function getConfigurationForm(array $form, array &$form_state) {

    $configuration_form['anonymize_ip_address'] = [
      '#type' => 'textfield',
      '#attributes' => [
        ' type' => 'number',
      ],
      '#title' => t('Anonymize ip address after (amount) days'),
      '#default_value' => $this->getConfig('anonymize_ip_address'),
      '#size' => 8,
      '#weight' => 1,
      '#description' => t('Number of days after which the ip address will be anonymized.'),
    ];

    $configuration_form['anonymize_user_data'] = [
      '#type' => 'textfield',
      '#attributes' => [
        ' type' => 'number',
      ],
      '#title' => t('Anonymize user data after (amount) days'),
      '#default_value' => $this->getConfig('anonymize_user_data'),
      '#size' => 8,
      '#weight' => 1,
      '#description' => t('Number of days after which the user data will be anonymized.'),
    ];

    $configuration_form['save_user_data'] = [
      '#type' => 'textfield',
      '#attributes' => [
        ' type' => 'number',
      ],
      '#title' => t('Remove user data after (amount) days'),
      '#default_value' => $this->getConfig('save_user_data'),
      '#size' => 8,
      '#weight' => 1,
      '#description' => t('Number of days after which the user data will be deleted.'),
    ];

    $form['dvg_authentication_privacy'] = $configuration_form;
    $form['dvg_authentication_privacy']['#tree'] = TRUE;
    // Add our object's validate function.
    $form['#validate'][] = static::class . '::validate';
    return $form;
  }

  /**
   * Validator for the Privacy configuration form.
   *
   * @param array $form
   *   The form.
   * @param array $form_state
   *   The form state.
   */
  public static function validate(array $form, array &$form_state) {
    $values = $form_state['values']['dvg_authentication_privacy'];
    $anonymize_ip_address_label = t('Anonymize ip address after (amount) days');
    $anonymize_user_data_label = t('Anonymize user data after (amount) days');
    $save_user_data_label = t('Remove user data after (amount) days');
    if ($values['anonymize_ip_address'] <= 0) {
      form_error($form['dvg_authentication_privacy']['anonymize_ip_address'], t('%field can not be smaller than 0.', ['%field' => $anonymize_ip_address_label]));
    }
    if ($values['anonymize_user_data'] <= 0) {
      form_error($form['dvg_authentication_privacy']['anonymize_user_data'], t('%field can not be smaller than 0.', ['%field' => $anonymize_user_data_label]));
    }
    if ($values['save_user_data'] <= 0) {
      form_error($form['dvg_authentication_privacy']['save_user_data'], t('%field can not be smaller than 0.', ['%field' => $save_user_data_label]));
    }
    if ($values['anonymize_user_data'] >= $values['save_user_data']) {
      form_error($form['dvg_authentication_privacy']['anonymize_user_data'], t('%anonymize needs to be less than %save.', [
        '%anonymize' => $anonymize_user_data_label,
        '%save' => $save_user_data_label,
      ]));
      form_error($form['dvg_authentication_privacy']['save_user_data']);
    }
  }

}
