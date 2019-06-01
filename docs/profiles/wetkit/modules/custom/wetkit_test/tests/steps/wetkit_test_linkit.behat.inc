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

class LinkItSubContext extends RawDrupalContext implements DrupalSubContextInterface {

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
   * @When /^I hover on the linkit result with selector "([^"]*)"$/
   */
  public function iHoverTheLinkitResult($cssSelector)
  {
    $session = $this->getSession();
    $session->executeScript('document.getElementsByClassName("better-autocomplete")[0].style.display = "block"');
    $element = $session->getPage()->find('xpath', $session->getSelectorsHandler()->selectorToXpath('css', $cssSelector));
    if (null === $element) {
      throw new \InvalidArgumentException(sprintf('Could not evaluate CSS Selector: "%s"', $cssSelector));
    }
    $element->mouseOver();
  }

  /**
   * @When /^I click on the linkit result with selector "([^"]*)"$/
   */
  public function iClickTheLinkitResult($cssSelector)
  {
    $session = $this->getSession();
    $session->executeScript('document.getElementsByClassName("better-autocomplete")[0].style.display = "block"');
    $element = $session->getPage()->find('xpath', $session->getSelectorsHandler()->selectorToXpath('css', $cssSelector));
    if (null === $element) {
      throw new \InvalidArgumentException(sprintf('Could not evaluate CSS Selector: "%s"', $cssSelector));
    }
    $element->click();
  }
}
