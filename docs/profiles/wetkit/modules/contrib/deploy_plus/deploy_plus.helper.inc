<?php
/**
 * @file
 * Deploy Plus helper function.
 */

/**
 * Get entity dependencies. Support nodes and menu links for now.
 *
 * @param object $entity
 *   Entity object.
 * @param string $type
 *   Entity type.
 *
 * @return array
 *   Array of entities links.
 */
function deploy_plus_get_entity_dependencies($entity, $type) {
  $dependencies = array();

  $entity_dependencies = module_invoke_all('entity_dependencies', $entity, $type);
  // This is how we make sure to show a dependency only once.
  $traversed = array();

  foreach ($entity_dependencies as $dependency) {
    if (!empty($traversed[$dependency['type']][$dependency['id']])) {
      continue;
    }
    if (!isset($traversed[$dependency['type']])) {
      $traversed[$dependency['type']] = array();
    }

    $typeraversed[$dependency['type']][$dependency['id']] = 1;

    // Some ids are actually uuids, but others are serial ids.
    // We distinguish them by is_numeric check.
    if (is_numeric($dependency['id'])) {
      $dep_entity = entity_load($dependency['type'], array($dependency['id']));
    }
    else {
      $dep_entity = entity_uuid_load($dependency['type'], array($dependency['id']));
    }
    $dep_entity = reset($dep_entity);

    if ($dep_entity) {
      $title = deploy_plus_build_entity_title($dependency['type'], $dep_entity);
      $label = deploy_plus_entity_type_label($dependency['type'], $dep_entity);
      $dependencies[$label][] = $title;
    }
  }

  return $dependencies;
}

/**
 * Helper function to build title.
 *
 * @param string $type
 *   Entity type.
 * @param object $entity
 *   Entity object.
 *
 * @return string
 *   URL strign.
 */
function deploy_plus_build_entity_title($type, $entity) {
  $title = entity_label($type, $entity);
  $items = array();
  $ids = array(
    'nid',
    'tid',
    'mlid',
    'fid',
    'item_id',
    'field_name',
    'uid',
    'uuid',
    'link_path',
    'menu_name',
    'vocabulary_machine_name',
  );
  foreach ($ids as $key) {
    $value = FALSE;
    if (is_object($entity) && isset($entity->{$key})) {
      $value = $entity->{$key};
    }
    elseif (is_array($entity) && isset($entity[$key])) {
      $value = $entity[$key];
    }
    if ($value) {
      switch ($key) {
        case 'menu_name':
          $menus = menu_get_menus();
          $value = $menus[$value];
          break;

        case 'vocabulary_machine_name':
          $vocabulary = taxonomy_vocabulary_machine_name_load($value);
          $value = $vocabulary->name;
          $key = 'vocabulary_name';
          break;

        default:
          break;
      }
      $items[] = t('@key: !value', array('@key' => $key, '!value' => $value));
    }
  }

  if ($type == 'file') {
    $url = is_array($entity) ? $entity['uri'] : $entity->uri;
    $title = l($title, file_create_url($url), array(
      'attributes' => array('target' => '_blank'),
    ));
  }
  else {
    $uri = entity_uri($type, $entity);
    if ($uri) {
      $uri['options']['attributes']['target'] = '_blank';
      $title = l($title, $uri['path'], $uri['options']);
    }
  }
  return $title . theme('item_list', array('items' => $items));
}

/**
 * Get entity type label.
 *
 * @param string $type
 *   Entity type.
 * @param object $entity
 *   Entity object.
 *
 * @return string
 *   Entity type human readable name.
 */
function deploy_plus_entity_type_label($type, $entity) {
  $label = '';
  if ($type == 'node') {
    $wrapped = entity_metadata_wrapper($type, $entity);
    $label = $wrapped->type->label();
  }
  else {
    $info = entity_get_info($type);
    $label = $info['label'];
  }
  return $label;
}
