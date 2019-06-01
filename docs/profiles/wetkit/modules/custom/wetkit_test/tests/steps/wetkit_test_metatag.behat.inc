<?php
/**
 * @file
 * Provide Behat step-definitions for the Media module.
 *
 * @todo Also, add a cleanup function to delete files uploaded.
 */

use Drupal\DrupalExtension\Context\DrupalSubContextInterface;
use Drupal\DrupalDriverManager;
use Drupal\DrupalExtension\Context\RawDrupalContext;

class MetatagSubContext extends RawDrupalContext implements DrupalSubContextInterface {

  /**
   * Contains the DrupalDriverManager.
   *
   * @var \Drupal\DrupalDriverManager
   */
  private $drupal;

  /**
   * Initializes context.
   */
  public function __construct(DrupalDriverManager $drupal) {
    $this->drupal = $drupal;
  }

  /**
   * @Then /^the metatag attribute "(?P<attribute>[^"]*)" should have the value "(?P<value>[^"]*)"$/
   *
   * @throws \Exception
   *   If region or link within it cannot be found.
   */
  public function assertMetaRegion($metatag, $value) {
    $element = $this->getSession()->getPage()->find('xpath', '/head/meta[@name="' . $metatag . '"]/@content');

    if ($value == $element->getText()) {
      $result = $value;
    }

    if (empty($result)) {
      throw new \Exception(sprintf('No link to "%s" on the page %s', $metatag, $this->getSession()->getCurrentUrl()));
    }
  }
}
