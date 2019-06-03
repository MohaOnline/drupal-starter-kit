<?php
/**
 * @file
 * implements NewsletterProvider using Optivos API.
 */

namespace Drupal\campaignion_newsletters_optivo;

use \Drupal\campaignion\CRM\Import\Source\ArraySource;
use \Drupal\campaignion\CRM\Import\Source\CombinedSource;

use \Drupal\campaignion_newsletters\ApiError;
use \Drupal\campaignion_newsletters\ApiPersistentError;
use \Drupal\campaignion_newsletters\NewsletterList;
use \Drupal\campaignion_newsletters\ProviderBase;
use \Drupal\campaignion_newsletters\QueueItem;
use \Drupal\campaignion_newsletters\Subscription;

class Optivo extends ProviderBase {
  protected $account;
  protected $optinProcessId;
  protected $factory;

  public static function fromParameters(array $params) {
    $factory = ClientFactory::fromCredentials([
      $params['mandatorId'],
      $params['username'],
      $params['password'],
    ]);
    return new static($params, $factory);
  }

  /**
   * Constructor. Creates an active session with Optivo.
   */
  public function __construct(array $params, ClientFactory $factory) {
    $this->account = $params['name'];
    $this->factory = $factory;
    $this->optinProcessId = $params['optinProcessId'];
  }

  /**
   * Fetches current lists from the provider.
   *
   * @return array
   *   An array of associative array
   *   (properties: identifier, title, source, language).
   */
  public function getLists() {
    $service = $this->factory->getClient('RecipientList');
    $list_ids = $service->getAllIds();
    $lists = [];
    foreach ( $list_ids as $id ) {
      $name = $service->getName($id);
      $attributes = $service->getAttributeNames($id, 'en');
      $lists[] = NewsletterList::fromData([
        'identifier' => $id,
        'title' => $name,
        'source' => 'Optivo-' . $this->account,
        'data' => (object) ['attributeNames' => $attributes],
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
    $service = $this->factory->getClient('Recipient');
    $receivers = $service->getAll($list->identifier, 'email');
    return $receivers;
  }

  /**
   * Subscribe a user, given a newsletter identifier and email address.
   */
  public function subscribe(NewsletterList $list, QueueItem $item) {
    $service = $this->factory->getClient('Recipient');
    $mail = $item->email;
    $recipientId = $mail;
    $address = $mail;
    $data = $item->data + ['names' => [], 'values' => []];
    $status = $service->add2(
      $list->identifier,
      $item->optIn() && $this->optinProcessId ? $this->optinProcessId : 0,
      $recipientId,
      $address,
      $data['names'],
      $data['values']
    );
    // Status codes and comments according to v1.13 of the SOAP API.
    switch ($status) {
      case 0: // Recipient has been added.
        return TRUE;
      case 1: // Recipient validation failed.
        throw new ApiPersistentError('Optivo', 'Recipient validation failed (@mail)', ['@mail' => $mail], $status);
      case 2: // Recipient is unsubscribed.
        throw new ApiError('Optivo', 'Recipient is unsubscribed (@mail)', ['@mail' => $mail], $status);
      case 3: // Recipient is blacklisted.
        throw new ApiPersistentError('Optivo', 'Recipient is blacklisted (@mail)', ['@mail' => $mail], $status);
      case 4: // Recipient is bounce overflowed.
        throw new ApiPersistentError('Optivo', 'Recipient is bounce overflowed (@mail)', ['@mail' => $mail], $status);
      case 5: // Recipient is already in the list so update the data.
        $service->setAttributes($list->identifier, $mail, $data['names'], $data['values']);
        return TRUE;
      case 6: // Recipient has been filtered.
        throw new ApiError('Optivo', 'Recipient has been filtered (@mail)', ['@mail' => $mail], $status);
      case 7: // A general error occured.
        throw new ApiError('Optivo', 'A general error occured when adding @mail', ['@mail' => $mail], $status);
    }

    throw new ApiError('Optivo', 'API returned unexpected status code', [], $status);
  }

  /**
   * Update user data.
   */
  public function update(NewsletterList $list, QueueItem $item) {
    // The best way to do this would be to try a setAttributes() call and then
    // if that fails because the recipient does not yet exist try to subscribe.
    // However the exception thrown by setAttributes() does not easily provide
    // the information why it failed.
    // The only other option would be to call contains() first and then either
    // subscribe() or setAttributes() which means just as many API-calls.
    $item->args['send_optin'] = FALSE;
    $item->args['send_welcome'] = FALSE;
    $this->subscribe($list, $item);
  }

  /**
   * Unsubscribe a user, given a newsletter identifier and email address.
   *
   * Should ignore the request if there is no such subscription.
   */
  public function unsubscribe(NewsletterList $list, QueueItem $item) {
    $service = $this->factory->getClient('Recipient');
    $service->remove($list->identifier, $item->email);
    return TRUE;
  }

  /**
   * Apply transformations to map an attribute name to form keys.
   *
   * @param string $lname
   *   The attribute name.
   *
   * @return string
   *   The cleaned name.
   */
  protected static function cleanName($lname) {
    return strtr(drupal_clean_css_identifier(strtolower($lname)), '-', '_');
  }

  /**
   * Get the subscriber-data for a subscription object.
   */
  protected function attributeData($subscription, $old_data) {
    $list = $subscription->newsletterList();
    $names = [];
    $values = [];

    if ($source = $this->getCombinedSource($subscription, 'optivo', $old_data)) {
      foreach ($list->data->attributeNames as $lname) {
        $name = self::cleanName($lname);
        if ($value = $source->value($name)) {
          $names[] = $lname;
          $values[] = $value;
        }
      }
    }
    return ['names' => $names, 'values' => $values];
  }

  /**
   * {@inheritdoc}
   */
  public function data(Subscription $subscription, $old_data = NULL) {
    $data = $this->attributeData($subscription, $old_data);
    $fingerprint = sha1(serialize($data));
    return array($data, $fingerprint);
  }

  /**
   * Get a list of all optin processes for this account.
   */
  public function getOptinProcessOptions() {
    $service = $this->factory->getClient('OptinProcess');
    $ids = $service->getIds();
    $options = [];
    foreach ($ids as $id) {
      $options[$id] = $service->getName($id);
    }
    return $options;
  }

  /**
   * Get a source object for exporting data.
   */
  protected function getCombinedSource(Subscription $subscription, $target, $old_data) {
    $source = $this->getSource($subscription, $target);
    if ($old_data) {
      $names = array_map([self::class, 'cleanName'], $old_data['names']);
      $old_source = new ArraySource(array_combine($names, $old_data['values']));
      $source = new CombinedSource($source, $old_source);
    }
    return $source;
  }

  /**
   * Get subscriber polling object.
   */
  public function polling() {
    return SubscriberPolling::create('Optivo-' . $this->account, $this->factory);
  }

}
