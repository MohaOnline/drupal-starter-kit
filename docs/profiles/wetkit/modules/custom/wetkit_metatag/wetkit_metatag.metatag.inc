<?php
/**
 * @file
 * Metatag integration for the wetkit_metatag module.
 */

/**
 * Implements hook_metatag_config_default_alter().
 */
function wetkit_metatag_metatag_config_default_alter(array &$configs) {
  foreach ($configs as &$config) {
    switch ($config->instance) {
      case 'global':
        $config->config += array(
          'dcterms.creator' => array('value' => '[current-page:creator_default_value]'),
          'dcterms.language' => array('value' => '[current-page:language_default_value]'),
          'dcterms.subject' => array('value' => '[current-page:bilingual_default_value]'),
          'dcterms.title' => array('value' => '[current-page:title]'),
          'dcterms:service' => array('value' => NULL),
          'dcterms:accessRights' => array('value' => '2'),
        );
        break;

      case 'global:frontpage':
        $config->config += array(
          'dcterms:service' => array('value' => NULL),
          'dcterms:accessRights' => array('value' => '2'),
          'dcterms.title' => array('value' => '[site:name]'),
        );
        break;

      case 'frontpage':
        $config->config += array(
          'dcterms.title' => array('value' => '[site:name]'),
          'dcterms:service' => array('value' => NULL),
          'dcterms:accessRights' => array('value' => '2'),
        );
        break;

      case 'node':
        $config->config += array(
          'dcterms.title' => array('value' => '[node:title]'),
          'dcterms.issued' => array('value' => '[node:bilingual_date_issued]'),
          'dcterms.modified' => array('value' => '[node:bilingual_date_published]'),
          'dcterms.language' => array('value' => '[node:bilingual_prefix]'),
          'dcterms.creator' => array('value' => '[node:improved_creator]'),
          'dcterms.description' => array('value' => '[node:improved_summary]'),
          'metatags.description' => array('value' => '[node:improved_summary]'),
          'dcterms:service' => array('value' => NULL),
          'dcterms:accessRights' => array('value' => '2'),
        );
        break;

      case 'panels':
        $config->config += array(
          'dcterms:service' => array('value' => NULL),
          'dcterms:accessRights' => array('value' => '2'),
          );
        break;

      case 'taxonomy_term':
        $config->config += array(
          'dcterms.title' => array('value' => '[term:name]'),
          'dcterms.description' => array('value' => '[term:description]'),
        );
        break;

      case 'user':
        $config->config += array(
          'dcterms.title' => array('value' => '[user:name]'),
          'dcterms.date' => array('value' => '[user:created:custom:Y-m-d]'),
          'dcterms.creator' => array('value' => '[user:name]'),
        );
        break;
    }
  }
}

/**
 * Implements hook_metatag_info().
 */
function wetkit_metatag_metatag_info() {

  // Native Groupings.
  $info['groups']['defaults'] = array(
    'label' => t('Defaults'),
    'description' => t('Default grouping for meta tags.'),
    'form' => array(
      '#weight' => 40,
    ),
  );
  $info['groups']['dublin-core'] = array(
    'label' => t('Dublin Core'),
    'description' => t('Information about Dublin Core can be found at: <a href="http://dublincore.org/documents/dcmi-terms">dublincore.org/documents/dcmi-terms</a>.'),
    'form' => array(
      '#weight' => 50,
    ),
  );

  // Native Tags.
  $info['tags']['dcterms.title'] = array(
    'label' => t('Dublin Core Title'),
    'description' => t('Information about this element can be found at: <a href="http://purl.org/dc/elements/1.1/title">purl.org/dc/elements/1.1/title</a>'),
    'class' => 'DrupalTextMetaTag',
    'group' => 'dublin-core',
    'element' => array(
      '#type' => 'term',
      '#theme' => 'wetkit_metatag',
    ),
  );
  $info['tags']['dcterms.creator'] = array(
    'label' => t('Dublin Core Creator'),
    'description' => t('Information about this element can be found at: <a href="http://purl.org/dc/elements/1.1/creator">purl.org/dc/elements/1.1/creator</a>'),
    'group' => 'dublin-core',
    'class' => 'DrupalTextMetaTag',
    'element' => array(
      '#theme' => 'wetkit_metatag',
    ),
  );
  $info['tags']['dcterms.subject'] = array(
    'label' => t('Dublin Core Subject'),
    'description' => t('Information about this element can be found at: <a href="http://purl.org/dc/elements/1.1/subject">purl.org/dc/elements/1.1/subject</a>'),
    'class' => 'DrupalTextMetaTag',
    'group' => 'dublin-core',
    'element' => array(
      '#theme' => 'wetkit_metatag',
      '#schema' => variable_get('wetkit_metatag_subject_scheme'),
    ),
  );
  $info['tags']['dcterms.description'] = array(
    'label' => t('Dublin Core Description'),
    'description' => t('Information about this element can be found at: <a href="http://purl.org/dc/elements/1.1/description">purl.org/dc/elements/1.1/description</a>'),
    'group' => 'dublin-core',
    'class' => 'DrupalTextMetaTag',
    'element' => array(
      '#theme' => 'wetkit_metatag',
    ),
  );
  $info['tags']['dcterms.issued'] = array(
    'label' => t('Dublin Core Issued Date'),
    'description' => t('Information about this element can be found at: <a href="http://purl.org/dc/elements/1.1/issued">purl.org/dc/elements/1.1/issued</a>'),
    'class' => 'DrupalTextMetaTag',
    'group' => 'dublin-core',
    'element' => array(
      '#theme' => 'wetkit_metatag',
      '#schema' => 'W3CDTF',
    ),
  );
  $info['tags']['dcterms.modified'] = array(
    'label' => t('Dublin Core Modified Date'),
    'description' => t('Information about this element can be found at: <a href="http://purl.org/dc/elements/1.1/modified">purl.org/dc/elements/1.1/modified</a>'),
    'class' => 'DrupalTextMetaTag',
    'group' => 'dublin-core',
    'element' => array(
      '#theme' => 'wetkit_metatag',
      '#schema' => 'W3CDTF',
    ),
  );
  $info['tags']['dcterms.language'] = array(
    'label' => t('Dublin Core Language'),
    'description' => t('Information about this element can be found at: <a href="http://purl.org/dc/elements/1.1/language">purl.org/dc/elements/1.1/language</a>'),
    'class' => 'DrupalTextMetaTag',
    'group' => 'dublin-core',
    'element' => array(
      '#theme' => 'wetkit_metatag',
      '#schema' => 'ISO639-2',
    ),
  );
  $info['tags']['dcterms:accessRights'] = array(
    'label' => t('Dublin Core Access Rights'),
    'description' => t('Information about this element can be found at: <a href="http://purl.org/dc/terms/accessRights">http://purl.org/dc/terms/accessRights</a>'),
    'class' => 'DrupalTextMetaTag',
    'group' => 'dublin-core',
    'element' => array(
      '#theme' => 'wetkit_metatag_property',
     ),
  );
  $info['tags']['dcterms:service'] = array(
    'label' => t('Dublin Core Service'),
    'description' => t('Information about this element can be found at: <a href="http://purl.org/dc/dcmitype/Service">http://purl.org/dc/dcmitype/Service</a>'),
    'class' => 'DrupalTextMetaTag',
    'group' => 'dublin-core',
    'element' => array(
      '#theme' => 'wetkit_metatag_property',
    ),
  );

  return $info;
}

/**
 * Implements hook_metatag_info_alter().
 */
function wetkit_metatag_metatag_info_alter(&$info) {
  // Cleaner groupings.
  $info['tags']['description']['group'] = 'defaults';
  $info['tags']['keywords']['group'] = 'defaults';
  $info['tags']['robots']['group'] = 'defaults';
  $info['tags']['title']['group'] = 'defaults';

  // Remove tags we don't need to leverage.
  unset($info['tags']['abstract']);
  unset($info['tags']['author']);
  unset($info['tags']['copyright']);
  unset($info['tags']['generator']);
  unset($info['tags']['image_src']);
  unset($info['tags']['news_keywords']);
  unset($info['tags']['original-source']);
  unset($info['tags']['publisher']);
  unset($info['tags']['revisit-after']);
  unset($info['tags']['robots']['form']['#options']['noodp']);
  unset($info['tags']['robots']['form']['#options']['noydir']);
  unset($info['tags']['shortlink']);
  unset($info['tags']['standout']);

  if (variable_get('wetkit_metatag_canonical', TRUE)) {
    unset($info['tags']['canonical']);
  }
  else {
    $info['tags']['canonical']['group'] = 'defaults';
  }
}
