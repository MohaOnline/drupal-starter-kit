<?php
/**
 * @file
 * Plugin for CiviCRM core fields.
 */

$plugin = array(
  'label' => t('CiviCRM Custom Fields'),
  'handler' => array(
    'class' => 'CiviCrmCustomFields',
  ),
);

class CiviCrmCustomFields extends CRMCoreConversionHandler {
  /**
   * Returns array of source plugins compatible with this object.
   */
  public function compatible() {
    return array('CivicrmDataSourceHandler');
  }

  /**
   * Process row of data.
   */
  public function prepareRow(&$row, $importer) {
    foreach ($row as $key => $value) {
      $civicrm_entity = array();
      list($civicrm_entity['type'], $civicrm_entity['bundle'], $civicrm_entity['field']) = explode(':', $key);
      $field = $this->getCiviCrmFieldInstance($civicrm_entity['type'], $civicrm_entity['field']);
      if ($this->isCustomField($field)) {
        // @see civicrm/CRM/Core/BAO/CustomField.php - CRM_Core_BAO_CustomField::dataType().
        if (!empty($field['data_type'])) {
          switch ($field['data_type']) {
            case 'Country':
              $row->{$key} = $this->convertConstant('countryIsoCode', $value);
              break;

            case 'StateProvince':
              $row->{$key} = $this->convertConstant('stateProvinceAbbreviation', $value);
              break;
          }
        }
      }
    }
  }

  /**
   * Returns TRUE if it is custom field.
   */
  public function isCustomField($field) {
    return !empty($field['custom_group_id']);
  }

  /**
   * Convert id to value via CiviCRM API.
   */
  public function convertConstant($name, $id) {
    $value = _crm_core_data_import_civicrm_get_constant($name);
    return !empty($value[$id]) ? $value[$id] : FALSE;
  }
}
