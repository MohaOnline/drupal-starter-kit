<?php
/**
 * @file
 * implements NewsletterProvider using the Dotmailer API.
 *
 * See http://api.dotmailer.com for documentation.
 */

namespace Drupal\campaignion_newsletters_dotmailer;

use \Drupal\campaignion_newsletters\NewsletterList;
use \Drupal\campaignion_newsletters\ProviderBase;
use \Drupal\campaignion_newsletters\QueueItem;
use \Drupal\campaignion_newsletters\Subscription;


class Provider extends ProviderBase {
  public $api;
  protected $account;

  /**
   * Construct a new instance from config parameters.
   */
  public static function fromParameters(array $params) {
    $api = new Api\Client($params['username'], $params['password']);
    return new static($api, $params['name']);
  }

  /**
   * Constructor.
   */
  public function __construct(Api\Client $api, $name) {
    $this->api = $api;
    $this->account = $name;
  }

  /**
   * Fetches current lists from the provider.
   *
   * @return array
   *   An array of associative array
   *   (properties: identifier, title, source, language).
   */
  public function getLists() {
    $lists = [];

    $data_fields = $this->api->get('data-fields');
    $addressbooks = $this->api->get('address-books');

    foreach ($addressbooks as $addressbook) {
      $lists[] = NewsletterList::fromData([
        'identifier' => $addressbook['id'],
        'title' => $addressbook['name'],
        'source' => 'dotmailer-' . $this->account,
        'data' => (object) ['fields' => $data_fields],
      ]);
    }
    return $lists;
  }

  /**
   * Fetches current lists of subscribers from the provider.
   *
   * @return array
   *   an array of subscribers.
   */
  public function getSubscribers($list) {
    $last_sync = variable_get('dotmailer_last_sync', []);
    $last_sync += [$list->identifier => 0];

    $since = gmdate('Y-m-d', $last_sync[$list->identifier]);

    $page = 0;
    $receivers = array();
    $list_id = $list->data->id;

    do {
      $new_receivers = $this->api->get("address-books/$list_id/contacts/modified-since/$since", [
        'select' => 1000,
        'skip' => $page++,
      ]);
      foreach ($new_receivers as $new_receiver) {
        $receivers[] = $new_receiver['email'];
      }
    } while ($new_receivers);
    $last_sync[$list->identifier] = date('Y-m-d', REQUEST_TIME);
    return $receivers;
  }

  /**
   * Get values for all merge tags if possible.
   */
  protected function attributeData(Subscription $subscription, $field_data = []) {
    $list = $subscription->newsletterList();

    if ($source = $this->getSource($subscription, 'dotmailer')) {
      foreach ($list->data->fields as $field) {
        $tag = $field['name'];
        // dotmailer's fields are all upper-case. Our form-keys are usually
        // lowercase. Let's try both!
        if (($v = $source->value($tag)) || ($v = $source->value(strtolower($tag)))) {
          $field_data[$tag] = $v;
        }
      }
    }
    // Let other modules alter the attributes (ie. for adding groupings).
    drupal_alter('campaignion_newsletters_dotmailer_attributes', $field_data, $subscription, $source);
    return $field_data;
  }

  /**
   * {@inheritdocs}
   */
  public function data(Subscription $subscription, $old_data) {
    $data = $this->attributeData($subscription, $old_data ? $old_data : []);
    $fingerprint = sha1(serialize($data));
    return array($data, $fingerprint);
  }

  /**
   * Subscribe a user, given a newsletter identifier and email address.
   *
   * @return: True on success.
   */
  public function subscribe(NewsletterList $list, QueueItem $item) {
    $data_fields = [];
    foreach ($item->data as $key => $value) {
      $data_fields[] = ['key' => $key, 'value' => $value];
    }

    $contact = [
      'email' => $item->email,
      'optInType' => $item->optIn() ? 'Single' : 'Double',
      'dataFields' => $data_fields,
      'emailType' => 'Html',
    ];
    return $this->api->post("address-books/{$list->identifier}/contacts", [], $contact);
  }

  /**
   * Unsubscribe a user, given a newsletter identifier and email address.
   *
   * Should ignore the request if there is no such subscription.
   *
   * @return: True on success.
   */
  public function unsubscribe(NewsletterList $list, QueueItem $item) {
    if ($contact = $this->api->get('contacts/' . $item->email)) {
      $this->api->delete("address-books/{$list->identifier}/contacts/{$contact['id']}");
    }
  }

}

