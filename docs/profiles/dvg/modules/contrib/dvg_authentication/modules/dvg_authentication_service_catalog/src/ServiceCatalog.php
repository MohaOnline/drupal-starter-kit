<?php

namespace Drupal\dvg_authentication_service_catalog;

/**
 * Class ServiceCatalog.
 */
class ServiceCatalog {

  /**
   * UUID for the service catalog, must be unique for each environment.
   *
   * @var string
   */
  protected $serviceCatalogUuid;

  /**
   * The OverheidsIdentificatieNummer, identifier of the municipality.
   *
   * @var string
   */
  protected $oin;

  /**
   * Name of the organisation.
   *
   * @var string
   */
  protected $organisationName;

  /**
   * Machine name of the organisation (optional).
   *
   * @var string|null
   */
  protected $organisationMachineName;

  /**
   * Url of the website.
   *
   * @var string
   */
  protected $organisationUrl;

  /**
   * Url to the privacy policy page.
   *
   * @var string
   */
  protected $privacyPolicyUrl;

  /**
   * Identifier of the IdentityProvider, can be found in the broker's metadata.
   *
   * @var string
   */
  protected $idp;

  /**
   * The service configuration.
   *
   * @var array
   */
  protected $authServices = [];

  /**
   * The service configuration, maps service name to service type.
   *
   * @var array
   */
  protected $namedServices = [];

  /**
   * The certificate used for saml. Required for eIDAS.
   *
   * The cert directory is found in saml config.
   * The certificate is found in authsources.
   *
   * @var string
   */
  protected $certificate;

  /**
   * ServiceCatalog constructor.
   */
  public function __construct() {
    $this->loadConfiguration();
  }

  /**
   * Load the SimpleSAML configuration file.
   *
   * @return bool
   *   Return false when required files are missing.
   */
  public function loadConfiguration() {
    $saml_path = libraries_get_path('simplesamlphp');
    $auth_sources_path = $saml_path . '/config/authsources.php';
    $config_path = $saml_path . '/config/config.php';

    // Load library because it is needed when loading the config file.
    libraries_load('simplesamlphp');

    if (!file_exists($auth_sources_path) || !file_exists($config_path)) {
      return FALSE;
    }

    // Load the auth_sources configuration.
    include $auth_sources_path;
    // Load simplesaml config.
    include $config_path;

    // Use the settings from the authsources file.
    /* @var string $service_catalog_uuid Introduced by SAML config. */
    $this->serviceCatalogUuid = $service_catalog_uuid;
    /* @var string $oin Introduced by SAML config. */
    $this->oin = $oin;
    /* @var string $organisation_name Introduced by SAML config. */
    $this->organisationName = $organisation_name;
    /* @var string $organisation_machine_name Introduced by SAML config. */
    if (isset($organisation_machine_name)) {
      $this->organisationMachineName = $organisation_machine_name;
    }
    /* @var string $organisation_url Introduced by SAML config. */
    $this->organisationUrl = $organisation_url;
    /* @var string $idp Introduced by SAML config. */
    $this->idp = $idp;
    $this->privacyPolicyUrl = url('/privacy', ['absolute' => TRUE]);

    /* @var array $etoegang_levels Introduced by SAML config. */
    foreach ($etoegang_levels as $service_id => $service_settings) {
      list($service_type) = explode('_', $service_settings['id'], 2);
      if (!isset($service_settings['serviceID'])) {
        // Support the old method of determining service ID.
        $service_settings['serviceID'] = "urn:etoegang:DV:$oin:services:$service_id";
      }
      $service_settings['type'] = $service_type;
      $this->authServices[$service_type][$service_id] = $service_settings;
      $this->namedServices[$service_settings['id']] = $service_type;
    }

    // Use settings from the config file.
    /* @var array $config Introduced by SAML config. */
    /* @var string $certificate Introduced by SAML config. */
    $cert_path = $config['certdir'] . $certificate;
    if (file_exists($cert_path)) {
      $this->certificate = $cert_path;
    }
    else {
      drupal_set_message(t('Certificate not correctly configured in authsources or config'), 'error');
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Get the configured Service types.
   *
   * @return array
   *   The configured authentication types.
   */
  public function getServiceTypes() {
    return array_keys($this->authServices);
  }

  /**
   * Generate and download the Catalog XML.
   *
   * @param array $service_types
   *   Selected service types.
   *
   * @throws \Exception
   */
  public function downloadCatalogXml(array $service_types) {
    $certificate_content = file_get_contents($this->certificate);
    // Remove the Begin and End of the certificate and newlines, as it is
    // not accepted.
    $certificate_content = str_replace([
      '-----BEGIN CERTIFICATE-----',
      '-----END CERTIFICATE-----',
      "\r\n",
      "\n",
    ], ['', '', ' ', ' '], $certificate_content);

    $date = new \DateTime();
    $variables = [
      'service_catalog_uuid' => $this->serviceCatalogUuid,
      'issue_instant' => $date->format('Y-m-d\TH:i:s\Z'),
      'oin' => $this->oin,
      'organisation_name' => $this->organisationName,
      'organisation_url' => $this->organisationUrl,
      'privacy_policy_url' => $this->privacyPolicyUrl,
      'idp' => $this->idp,
      'services' => [],
      'certificate_name' => basename($this->certificate),
      'certificate_content' => trim($certificate_content),
    ];

    foreach ($this->authServices as $service_type => $auth_services) {
      if (in_array($service_type, $service_types)) {
        $variables['services'] += $auth_services;
      }
    }

    $file_name = $this->organisationName . ' service-catalog.xml';
    header('Content-Type: application/xml');
    header("Content-Disposition: attachment; filename=\"$file_name\"");
    echo theme('authentication_service_catalog', $variables);
    exit();
  }

  /**
   * Provides a simple download for simplesaml metadata.
   *
   * Adds the correct headers for filename and file extension.
   *
   * @param string $service
   *   Name of the service to get the metadata for.
   */
  public function downloadMetadata($service) {
    if (isset($this->namedServices[$service])) {
      $file_name = ($this->organisationMachineName ?? $this->organisationName) . "-$service.xml";
      header('Content-Type: application/xml');
      header("Content-Disposition: attachment; filename=\"$file_name\"");
      $host = $_SERVER['HTTP_ORIGIN'] ?? $_SERVER['REQUEST_SCHEME'] . '://' . $_SERVER['SERVER_NAME'];
      $url = $host . '/simplesaml/module.php/saml/sp/metadata.php/' . $service;
      echo file_get_contents($url);
      drupal_exit();
    }
    else {
      drupal_not_found();
    }
  }

  /**
   * Build a render array with information about the current SAML configuration.
   *
   * @return array
   *   Render array with all information.
   */
  public function getInfo() {
    $build = [
      'oin' => [
        '#type' => 'item',
        '#title' => t('OIN'),
        '#markup' => $this->oin,
      ],
      'organisationName' => [
        '#type' => 'item',
        '#title' => t('Organisation Name'),
        '#markup' => $this->organisationName,
      ],
      'organisationUrl' => [
        '#type' => 'item',
        '#title' => t('Organisation URL'),
        '#markup' => $this->organisationUrl,
      ],
      'idp' => [
        '#type' => 'item',
        '#title' => t('IDP'),
        '#markup' => $this->idp,
      ],
      'certificate' => [
        '#type' => 'item',
        '#title' => t('Certificate'),
        '#markup' => $this->certificate,
      ],
    ];

    // Add links to download metadata.
    $build['metadata'] = [
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Metadata downloads'),
    ];
    $link_options = [
      'attributes' => [
        'download' => NULL,
      ],
    ];
    foreach ($this->namedServices as $service_name => $service_type) {
      if (!isset($build['metadata'][$service_type])) {
        $build['metadata'][$service_type] = [
          'clickable' => [
            '#theme' => 'item_list',
            '#title' => $service_type,
            '#items' => [],
          ],
          'copyable' => [
            '#type' => 'textarea',
            '#disabled' => TRUE,
            '#value' => '',
            '#rows' => 1,
          ],
        ];
      }
      $host = $_SERVER['HTTP_ORIGIN'] ?? $_SERVER['REQUEST_SCHEME'] . '://' . $_SERVER['SERVER_NAME'];
      $url = 'admin/config/services/dvg-authentication/service-catalog/metadata/' . $service_name;
      $build['metadata'][$service_type]['clickable']['#items'][] = l($service_name, $url, $link_options);
      $build['metadata'][$service_type]['copyable']['#value'] .= "$host/$url\n";
      $build['metadata'][$service_type]['copyable']['#rows']++;
    }

    return $build;
  }

}
