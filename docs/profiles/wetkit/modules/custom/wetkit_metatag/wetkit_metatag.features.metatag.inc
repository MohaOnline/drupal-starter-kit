<?php
/**
 * @file
 * wetkit_metatag.features.metatag.inc
 */

/**
 * Implements hook_metatag_export_default().
 */
function wetkit_metatag_metatag_export_default() {
  $config = array();

  // Exported Metatag config instance: global.
  $config['global'] = array(
    'instance' => 'global',
    'disabled' => FALSE,
    'config' => array(
      'title' => array(
        'value' => '[current-page:title] | [site:name]',
      ),
      'generator' => array(
        'value' => 'Drupal 7 (http://drupal.org)',
      ),
      'canonical' => array(
        'value' => '[current-page:url:absolute]',
      ),
      'shortlink' => array(
        'value' => '[current-page:url:unaliased]',
      ),
      'dcterms.creator' => array(
        'value' => '[current-page:creator_default_value]',
      ),
      'dcterms.language' => array(
        'value' => '[current-page:language_default_value]',
      ),
      'dcterms.subject' => array(
        'value' => '[current-page:bilingual_default_value]',
      ),
      'dcterms.title' => array(
        'value' => '[current-page:title]',
      ),
    ),
  );

  // Exported Metatag config instance: global:frontpage.
  $config['global:frontpage'] = array(
    'instance' => 'global:frontpage',
    'disabled' => FALSE,
    'config' => array(
      'title' => array(
        'value' => '[site:name]',
      ),
      'canonical' => array(
        'value' => '[site:url]',
      ),
      'shortlink' => array(
        'value' => '[site:url]',
      ),
      'dcterms.title' => array(
        'value' => '[site:name]',
      ),
      'dcterms.identifier' => array(
        'value' => '[site:url]',
      ),
    ),
  );

  // Exported Metatag config instance: node.
  $config['node'] = array(
    'instance' => 'node',
    'disabled' => FALSE,
    'config' => array(
      'title' => array(
        'value' => '[node:title] | [site:name]',
      ),
      'description' => array(
        'value' => '[node:summary]',
      ),
      'dcterms.title' => array(
        'value' => '[node:title]',
      ),
      'dcterms.creator' => array(
        'value' => '[node:improved_creator]',
      ),
      'dcterms.description' => array(
        'value' => '[node:improved_summary]',
      ),
      'dcterms.issued' => array(
        'value' => '[node:bilingual_date_issued]',
      ),
      'dcterms.modified' => array(
        'value' => '[node:bilingual_date_published]',
      ),
      'dcterms.identifier' => array(
        'value' => '[site:url]/node/[node:nid]',
      ),
      'dcterms.language' => array(
        'value' => '[node:bilingual_prefix]',
      ),
    ),
  );

  // Exported Metatag config instance: taxonomy_term.
  $config['taxonomy_term'] = array(
    'instance' => 'taxonomy_term',
    'disabled' => FALSE,
    'config' => array(
      'title' => array(
        'value' => '[term:name] | [site:name]',
      ),
      'description' => array(
        'value' => '[term:description]',
      ),
      'dcterms.title' => array(
        'value' => '[term:name]',
      ),
      'dcterms.identifier' => array(
        'value' => '[current-page:url:absolute]',
      ),
      'dcterms.description' => array(
        'value' => '[term:description]',
      ),
    ),
  );

  // Exported Metatag config instance: user.
  $config['user'] = array(
    'instance' => 'user',
    'disabled' => FALSE,
    'config' => array(
      'title' => array(
        'value' => '[user:name] | [site:name]',
      ),
      'dcterms.title' => array(
        'value' => '[user:name]',
      ),
      'dcterms.date' => array(
        'value' => '[user:created:custom:Y-m-d]',
      ),
      'dcterms.identifier' => array(
        'value' => '[current-page:url:absolute]',
      ),
      'dcterms.creator' => array(
        'value' => '[user:name]',
      ),
    ),
  );

  return $config;
}
