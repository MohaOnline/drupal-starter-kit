<?php

namespace Drupal\dvg_authentication_dummy;

use Drupal\dvg_authentication\AuthenticationProviderBase;

/**
 * Class DummyAuthenticationProvider.
 */
class DummyAuthenticationProvider extends AuthenticationProviderBase {

  /**
   * {@inheritdoc}
   */
  protected $useLevelLogos = TRUE;

  /**
   * {@inheritdoc}
   */
  public function getId() {
    return 'dummy';
  }

  /**
   * {@inheritdoc}
   */
  public function getLabel() {
    return t('Dummy provider');
  }

  /**
   * {@inheritdoc}
   */
  public function getErrorMessage() {
    return '';
  }

  /**
   * {@inheritdoc}
   */
  public function getButtonDescription() {
    return t('The dummy provider is for testing only.');
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm() {
    $form = parent::buildConfigurationForm();

    $dummy_user = $this->getConfig('dummy_user');

    // Add configuration fields for the test user, so we can use it for testing.
    $form['dummy_user'] = [
      '#type' => 'fieldset',
      '#title' => t('Debug dummy user data'),
      '#description' => t('The dummy user data can be used for testing tokens and prefill.'),
      '#tree' => TRUE,
    ];

    $form['dummy_user']['first_name'] = [
      '#type' => 'textfield',
      '#title' => t('First name'),
      '#default_value' => $dummy_user['first_name'] ?? '',
    ];
    $form['dummy_user']['name_infix'] = [
      '#type' => 'textfield',
      '#title' => t('Name infix'),
      '#default_value' => $dummy_user['name_infix'] ?? '',
    ];
    $form['dummy_user']['last_name'] = [
      '#type' => 'textfield',
      '#title' => t('Last name'),
      '#default_value' => $dummy_user['last_name'] ?? '',
    ];
    $form['dummy_user']['date_of_birth'] = [
      '#type' => 'date',
      '#title' => t('Birthday'),
      '#default_value' => $dummy_user['date_of_birth'] ?? '',
    ];
    $form['dummy_user']['identifier'] = [
      '#type' => 'textfield',
      '#title' => t('Identifier'),
      '#default_value' => $dummy_user['identifier'] ?? '',
      '#description' => t('This value usually corresponds with the identification number of the user, e.g. the BSN or KvK number.'),
    ];
    $form['dummy_user']['bsn'] = [
      '#type' => 'textfield',
      '#title' => t('BSN'),
      '#default_value' => $dummy_user['bsn'] ?? '',
    ];
    $form['dummy_user']['kvk_number'] = [
      '#type' => 'textfield',
      '#title' => t('KvK number'),
      '#default_value' => $dummy_user['kvk_number'] ?? '',
    ];
    $form['dummy_user']['kvk_department_number'] = [
      '#type' => 'textfield',
      '#title' => t('KvK Department number'),
      '#default_value' => $dummy_user['kvk_department_number'] ?? '',
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function buildLevelConfigFields($level) {
    $fields = parent::buildLevelConfigFields($level);
    // Make level logos optional for the dummy provider,
    // so they can be added for test purposes.
    if (isset($fields['logo'])) {
      $fields['logo']['#required'] = FALSE;
    }
    return $fields;
  }

  /**
   * {@inheritdoc}
   */
  public function login($level = AuthenticationProviderBase::LEVEL_NONE) {
    // The dummy login callback always logs the user directly in as
    // an external user.
    return $this->authenticationManager->userLogin($this, 'dummy_id', $level);
  }

  /**
   * {@inheritdoc}
   */
  public function getLevels() {
    return [
      'level_1' => 'Level 1',
      'level_2' => 'Level 2',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function getUser(\stdClass $account) {
    return new DummyUser($account, $this);
  }

  /**
   * {@inheritdoc}
   */
  public function getRequirementsInfo() {
    $requirements = parent::getRequirementsInfo();
    // Change the severity to 'warning' because this module should not
    // be used on a production environment.
    $requirements['dvg_authentication_' . $this->getId()]['severity'] = REQUIREMENT_WARNING;
    $requirements['dvg_authentication_' . $this->getId()]['description'] = t('Do not use this module in a production environment.');
    return $requirements;
  }

}
