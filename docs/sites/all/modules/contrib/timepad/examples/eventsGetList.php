<?php
/**
 * @file.
 * Examples to get events list.
 */

// Get all events from TimePad.
$client = new TimePadApi();
$result = $client->eventsGetList();
print $result['total'];
foreach ($result['values'] as $event) {
  var_dump($event);
}

// Get filtered list of events.
$client = new TimePadApi();
$params = array(
  'fields' => array(
    'created_at',
    'ends_at',
    'description_short',
    'description_html',
    'ad_partner_percent',
    'locale',
    'location',
    'organization',
    'ticket_types',
    'questions',
    'widgets',
    'properties',
    'access_status',
    'registration_data',
  ),
  'limit' => 20, // By default is 10, max 20.
  'skip' => 1,
  'sort' => '+name', // Allowed fields: name, starts_at, city, referrer_percent, created_at, id.
  'category_ids' => array(
    452, // Example: IT and internet.
  ),
  'category_ids_exclude' => array(),
  'cities' => array(
    'Москва',
  ),
  'cities_exclude' => array(),
  'organization_ids' => array(),
  'organization_ids_exclude' => array(),
  'event_ids' => array(),
  'event_ids_exclude' => array(),
  'keywords' => array(
    'Drupal',
  ),
  'keywords_exclude' => array(),
  'access_statuses' => array( // Access only for organization.
    'public',
    'private',
    'draft',
    'link_only',
  ),
  'moderation_statuses' => array(
    'featured',
    'shown',
    'hidden',
    'not_moderated',
  ),
  'price_min' => 0, // Price min in RUR.
  'price_max' => 1000, // Price min in RUR.
  'ad_partner_percent_min' => 0,
  'ad_partner_percent_max' => 15,
  'ad_partner_profit_min' => 2000, // Profit min in RUR.
  'ad_partner_profit_max' => '',   // Profit max in RUR.
  'starts_at_min' => '2015-09-26T15:00:00+0300', // Date in format "Y-m-d\Th:i:sO"
  'starts_at_max' => '', // Date in format "Y-m-d\Th:i:sO"
  'created_at_min' => '', // Date in format "Y-m-d\Th:i:sO"
  'created_at_max' => '', // Date in format "Y-m-d\Th:i:sO"
);
$result = $client->eventsGetList($params);
print $result['total'];
foreach ($result['values'] as $event) {
  var_dump($event);
}
