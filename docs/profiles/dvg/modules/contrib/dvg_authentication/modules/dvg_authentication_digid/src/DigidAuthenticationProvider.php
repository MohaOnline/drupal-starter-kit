<?php

namespace Drupal\dvg_authentication_digid;

use Drupal\dvg_authentication\AuthenticationProviderBase;
use Drupal\dvg_authentication\SamlAuthenticationProviderBase;

/**
 * Class DigidAuthenticationProvider.
 */
class DigidAuthenticationProvider extends SamlAuthenticationProviderBase {

  /**
   * {@inheritdoc}
   */
  public function getId() {
    return 'digid';
  }

  /**
   * {@inheritdoc}
   */
  public function getLabel() {
    return t('DigiD');
  }

  /**
   * {@inheritdoc}
   */
  public function getButtonDescription() {
    return t('You are a private individual and have a burgerservicenummer (BSN). Login with DigiD. For more information visit <a href="@url">digid.nl</a>.', ['@url' => 'https://www.digid.nl']);
  }

  /**
   * {@inheritdoc}
   */
  public function getLevels() {
    return [
      'basic' => t('basic'),
      'middle' => t('middle'),
      'substantial' => t('substantial'),
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function getErrorMessage() {
    // It is a requirement of DigiD that this message is always shown in
    // Dutch, therefore it is not translatable.
    return 'Er is een fout opgetreden in de communicatie met DigiD. Probeer het later opnieuw. Controleer de website <a href="http://www.digid.nl">digid.nl</a> voor de laatste informatie.';
  }

  /**
   * {@inheritdoc}
   */
  protected function getDefaultConfiguration() {
    return [
      'auth_source' => FALSE,
      'show_confirmation_page' => FALSE,
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function login($level = AuthenticationProviderBase::LEVEL_NONE) {

    // Show the confirmation page, if enabled and the user hasn't confirmed yet.
    if ($this->getConfig('show_confirmation_page') && !isset($_GET['confirm'])) {
      return $this->getLoginConfirmationPage();
    }

    // If the dummy mode is enabled, we can only login with a test account,
    // so redirect to the normal login page and show a message.
    if ($this->isDummyMode($level)) {
      $this->redirectDummyLogin($level);
    }

    $simplesamlphp = $this->getSimpleSaml($level);
    // Is the user logged into SimpleSAMLphp?
    if ($simplesamlphp) {
      if ($simplesamlphp->isAuthenticated()) {
        $attributes = $simplesamlphp->getAttributes();
        $nameid = $attributes['nameid'][0];

        if ($this->authenticationManager->userLogin($this, $nameid, $level)) {
          // Store the BSN in the session object.
          $value = explode(':', $nameid)[1];
          $_SESSION['dvg_authentication_digid'][DIGID_SECTOR_BSN] = $value;
        }
        else {
          $msg = 'Error logging into Drupal. SAML attributes: @attributes';
          $msg_args = ['@attributes' => var_export($attributes, 1)];
          watchdog('dvg_authentication_digid', $msg, $msg_args, WATCHDOG_ERROR);
          drupal_set_message($this->getErrorMessage(), 'error');
        }

        drupal_goto();
      }
      else {
        // If the destination is not available,
        // redirect errors to the front page.
        $error_path = $_GET['destination'] ?: '<front>';
        // Add the provider id to the error url.
        $options = [
          'query' => [
            'provider_id' => $this->getId(),
            'level' => $level,
          ],
        ];
        $simplesamlphp->requireAuth(['ErrorURL' => url($error_path, $options)]);
      }
    }
    else {
      $link = l(t('Config'), 'admin/config/services/dvg-authentication/digid');
      watchdog('dvg_authentication_digid', 'Missing SAML configuration or enable dummy mode.', [], WATCHDOG_ERROR, $link);
    }

    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function getUser(\stdClass $account) {
    return new DigidUser($account, $this);
  }

}
