<?php
/**
 * @file
 * implements NewsletterProvider using CleverReach API.
 *
 * See http://api.cleverreach.com/soap/doc/5.0/ for documentation.
 */

namespace Drupal\campaignion_newsletters_cleverreach;

use \Drupal\campaignion\CRM\Import\Source\ArraySource;
use \Drupal\campaignion\CRM\Import\Source\CombinedSource;

use \Drupal\campaignion_newsletters\ApiError;
use \Drupal\campaignion_newsletters\ApiPersistentError;
use \Drupal\campaignion_newsletters\FormSubmission;
use \Drupal\campaignion_newsletters\NewsletterList;
use \Drupal\campaignion_newsletters\ProviderBase;
use \Drupal\campaignion_newsletters\QueueItem;
use \Drupal\campaignion_newsletters\Subscription;

class CleverReach extends ProviderBase {
  protected $account;
  protected $api;

  /**
   * Construct a new instance from config parameters.
   */
  public static function fromParameters(array $params) {
    $api = new ApiClient($params['key']);
    return new static($api, $params['name']);
  }

  /**
   * Constructor.
   *
   * @param \Drupal\campaignion_newsletters_cleverreach\ApiClient $api
   *   Instance of our SoapClient sub-class.
   * @param string $name
   *   Name of this CleverReach account.
   */
  public function __construct(ApiClient $api, $name) {
    $this->account = $name;
    $this->api = $api;
  }

  /**
   * Fetches current lists from the provider.
   *
   * @return array
   *   An array of associative array
   *   (properties: identifier, title, source, language).
   */
  public function getLists() {
    $lists = array();
    $groups = $this->listGroups();
    foreach ($groups as $group) {
      $details = $this->getGroupDetails($group);
      $details->forms = $this->getForms($group);
      $id = $this->toIdentifier($details->name);
      $lists[] = NewsletterList::fromData(array(
        'identifier' => $id,
        'title'      => $details->name,
        'source'     => 'CleverReach-' . $this->account,
        'data'       => $details,
        // @TODO: find a way to get an actual list specific language.
      ));
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
    $page = 0;
    $receivers = array();

    $group_id = $list->data->id;

    do {
      $result = $this->api->receiverGetPage($group_id,
                array(
                  'page'   => $page++,
                  'filter' => 'active',
                ));
      if ($result->message == 'data not found') {
        return $receivers;
      }
      else {
        $new_receivers = $this->handleResult($result);
        foreach ($new_receivers as $new_receiver) {
          $receivers[] = $new_receiver->email;
        }
      }
    } while ($new_receivers);
    return $receivers;
  }

  /**
   * Get a source object for exporting data.
   */
  protected function getCombinedSource(Subscription $subscription, $target, $old_data) {
    $source = $this->getSource($subscription, $target);
    if ($old_data) {
      $data = [];
      foreach ($old_data as $i) {
        $data[$i['key']] = $i['value'];
      }
      $old_source = new ArraySource($data);
      $source = new CombinedSource($source, $old_source);
    }
    return $source;
  }

  protected function attributeData(Subscription $subscription, $old_data) {
    $list = $subscription->newsletterList();
    $attributes = array();

    $listAttributes = array_merge($list->data->attributes, $list->data->globalAttributes);
    if ($source = $this->getCombinedSource($subscription, 'cleverreach', $old_data)) {
      foreach ($listAttributes as $attribute) {
        if ($value = $source->value($attribute->key)) {
          $attributes[] = array(
            'key' => $attribute->key,
            'value' => $value,
          );
        }
      }
    }
    return $attributes;
  }

  public function data(Subscription $subscription, $old_data) {
    $data = $this->attributeData($subscription, $old_data);
    $attr = $data;
    unset($attr['updated']);
    $fingerprint = sha1(serialize($attr));
    return array($data, $fingerprint);
  }

  /**
   * Subscribe a user, given a newsletter identifier and email address.
   *
   * @return: True on success.
   */
  public function subscribe(NewsletterList $list, QueueItem $item) {
    $mail = $item->email;
    $opt_in = $item->optIn();
    $user = array(
      'email'  => $mail,
      'attributes' => $item->data,
      'active' => !$opt_in,
      'activated' => $opt_in ? FALSE : $item->created,
    );
    $group_id = $list->data->id;
    $result = $this->api->receiverGetByEmail($group_id, $mail, 0);
    if ($result->message === 'data not found') {
      $user['registered'] = $item->created;
      $result = $this->api->receiverAdd($group_id, $user);
    }
    else {
      $result = $this->api->receiverUpdate($group_id, $user);
    }
    $continue = $this->handleResult($result);
    if ($continue && $opt_in && ($form_id = $this->getFormId($list))) {
      if (!$item->optin_info) {
        throw new ApiPersistentError('CleverReach', "Unable to send action email without opt-in data.");
      }
      $doidata = $this->formatDOIData($item->optin_info);
      $result = $this->api->formsSendActivationMail($form_id, $mail, $doidata);
      return (bool) $this->handleResult($result);
    }
    return $continue;
  }

  protected function formatDOIData($optin_info) {
    $postdata = [];
    foreach ($optin_info->data as $n => $data) {
      $postdata[] = "$n:$data";
    }
    $doidata = [
      'user_ip' => $optin_info->ip,
      'user_agent' => $optin_info->userAgent,
      'referer' => $optin_info->url,
      'postdata' => implode(',', $postdata),
      'info' => format_string('campaignion_newsletters on @site', ['@site' => variable_get('site_name', '')]),
    ];
    return $doidata;
  }

  protected function getFormId(NewsletterList $list) {
    return !empty($list->data->forms) ? $list->data->forms[0]->id : NULL;
  }

  /**
   * Unsubscribe a user, given a newsletter identifier and email address.
   *
   * Should ignore the request if there is no such subscription.
   *
   * @return: True on success.
   */
  public function unsubscribe(NewsletterList $list, QueueItem $item) {
    $result = $this->api->receiverDelete($list->data->id, $item->email);
    return (bool) $this->handleResult($result);
  }

  /**
   * Fetches a list of groups (without details). Called by the constructor.
   */
  protected function listGroups() {
    $data = $this->handleResult($this->api->groupGetList());
    $return = array();
    if ($data !== FALSE) {
      foreach ($data as $group) {
        $identifier = $this->toIdentifier($group->name);
        $return[$identifier] = $group;
      }
      return $return;
    }
    else {
      return array();
    }
  }

  /**
   * Fetches details for a single, given group.
   */
  protected function getGroupDetails($group) {
    $result = $this->api->groupGetDetails($group->id);
    return $this->handleResult($result);
  }

  /**
   * Get a list of all forms for this group.
   */
  protected function getForms($group) {
    $result = $this->api->formsGetList($group->id);
    return $this->handleResult($result);
  }

  /**
   * Handles errors if any, extracts data if not.
   */
  protected function handleResult($result) {
    if ($result->status !== 'SUCCESS') {
      $b = 'CleverReach';
      $args = array(
        '@status' => $result->status,
        '@message' => $result->message,
      );
      switch ($result->statuscode) {
        case 20:
        case 40:
          throw new ApiPersistentError($b, '@status #@code @message - removing item from queue.', $args, $result->statuscode);
        default:
          throw new ApiError($b, '@status #@code: @message', $args, $result->statuscode);
      }
    }
    return $result->data;
  }

  /**
   * Helper to create unified identifiers for newsletters.
   */
  public function toIdentifier($string) {
    return strtolower(drupal_clean_css_identifier($string));
  }

  /**
   * Protected clone method to prevent cloning of the singleton instance.
   */
  protected function __clone() {}

  /**
   * Protected unserialize method to prevent unserializing of singleton.
   */
  protected function __wakeup() {}
}
