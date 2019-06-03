<?php

namespace Drupal\campaignion_mp_fields;

use \Drupal\campaignion_email_to_target\Api\Client;
use \Drupal\campaignion_email_to_target\Api\ConfigError;

/**
 * Extract UK postcodes from address fields and add MP data to an entity.
 */
class MPDataLoader {

  /**
   * API-Client for the e2t-api.
   *
   * @var \Drupal\campaignion_email_to_target\Api\Client
   */
  protected $api;

  /**
   * Generate new instance from hard-coded configuration.
   *
   * @return static|null
   *   A new instance of this class or NULL if the API is not available.
   */
  public static function fromConfig() {
    try {
      $api = Client::fromConfig();
    }
    catch (ConfigError $e) {
      return NULL;
    }
    $setters = [
      'mp_constituency' => function ($field, $constituency, $target) {
        if (!empty($constituency['name'])) {
          $field->set($constituency['name']);
        }
      },
      'mp_country' => function ($field, $constituency, $target) {
        if (!empty($constituency['country']['name'])) {
          $tagger = Tagger::byNameAndParentUuid('mp_country');
          $tagger->tagSingle($field, $constituency['country']['name'], TRUE);
        }
      },
      'mp_party' => function ($field, $constituency, $target) {
        if (!empty($target['political_affiliation'])) {
          $tagger = Tagger::byNameAndParentUuid('mp_party');
          $tagger->tagSingle($field, $target['political_affiliation'], TRUE);
        }
      },
      'mp_salutation' => function ($field, $constituency, $target) {
        if (!empty($target['salutation'])) {
          $field->set($target['salutation']);
        }
      },
    ];
    return new static($api, $setters);
  }

  /**
   * Construct a new MPDataLoader.
   *
   * @param function[] $setters
   *   List of setter functions keyed by field names. Each function takes
   *   exactly 3 arguments:
   *   - field: The entity metadata wrapper representation of the field.
   *   - constituency: The constituency data from the API (or NULL).
   *   - target: The target data from the API (or NULL).
   *   The functions should set their field’s value if available.
   */
  public function __construct(Client $api, array $setters) {
    $this->api = $api;
    $this->setters = $setters;
  }

  /**
   * Update the data in the MP fields.
   *
   * All addressfields on the entity are checked for a UK postcode. The first
   * postcode found is used to query the e2t database for data on the
   * constituency and MP.
   *
   * @param string $entity_type
   *   Type of the entity that is passed.
   * @param string $entity
   *   The entity that should have the data added.
   */
  public function setData($entity_type, $entity) {
    list($id, $rev_id, $bundle) = entity_extract_ids($entity_type, $entity);
    $fields = field_read_fields([
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'type' => 'addressfield',
    ]);
    if (!$fields) {
      return;
    }
    $target_fields = field_read_fields([
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'field_name' => array_keys($this->setters),
    ]);
    if (!$target_fields) {
      return;
    }
    $postcode = NULL;
    foreach ($fields as $field) {
      if ($items = field_get_items($entity_type, $entity, $field['field_name'])) {
        foreach ($items as $item) {
          if ($postcode = $this->extractPostcode($item)) {
            break 2;
          }
        }
      }
    }
    if ($postcode) {
      $data = $this->api->getTargets('mp', ['postcode' => $postcode]);
      if ($data) {
        $target = !empty($data[0]) ? $data[0] : NULL;
        $constituency = !empty($target['constituency']) ? $target['constituency'] : NULL;
        $wrapped = entity_metadata_wrapper($entity_type, $entity);
        foreach ($target_fields as $field_name => $field) {
          $this->setters[$field_name]($wrapped->{$field_name}, $constituency, $target);
        }
      }
    }
  }

  /**
   * Extracts a valid UK postcode from an addressfield item.
   *
   * @param array $item
   *   The address to extract from.
   *
   * @return string|null
   *   A valid (normalized) UK postcode, or NULL if the address doesn’t contain
   *   one.
   */
  protected function extractPostcode(array $item) {
    if (!empty($item['postal_code']) && !empty($item['country']) && $item['country'] == 'GB') {
      $r = postal_code_validation_validate($item['postal_code'], 'GB');
      if (empty($r['error'])) {
        // Strip spaces and dashes allowed by postal_code_validation_validate().
        return preg_replace('/[ -]/', '', $item['postal_code']);
      }
    }
  }

}
