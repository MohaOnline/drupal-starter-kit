<?php
/**
 * @file.
 * Example for create event.
 */

// Get all events from TimePad.
$token = ''; // For edit events token is necessary.
$client = new TimePadApi($token);
$params = array(
  'name' => 'Test edit',
  'description_short' => 'Test description',
  'description_html' => 'Test html description',
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
    'city' => 'Москва',
    'address' => 'string',
  ),
  'poster_image_url' => 'string', // URL to image.
  'properties' => array(
    'string',
  ),
  'custom' => array(),
  'access_status' => 'draft',
);
$event_id = 0;
$result = $client->eventsEdit($event_id, $params);
