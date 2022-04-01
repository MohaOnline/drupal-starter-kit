<?php

namespace Drupal\campaignion_content_security_policy;

/**
 * Service for setting the configured Content-Security-Policy headers.
 */
class HeaderGenerator {

  /**
   * A Drupal API wrapper.
   *
   * @var \Drupal\campaignion_content_security_policy\Drupal
   */
  protected $drupal;

  /**
   * Array of trusted frame-ancestors.
   *
   * @var array
   */
  protected $sources;

  /**
   * Create a new instance based on the textarea value.
   *
   * @param \Drupal\campaignion_content_security_policy\Drupal $drupal
   *   A Drupal API wrapper.
   * @param string|null $trusted_sources_str
   *   The string trusted source URLs as stored in the Drupal variable. This
   *   might be NULL when the module is enabled initally and before the caches
   *   are cleared.
   */
  public static function fromConfig(Drupal $drupal, $trusted_sources_str) {
    $trusted_sources_str = $trusted_sources_str ?? "'self'\n";
    $trusted_sources = array_filter(array_map('trim', explode("\n", $trusted_sources_str)));
    return new static($drupal, $trusted_sources);
  }

  /**
   * Create a new instance.
   */
  public function __construct(Drupal $drupal, array $trusted_sources) {
    $this->drupal = $drupal;
    $this->sources = $trusted_sources;
  }

  /**
   * Add the headers for this request.
   */
  public function addHeaders() {
    $header_urls = implode(' ', $this->sources);
    $this->drupal->addHeader('Content-Security-Policy', "frame-ancestors $header_urls");
  }

}
