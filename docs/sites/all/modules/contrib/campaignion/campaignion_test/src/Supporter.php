<?php

namespace Drupal\campaignion_test;

use \Drupal\campaignion\ContactTypeInterface;
use \Drupal\campaignion\CRM\Import\Field;
use \Drupal\campaignion\CRM\ImporterBase;

use \Drupal\campaignion\CRM\Export\SingleValueField;
use \Drupal\campaignion\CRM\Export\WrapperField;
use \Drupal\campaignion\CRM\Export\MappedWrapperField;
use \Drupal\campaignion\CRM\Export\AddressField;
use \Drupal\campaignion\CRM\Export\DateField;
use \Drupal\campaignion\CRM\Export\KeyedField;
use \Drupal\campaignion\CRM\Export\TagsField;
use \Drupal\campaignion\CRM\ExporterBase;

class Supporter implements ContactTypeInterface {
  public function __construct() {}

  public function importer($type) {
    $mappings = array(
      new Field\Name('first_name'),
      new Field\Name('last_name'),
    );
    if ($type == 'campaignion_action_taken') {
      $mappings = array_merge($mappings, array(
        new Field\Field('field_gender',     'gender'),
        new Field\Field('field_salutation', 'salutation'),
        new Field\Field('field_title',      'title'),
        new Field\Date('field_date_of_birth',    'date_of_birth'),
        new Field\Address('field_address', array(
          'thoroughfare'        => 'street_address',
          'postal_code'         => ['zip_code', 'postcode'],
          'locality'            => 'city',
          'administrative_area' => 'state',
          'country'             => 'country',
        )),
        new Field\Phone('field_phone_number', 'phone_number'),
        new Field\Phone('field_phone_number', 'mobile_number'),
        new Field\EmailBulk('redhen_contact_email', 'email', 'email_newsletter'),
        new Field\Field('field_direct_mail_newsletter', 'direct_mail_newsletter'),
        new Field\Field('field_preferred_language', 'language'),
      ));
    }
    return new ImporterBase($mappings);
  }

  public function exporter($type, $language) {
    $map = array();
    switch ($type) {
      case 'cleverreach':
        $map['email'] = new WrapperField('email');
        $map['salutation'] = new WrapperField('field_salutation');
        $map['firstname'] = new SingleValueField('first_name');
        $map['lastname'] = new SingleValueField('last_name');
        $map['title'] = new WrapperField('field_title');
        $map['gender'] = new WrapperField('field_gender');
        $map['date_of_birth'] = new DateField('field_date_of_birth', '%Y-%m-%d');
        $map['street'] = new KeyedField('field_address', 'thoroughfare');
        $map['country'] = new KeyedField('field_address', 'country');
        $map['zip'] = new KeyedField('field_address', 'postal_code');
        $map['city'] = new KeyedField('field_address', 'locality');
        $map['region'] = new KeyedField('field_address', 'administrative_area');
        $map['language'] = new WrapperField('field_preferred_language');
        $map['created'] = new DateField('created', '%Y-%m-%d');
        $map['updated'] = new DateField('updated', '%Y-%m-%d');
        $map['tags'] = new TagsField('supporter_tags', TRUE);
        break;
      case 'mailchimp':
        $map['EMAIL'] = new WrapperField('email');
        $map['FNAME'] = new SingleValueField('first_name');
        $map['LNAME'] = new SingleValueField('last_name');
        $map['SALUTATION'] = new WrapperField('field_salutation');
        $map['TITLE'] = new WrapperField('field_title');
        $map['GENDER'] = new WrapperField('field_gender');
        $map['DATE_OF_BIRTH'] = new DateField('field_date_of_birth', '%Y-%m-%d');
        $map['STREET'] = new KeyedField('field_address', 'thoroughfare');
        $map['COUNTRY'] = new KeyedField('field_address', 'country');
        $map['ZIP'] = new KeyedField('field_address', 'postal_code');
        $map['CITY'] = new KeyedField('field_address', 'locality');
        $map['REGION'] = new KeyedField('field_address', 'administrative_area');
        $map['LANGUAGE'] = new WrapperField('field_preferred_language');
        $map['CREATED'] = new DateField('created', '%Y-%m-%d');
        $map['UPDATED'] = new DateField('updated', '%Y-%m-%d');
        $map['TAGS'] = new TagsField('supporter_tags');
        break;
      case 'dadiapi':
        $map['email'] = new WrapperField('email');
        $map['vorname'] = new SingleValueField('first_name');
        $map['name'] = new SingleValueField('last_name');
        $map['titel'] = new WrapperField('field_title');
        $genderMap = array('m' => 'M', 'f' => 'W');
        $map['geschlecht'] = new MappedWrapperField('field_gender', $genderMap);
        $map['geburtsdatum'] = new DateField('field_date_of_birth', '%Y%m%d');
        $map['strasse'] = new KeyedField('field_address', 'thoroughfare');
        $map['land'] = new KeyedField('field_address', 'country');
        $map['plz'] = new KeyedField('field_address', 'postal_code');
        $map['ort'] = new KeyedField('field_address', 'locality');
        break;
      case 'campaignion_manage':
        $address_mapping = array(
          'street'  => 'thoroughfare',
          'country' => 'country',
          'zip'     => 'postal_code',
          'city'    => 'locality',
          'region'  => 'administrative_area',
        );
        $map['redhen_contact_email']         = new WrapperField('email');
        $map['field_salutation']             = new WrapperField('field_salutation');
        $map['first_name']                   = new SingleValueField('first_name');
        $map['middle_name']                  = new SingleValueField('middle_name');
        $map['last_name']                    = new SingleValueField('last_name');
        $map['field_title']                  = new WrapperField('field_title');
        $map['field_gender']                 = new WrapperField('field_gender');
        $map['field_date_of_birth']          = new DateField('field_date_of_birth', '%Y-%m-%d');
        $map['field_address']                = new AddressField('field_address', $address_mapping);
        $map['created']                      = new DateField('created', '%Y-%m-%d');
        $map['updated']                      = new DateField('updated', '%Y-%m-%d');
        $map['field_ip_adress']              = new WrapperField('field_ip_adress');
        $map['field_phone_number']           = new WrapperField('field_phone_number');
        $map['field_direct_mail_newsletter'] = new WrapperField('field_direct_mail_newsletter');
        $map['field_social_network_links']   = new WrapperField('field_social_network_links');
        $map['supporter_tags']               = new TagsField('supporter_tags');
        $map['field_preferred_language']     = new WrapperField('field_preferred_language');
        break;
    }
    if ($map) {
      return new ExporterBase($map);
    }
  }
}
