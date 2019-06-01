<?php
/**
 * @file
 * wetkit_splash.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function wetkit_splash_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'splashify_how_desktop_mode';
  $strongarm->value = 'redirect';
  $export['splashify_how_desktop_mode'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'splashify_how_desktop_size';
  $strongarm->value = '';
  $export['splashify_how_desktop_size'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'splashify_what_desktop_content';
  $strongarm->value = array(
    'value' => 'Welcome / Bienvenue',
    'format' => 'plain_text',
  );
  $export['splashify_what_desktop_content'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'splashify_what_desktop_mode';
  $strongarm->value = 'template';
  $export['splashify_what_desktop_mode'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'splashify_what_desktop_pagetitle';
  $strongarm->value = 'Drupal WxT';
  $export['splashify_what_desktop_pagetitle'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'splashify_when_anonymous';
  $strongarm->value = 1;
  $export['splashify_when_anonymous'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'splashify_when_desktop_frequency';
  $strongarm->value = 'once';
  $export['splashify_when_desktop_frequency'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'splashify_when_roles';
  $strongarm->value = FALSE;
  $export['splashify_when_roles'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'splashify_when_roles_options';
  $strongarm->value = array(
    'authenticated user' => 'authenticated user',
    'administrator' => 'administrator',
    'editor' => 'editor',
  );
  $export['splashify_when_roles_options'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'splashify_where_desktop_listpages';
  $strongarm->value = '';
  $export['splashify_where_desktop_listpages'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'splashify_where_desktop_opposite';
  $strongarm->value = 0;
  $export['splashify_where_desktop_opposite'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'splashify_where_desktop_page';
  $strongarm->value = 'home';
  $export['splashify_where_desktop_page'] = $strongarm;

  return $export;
}
