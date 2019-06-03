<?php

namespace Drupal\campaignion_action;

/**
 * Generate or check a HMAC signed link with query arguments.
 */
class SignedLink {
  public $path;
  protected $parameter;
  public $query;

  public static function fromCurrentLocation() {
    return new static(current_path(), drupal_get_query_parameters());
  }

  public function __construct($path, $query, $hash_parameter = 'hash') {
    $this->path = $path;
    $this->query = $query;
    $this->parameter = $hash_parameter;
  }

  /**
   * Check for a matching hash in the query.
   *
   * @return bool
   *   TRUE if there is a hash in the query and it matches the current path
   *   and query arguments.
   */
  public function checkHash() {
    $q = $this->query;
    $p = $this->parameter;
    return isset($q[$p]) && ($q[$p] == $this->hash());
  }

  /**
   * Generate the hash for this link.
   *
   * @return string
   *   Valid hash for this link object.
   */
  public function hash() {
    $q = $this->query;
    unset($q[$this->parameter]);
    $str = $this->path . '?' . http_build_query($q);
    return drupal_hmac_base64($str, drupal_get_private_key());
  }

  /**
   * Generate a signed version of the query array.
   *
   * @return array
   *   query array that can be passed into @see url() or @see l().
   */
  public function hashedQuery($options = []) {
    $q = $this->query;
    $q[$this->parameter] = $this->hash();
    return $q;
  }

}
