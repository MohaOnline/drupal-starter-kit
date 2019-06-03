<?php

namespace Drupal\campaignion;

interface ContactTypeInterface {
  /**
   * Contact-types will be constructed without any arguments.
   */
  public function __construct();
  /**
   * Get an exporter-wrapper for this contact-type.
   *
   * @param string $target
   *   Target to export to (ie. cleverreach, dadiapi, mailchimp).
   * @param stdclass $language
   *   Target language.
   *
   * @return \Drupal\campaignion\CRM\ExporterInterface
   *   An exporter capable for the specified target.
   */
  public function exporter($target, $language);
  /**
   * Get an importer-wrapper for this contact.
   *
   * @param string $source
   *   Source to import from (ie. campaignion_activity, …).
   *
   */
  public function importer($source);
}
