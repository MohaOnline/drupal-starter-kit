<?php

namespace Drupal\campaignion_expiry;

use Drupal\campaignion\Contact;
use Drupal\campaignion_expiry\Anonymizer;

/**
 * Cron-job for expiring old contact records.
 */
class ContactCron {

  /**
   * Create a new cron-job instance.
   *
   * @param int $time_limit
   *   Soft time limit (in seconds): No new batches are started in the same
   *   cron-run after the limit has been reached.
   * @param string $inactive_since_str
   *   String that can be passed to strtotime(). Expire all contacts that didn’t
   *   have any activity logged in this time frame.
   */
  public function __construct(Anonymizer $anonymizer, int $time_limit, string $inactive_since_str) {
    $this->anonymizer = $anonymizer;
    $this->timeLimit = $time_limit;
    $this->inactiveSinceStr = $inactive_since_str;
    $this->entityController = entity_get_controller('redhen_contact');
  }


  /**
   * Load inactive contacts from the database.
   */
  protected function loadInactiveContacts(int $inactive_since, int $last_id = 0, int $limit = 20) {
    $sql = <<<SQL
SELECT c.contact_id
FROM redhen_contact c
  INNER JOIN field_data_redhen_contact_email AS ce ON ce.entity_type='redhen_contact' AND ce.entity_id=contact_id
   LEFT OUTER JOIN (
    campaignion_activity ca
    INNER JOIN campaignion_activity_webform USING(activity_id)
  ) ON ca.contact_id=c.contact_id AND ca.created>:time
WHERE redhen_contact_email_value NOT LIKE '%@deleted'
AND ca.activity_id IS NULL
AND c.updated < :time
AND c.contact_id > :last_id
ORDER BY c.contact_id
LIMIT $limit;
SQL;
    $params = [':last_id' => $last_id, ':time' => $inactive_since];
    if ($ids = db_query($sql, $params)->fetchCol()) {
      return entity_load('redhen_contact', $ids, [], TRUE);
    }
    return FALSE;
  }

  /**
   * Execute the cron-job.
   */
  public function run() {
    $stop_after = time() + $this->timeLimit;
    $last_id = 0;
    $contact_count = 0;
    $inactive_since = strtotime($this->inactiveSinceStr);
    $args['@time'] = format_date($inactive_since);
    watchdog('campaignion_expiry', 'Expiring contacts inactive since @time', $args, WATCHDOG_INFO);
    while (time() < $stop_after && ($contacts = $this->loadInactiveContacts($inactive_since, $last_id))) {
      foreach ($contacts as $contact) {
        $this->anonymizer->anonymize($contact);
        $this->anonymizer->deleteOldRevisions($contact);
        $last_id = $contact->contact_id;
        $contact_count++;
      }
    }
    $args['@count'] = $contact_count;
    watchdog('campaignion_expiry', 'Expired @count contacts.', $args, WATCHDOG_INFO);
  }

}
