<?php
/**
 * @file
 * Plugin for CiviCRM core fields.
 */

$plugin = array(
  'label' => t('CiviCRM Core'),
  'handler' => array(
    'class' => 'CiviCrmCoreFields',
  ),
);

class CiviCrmCoreFields extends CRMCoreConversionHandler {
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
      $converted_value = NULL;
      $civicrm_entity = array();
      list($civicrm_entity['type'], $civicrm_entity['bundle'], $civicrm_entity['field'], $civicrm_entity['property'], $civicrm_entity['sub_property']) = explode(':', $key);

      switch ($civicrm_entity['field']) {
        case 'country':
          $converted_value = $this->countryCode($value);
          break;

        case 'prefix_id':
          $converted_value = $this->fetchFieldValue('prefix_id', $value, $civicrm_entity['type']);
          break;

        case 'suffix_id':
          $converted_value = $this->fetchFieldValue('suffix_id', $value, $civicrm_entity['type']);
          break;

        case 'gender_id':
          $converted_value = $this->fetchFieldValue('gender_id', $value, $civicrm_entity['type']);
          break;

        case 'priority_id':
          $row->{$civicrm_entity['type'] . ':' . $civicrm_entity['bundle'] . ':priority'} = $this->fetchFieldValue('priority_id', $value, $civicrm_entity['type']);
          break;

        case 'status_id':
          $row->{$civicrm_entity['type'] . ':' . $civicrm_entity['bundle'] . ':status'} = $this->fetchOptionsValue('activity_status', $value);
          break;

        case 'email':
          if (isset($civicrm_entity['property'])) {
            switch ($civicrm_entity['property']) {
              case 'location_type_id':
                foreach ($value as &$item) {
                  $item = $this->fetchLocationType($item);
                }
                break;
            }

            $converted_value = $value;
          }
          break;

        case 'website':
          if (isset($civicrm_entity['property'])) {
            switch ($civicrm_entity['property']) {
              case 'website_type_id':
                foreach ($value as &$item) {
                  $item = $this->fetchOptionsValue('website_type', $item);
                }
                break;
            }

            $converted_value = $value;
          }
          break;

        case 'phone':
          if (isset($civicrm_entity['property'])) {
            switch ($civicrm_entity['property']) {
              case 'location_type_id':
                foreach ($value as &$item) {
                  $item = $this->fetchLocationType($item);
                }
                break;

              case 'phone_type_id':
                foreach ($value as &$item) {
                  $item = $this->fetchLocationType($item);
                }
                break;
            }

            $converted_value = $value;
          }
          break;

        case 'address':
          if (isset($civicrm_entity['property'])) {
            switch ($civicrm_entity['property']) {
              case 'location_type_id':
                foreach ($value as &$item) {
                  $item = $this->fetchLocationType($item);
                }
                break;

              case 'country_id':
                foreach ($value as &$item) {
                  $item = $this->fetchCountryById($item);
                }
                break;

              case 'state_province_id':
                foreach ($value as &$item) {
                  $item = $this->fetchStateById($item);
                }
                break;
            }

            $converted_value = $value;
          }
          break;

        // Address location type.
        case 'location_type_id':
          $converted_value = $this->fetchLocationType($value);
          break;

        // Address country ID.
        case 'country_id':
          $converted_value = $this->fetchCountryById($value);
          break;

        // Address state ID.
        case 'state_province_id':
          $converted_value = $this->fetchStateById($value);
          break;
      }

      if (!empty($converted_value)) {
        $row->{$key} = $converted_value;
      }
    }
  }

  /**
   * Returns value based on the getfields CiviCRM API method.
   */
  public function fetchFieldValue($field, $value, $entity_type) {
    $fields = crm_core_data_import_civicrm_api($entity_type, 'getfields');
    if (!empty($fields[$field]['options'][$value])) {
      return $fields[$field]['options'][$value];
    }
    return $value;
  }

  /**
   * Returns value based on the option_group and option_value database tables.
   */
  public function fetchOptionsValue($group_name, $value) {
    $params = array('name' => $group_name);
    $option_group = crm_core_data_import_civicrm_api('option_group', 'get', $params);
    $option_group = reset($option_group);
    if (!empty($option_group['id'])) {
      $options = array('option_group_id' => $option_group['id'], 'value' => $value);
      $type = crm_core_data_import_civicrm_api('option_value', 'get', $options);
      $type = reset($type);
      return $type['name'];
    }
    return FALSE;
  }

  /**
   * Returns value based on the civicrm_location_type database table.
   */
  public function fetchLocationType($id) {
    $type = crm_core_data_import_civicrm_api('location_type', 'get', array('id' => $id));
    $type = reset($type);
    if (!empty($type['id'])) {
      return $type['name'];
    }
    return $id;
  }

  /**
   * Returns country code by id.
   *
   * @see civicrm/CRM/Core/PseudoConstant.php
   */
  public function fetchCountryById($id) {
    return CRM_Core_PseudoConstant::countryIsoCode($id);
  }

  /**
   * Returns state code by id.
   *
   * @see civicrm/CRM/Core/PseudoConstant.php
   */
  public function fetchStateById($id) {
    return CRM_Core_PseudoConstant::stateProvinceAbbreviation($id);
  }
}
