<?php
/**
 * @file.
 * Example for create event.
 */

// Get all events from TimePad.
$client = new TimePadApi();
$params = array(
  'name' => 'string',
  'starts_at' => '2015-09-26T06:21:14.243Z', // Date in format "Y-m-d\Th:i:sO"
  'ends_at' => '2015-09-26T06:21:14.243Z', // Date in format "Y-m-d\Th:i:sO"
  'description_short' => 'string',
  'description_html' => 'string',
  'organization' => array(
    'id' => 0,
    'subdomain' => 'example', // Name of your organization "example.timepad.ru".
  ),
  'ticket_types' => array(
    array(
      'price' => 0,
      'name' => 'string',
      'description' => 'string',
    )
  ),
  'questions' => array(), // For now it isn't worked.
  'categories' => array(
    array('name' => 'ИТ и интернет'),
  ),
  'location' => array(
    'city' => 'string',
    'address' => 'string',
  ),
  'poster_image_url' => 'string', // URL to image.
  'properties' => array(
    'string',
  ),
  'custom' => array(),
  'access_status' => 'draft', // Can be public, private, draft, link_only.
);
$result = $client->eventsCreate($params);
