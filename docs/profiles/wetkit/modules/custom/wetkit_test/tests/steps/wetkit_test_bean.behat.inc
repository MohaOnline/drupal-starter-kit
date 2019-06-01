<?php
/**
 * @file
 * Provide Behat step-definitions for Beans.
 *
 * @todo This should become bean.behat.inc in the Bean module!
 */

use Drupal\DrupalExtension\Context\DrupalSubContextInterface;
use Drupal\DrupalDriverManager;
use Drupal\DrupalExtension\Context\RawDrupalContext;
use Drupal\DrupalExtension\Hook\Scope\AfterUserCreateScope;

class BeanSubContext extends RawDrupalContext implements DrupalSubContextInterface {

  /**
   * Contains the DrupalDriverManager.
   *
   * @var \Drupal\DrupalDriverManager
   */
  private $drupal;

  /**
   * An array of Drupal users created by other contexts.
   *
   * @var array
   */
  protected $external_users = array();

  /**
   * Initializes context.
   */
  public function __construct(DrupalDriverManager $drupal) {
    $this->drupal = $drupal;
  }

  /**
   * Get a Bean by admin title.
   *
   * @param string $admin_title
   *   The admin title of the Bean to retrieve.
   *
   * @return object
   *   The Bean entity object.
   */
  protected function getBeanByAdminTitle($admin_title) {
    $beans = bean_load_multiple(array(), array('admin_title' => $admin_title));
    if (empty($beans)) {
      throw new \Exception(sprintf('Bean "%s" was not found', $admin_title));
    }
    return reset($beans);
  }

  /**
   * Get the list of revisions for a Bean.
   *
   * @param int $bid
   *   The ID for the Bean.
   */
  protected function getBeanRevisions($bid) {
    return db_query("SELECT * FROM {bean} WHERE bid = :bid", array(':bid' => $bid))->fetchAll();
  }

  /**
   * Record all the users created during this scenario.
   *
   * We need to use this hook so we can get users created in steps on other
   * contexts (most probably the DrupalContext).
   *
   * @AfterUserCreate
   */
  public function afterUserCreate(AfterUserCreateScope $scope) {
    $user = $scope->getEntity();
    $this->external_users[$user->name] = $user;
  }

  /**
   * Get a list of UIDs.
   *
   * @return
   *   An array of numeric UIDs of users created by Given... steps during this scenario.
   */
  public function getUIDs() {
    $uids = array();
    foreach ($this->external_users as $user) {
      $uids[] = $user->uid;
    }
    return $uids;
  }

  /**
   * Cleans up Beans.
   *
   * @AfterScenario @api
   */
  public function cleanBeans($event) {
    $bids = array();

    // Get UIDs of users created during this scenario.
    $uids = $this->getUIDs();
    if (!empty($uids)) {
      // Find any Beans created by the test users.
      // First, get a list of Beans with their first revision VID.
      $bean_vids = db_query('SELECT min(vid) AS vid FROM {bean_revision} GROUP BY bid')->fetchAll();
      $vids = array();
      if (!empty($bean_vids)) {
        foreach ($bean_vids as $vid) {
          $vids[] = $vid->vid;
        }
        // Then, check whether that first revision was created by a current test user.
        $bean_bids = db_query('SELECT bid FROM {bean_revision} WHERE vid IN (:vids) AND uid IN (:uids)', array(':uids' => $uids, ':vids' => $vids))->fetchAll();
        if (!empty($bean_bids)) {
          foreach ($bean_bids as $bid) {
            $bids[] = $bid->bid;
          }
        }
      }
      // Add Beans created by users to the $bids variable.
      $bids = array_unique($bids);
    }

    // Delete any beans that were created by test users or a Given step.
    foreach ($bids as $bid) {
      $bean = bean_load($bid);
      bean_delete($bean);
    }

    // Clear out the list.
    $this->external_users = array();
  }

  /**
   * @Then bean :admin_title should have :expected_count revision(s)
   *
   * @todo: Can we only allow this to work with @api tests?
   */
  public function assertRevisionCount($admin_title, $expected_count) {
    $bean = $this->getBeanByAdminTitle($admin_title);
    $revisions = $this->getBeanRevisions($bean->bid);

    $actual_count = count($revisions);
    if ($actual_count != $expected_count) {
      throw new \Exception(sprintf('Bean "%s" has %s revisions (rather than the expected %s)', $admin_title, $actual_count, $expected_count));
    }
  }

  /**
   * @Given I am viewing revision :revision_number of Bean :admin_title
   *
   * @todo: Can we only allow this to work with @api tests?
   */
  public function iAmViewingBeanRevision($revision_number, $admin_title) {
    $bean = $this->getBeanByAdminTitle($admin_title);
    $revisions = $this->getBeanRevisions($bean->bid);

    $vid = $revisions[$revision_number - 1]->vid;
    $path = "admin/structure/bean/view/{$bean->bid}/revision/$vid";

    $this->getSession()->visit($this->locatePath($path));
  }
}
