<?php
/**
 * @file
 * Default conversion handler for CRM Core Data Import.
 */

abstract class CRMCoreConversionHandler {
  /**
   * Process row of data.
   *
   * @param object $row
   *   Raw source data.
   *
   * @param CRMCoreDataImport $importer
   *   Instance of importer.
   */
  abstract public function prepareRow(&$row, $importer);

  /**
   * Returns array of source plugins compatible with this object.
   *
   * Empty array means compatible with all plugins.
   *
   * @return array
   *   Array of compatible types.
   */
  public function compatible() {
    return array();
  }

  /**
   * Returns country code for country name.
   */
  public function countryCode($country_name) {
    include_once DRUPAL_ROOT . '/includes/locale.inc';
    $countries = country_get_list();
    if ($country_code = array_search($country_name, $countries)) {
      return $country_code;
    }
    else {
      return $country_name;
    }
  }

  /**
   * Returns field instance.
   */
  public function getCiviCrmFieldInstance($entity_type, $field_name) {
    $fields = crm_core_data_import_civicrm_api($entity_type, 'getfields');
    return !empty($fields[$field_name]) ? $fields[$field_name] : FALSE;
  }
}
