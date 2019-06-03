<?php
/**
 * @file
 * Entity related code for the Topical feature.
 */

/**
 * Implements hook_entity_info_alter().
 */
function dvg_topical_entity_info_alter(&$entity_info) {
  $entity_info['node']['view modes']['topical'] = array(
    'label' => t('Topical'),
    'custom settings' => FALSE,
  );
}
