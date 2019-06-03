<?php

namespace Drupal\campaignion_newsletters;

use Drupal\campaignion\Contact;
use Drupal\campaignion_newsletters\Subscription;
use Drupal\campaignion_newsletters\Subscriptions;

/**
 * Tests for redhen integration.
 */
class RedhenContactTest extends \DrupalUnitTestCase {

  /**
   * Contact without email does not set newsletters property.
   */
  public function testEmptyNewsletterSubscriptionsForRedhenContact() {
    // Contact without email
    $contact = new Contact();

    $form = array();
    $form['actions']['submit']['#submit'] = array();
    $form_state = array();
    $form_state['redhen_contact'] = $contact;

    campaignion_newsletters_form_redhen_contact_contact_form_alter($form, $form_state);

    $this->assertFalse(property_exists($form_state['redhen_contact'], 'newsletters'));
  }

  /**
   * Contact with known email address can have subscriptions.
   *
   * The newsletter property is set and the default values on subscriptions in
   * the form are set to the list id (when subscribed).
   */
  public function testNewsletterSubscriptionsForRedhenContact() {
    module_load_include('inc', 'redhen_contact', 'includes/redhen_contact.forms');

    $email = 'test5@example.com';
    $email_identifier = drupal_clean_css_identifier($email);
    $contact = Contact::fromBasicData($email);

    $form = drupal_get_form('redhen_contact_contact_form', $contact);
    $form_state = array();
    $form_state['redhen_contact'] = $contact;

    $list_id = 4711;
    $subscription = Subscription::byData($list_id, $email);
    $subscription->save();

    campaignion_newsletters_form_redhen_contact_contact_form_alter($form, $form_state);

    $this->assertInstanceOf(Subscriptions::class, $form_state['redhen_contact']->newsletters);

    $this->assertEqual($list_id, $form['newsletters_subscriptions'][$email_identifier]['#default_value'][$list_id]);

    $subscription->delete();
  }

}
