<?php

namespace Drupal\campaignion_newsletters_mailchimp;

use \Drupal\campaignion_newsletters\NewsletterList;
use \Drupal\campaignion_newsletters\ProviderBase;
use \Drupal\campaignion_newsletters\QueueItem;
use \Drupal\campaignion_newsletters\Subscription;

use \Drupal\campaignion_newsletters_mailchimp\Rest\ApiError;
use \Drupal\campaignion_newsletters_mailchimp\Rest\MailChimpClient;

/**
 * Newsletter provider implementation for MailChimp.
 */
class MailChimp extends ProviderBase {

  const WEBHOOK_PATH = 'mailchimp-webhook';

  protected $account;
  protected $api;
  protected $registerWebhooks;

  /**
   * Extract the DC from a valid API-key.
   */
  public static function key2dc($key) {
    return substr($key, strrpos($key, '-') + 1);
  }

  /**
   * Generate the correct hash signature for a webhook call.
   */
  public static function webhookHash($list_id) {
    return drupal_hmac_base64("mailchimp:webhook:$list_id", drupal_get_private_key());
  }

  /**
   * Construct instance from an parameters array as produced by the config form.
   */
  public static function fromParameters(array $params) {
    $dc = static::key2dc($params['key']);
    $endpoint = "https://campaignion:{$params['key']}@{$dc}.api.mailchimp.com/3.0";
    $webhooks = variable_get('campaignion_newsletters_mailchimp_register_webhooks', TRUE) && variable_get('webhooks_enabled', TRUE);
    return new static(new MailChimpClient($endpoint), $params['name'], $webhooks);
  }

  /**
   * Constructor. Gets settings and fetches intial group list.
   */
  public function __construct($api, $name, $register_webhooks) {
    $this->api = $api;
    $this->account = $name;
    $this->registerWebhooks = $register_webhooks;
  }

  /**
   * Get all interest groups for a list.
   *
   * @param string $list_id
   *   List identifier (MailChimp).
   *
   * @return array
   *   Associative array of interest group names keyed by the group id. Groups
   *   in different categories are not discerned.
   */
  public function getInterestGroups($list_id) {
    $groups = [];
    foreach ($this->api->getPaged("/lists/$list_id/interest-categories", ['fields' => 'categories.id'], [], 100, 'categories') as $category) {
      foreach ($this->api->getPaged("/lists/$list_id/interest-categories/{$category['id']}/interests", ['fields' => 'interests.id,interests.name'], [], 100) as $group) {
        $groups[$group['id']] = $group['name'];
      }
    }
    return $groups;
  }

  /**
   * Fetches current lists from the provider.
   *
   * @return array
   *   An array of associative array
   *   (properties: identifier, title, source, language).
   */
  public function getLists() {
    $mc_lists = $this->api->getPaged('/lists', ['fields' => 'lists.id,lists.name'], [], 100);
    $this->merge_vars = array();
    $lists = array();
    foreach ($mc_lists as $list) {
      $list['merge_vars'] = $this->api->getPaged("/lists/{$list['id']}/merge-fields", ['fields' => 'merge_fields.tag'], [], 100);
      $list['groups'] = $this->getInterestGroups($list['id']);
      $lists[] = NewsletterList::fromData([
        'identifier' => $list['id'],
        'title'      => $list['name'],
        'source'     => 'MailChimp-' . $this->account,
        'data'       => (object) $list,
      ]);
    }

    // Try refreshing the webhooks. This will fail on test installations with
    // non-routable addresses (ie. for testing environments) - which is fine.
    try {
      $this->setWebhooks($lists);
    }
    catch (ApiError $e) {
      watchdog_exception('campaignion_newsletters_mailchimp', $e);
    }

    return $lists;
  }

  /**
   * Check whether registering webhooks is enabled.
   *
   * Staging and development installations should set one of these to FALSE.
   */
  protected function registerWebhooks() {
    return $this->registerWebhooks;
  }

  /**
   * Register webhooks for a set of lists (if needed).
   *
   * @param \Drupal\campaignion_newsletters\NewsletterList[] $lists
   *   Register webhooks for these $lists.
   *
   * @throws \Drupal\campaignion_newsletters_mailchimp\Rest\ApiError
   */
  public function setWebhooks(array $lists) {
    $base_url = $GLOBALS['base_url'];
    $register = $this->registerWebhooks();

    foreach ($lists as $list) {
      // Get existing webhook URLs.
      $webhook_urls = [];
      foreach ($this->api->getPaged("/lists/{$list->identifier}/webhooks", ['fields' => 'webhooks.id,webhooks.url'], [], 100) as $webhook) {
        if (substr($webhook['url'], 0, strlen($base_url)) == $base_url) {
          $webhook_urls[$webhook['url']] = $webhook['id'];
        }
      }

      $hash = static::webhookHash($list->list_id);
      $webhook_url = url(static::WEBHOOK_PATH . "/{$list->list_id}/$hash", [
        'absolute' => TRUE,
      ]);

      if (isset($webhook_urls[$webhook_url])) {
        unset($webhook_urls[$webhook_url]);
      }
      elseif ($register) {
        $this->api->post("/lists/{$list->identifier}/webhooks", [], [
          'url' => $webhook_url,
          'events' => [
            'subscribe' => FALSE,
            'unsubscribe' => TRUE,
            'profile' => FALSE,
            'cleaned' => TRUE,
            'campaign' => FALSE,
          ],
          'sources' => [
            'user' => TRUE,
            'admin' => TRUE,
            'api' => TRUE,
          ],
        ]);
      }

      // Now delete all webhooks that we don't need anymore.
      foreach ($webhook_urls as $id) {
        $this->api->delete("/lists/{$list->identifier}/webhooks/$id");
      }
    }
  }

  /**
   * Fetches current lists of subscribers from the provider.
   *
   * @return array
   *   an array of subscribers.
   */
  public function getSubscribers($list) {
    $receivers = array();
    $list_id = $list->data->id;

    foreach ($this->api->getPaged("/lists/{$list_id}/members", [
      'status' => 'subscribed',
      'fields' => 'members.email',
    ], [], 1000) as $new_receiver) {
      $receivers[] = $new_receiver['email'];
    }
    return $receivers;
  }

  /**
   * Get values for all merge tags if possible.
   */
  protected function attributeData(Subscription $subscription, $attributes) {
    $list = $subscription->newsletterList();

    if ($source = $this->getSource($subscription, 'mailchimp')) {
      foreach ($list->data->merge_vars as $attribute) {
        $tag = $attribute['tag'];
        // MailChimp's merge tags are all upper-case. Our form-keys are usually
        // lowercase. Let's try both!
        if (($v = $source->value($tag)) || ($v = $source->value(strtolower($tag)))) {
          $attributes[$tag] = $v;
        }
      }
    }
    return $attributes;
  }

  /**
   * Generate the QueueItem::data attribute for a given subscription.
   */
  public function data(Subscription $subscription, $old_data) {
    $data = $old_data ? $old_data : [];
    $data += [
      'merge_fields' => [],
      'interests' => [],
    ];
    $data['merge_fields'] = $this->attributeData($subscription, $data['merge_fields']);
    // Let other modules alter the data (ie. for adding interest groups).
    drupal_alter('campaignion_newsletters_mailchimp_data', $data, $subscription);
    $fingerprint = sha1(serialize($data));
    return array($data, $fingerprint);
  }

  /**
   * Prepare data for being sent to MailChimp.
   *
   * Make sure everything that needs to be a JSON-object is serialized as such
   * even if empty.
   */
  protected function preprocessData($data) {
    $data += [
      'interests' => [],
      'merge_fields' => [],
    ];
    $data['interests'] = (object) $data['interests'];
    $data['merge_fields'] = (object) $data['merge_fields'];
    return $data;
  }

  /**
   * Subscribe a user, given a newsletter identifier and email address.
   */
  public function subscribe(NewsletterList $list, QueueItem $item) {
    $hash = md5(strtolower($item->email));
    $data = $this->preprocessData($item->data);
    $this->api->put("/lists/{$list->identifier}/members/$hash", [], [
      'email_address' => $item->email,
      'status' => $item->optIn() ? 'pending' : 'subscribed',
    ] + $data);
  }

  /**
   * Update a user's data.
   */
  public function update(NewsletterList $list, QueueItem $item) {
    $hash = md5(strtolower($item->email));
    $data = $this->preprocessData($item->data);
    $this->api->put("/lists/{$list->identifier}/members/$hash", [], [
      'email_address' => $item->email,
      'status_if_new' => 'subscribed', // Updates don’t trigger opt-in emails.
    ] + $data);
  }

  /**
   * Unsubscribe a user, given a newsletter identifier and email address.
   *
   * Should ignore the request if there is no such subscription.
   */
  public function unsubscribe(NewsletterList $list, QueueItem $item) {
    $hash = md5(strtolower($item->email));
    try {
      $this->api->put("/lists/{$list->identifier}/members/$hash", [], [
        'status' => 'unsubscribed',
        'email_address' => $item->email,
      ]);
    }
    catch (ApiError $e) {
      // Ignore 404 errors.
      if ($e->getCode() != 404) {
        throw $e;
      }
    }
  }

}
