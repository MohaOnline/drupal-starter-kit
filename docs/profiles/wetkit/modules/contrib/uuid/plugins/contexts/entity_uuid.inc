<?php
/**
 * @file
 *
 * Plugin to provide an entity derived context.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Entity UUID"),
  'description' => t('Entity object based on UUID context.'),
  'context' => 'uuid_context_create_entity_uuid',
  'edit form' => 'uuid_context_entity_uuid_settings_form',
  'defaults' => array('uuid' => ''),
  'convert list' => 'uuid_context_entity_uuid_convert_list',
  'convert' => 'uuid_context_entity_uuid_convert',
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter the UUID of an entity for this context.'),
  ),
  'get child' => 'uuid_context_entity_uuid_get_child',
  'get children' => 'uuid_context_entity_uuid_get_children',
);

function uuid_context_entity_uuid_get_child($plugin, $parent, $child) {
  $plugins = uuid_context_entity_uuid_get_children($plugin, $parent);
  return $plugins[$parent . ':' . $child];
}

function uuid_context_entity_uuid_get_children($plugin, $parent) {
  $entities = entity_get_info();
  $plugins = array();
  foreach ($entities as $entity_type => $entity) {
    if (isset($entity['uuid'])) {
      $child_plugin = $plugin;
      $child_plugin['title'] = t('@entity (UUID)', array('@entity' => $entity['label']));
      $child_plugin['keyword'] = $entity_type;
      $child_plugin['context name'] = $entity_type;
      $child_plugin['name'] = $parent . ':' . $entity_type;
      $child_plugin['description'] = t('Creates @entity context from an entity UUID.', array('@entity' => $entity_type));
      $child_plugin_id = $parent . ':' . $entity_type;
      drupal_alter('ctools_entity_context', $child_plugin, $entity, $child_plugin_id);
      $plugins[$child_plugin_id] = $child_plugin;
    }
  }
  drupal_alter('ctools_entity_contexts', $plugins);
  return $plugins;
}

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function uuid_context_create_entity_uuid($empty, $data = NULL, $conf = FALSE, $plugin) {
  $entity_type = $plugin['keyword'];
  $entity = entity_get_info($entity_type);
  $context = new ctools_context(array('entity:' . $entity_type, 'entity', $entity_type));
  $context->plugin = $plugin['name'];
  $context->keyword = $entity_type;

  if ($empty) {
    return $context;
  }

  // Attempt to retain compatibility with broken uuid:
  if (is_array($data) && isset($data['uuid'])) {
    $uuid = $data['uuid'];
  }
  elseif (is_array($data) && !isset($data['entity_id']) && isset($data['id'])) {
    $id = $data['id'];
    $uuids = entity_get_uuid_by_id($entity_type, array($data['id']));
    $uuid = !empty($uuids[$id]) ? $uuids[$id] : FALSE;
  }
  elseif (is_object($data)) {
    $ids = entity_extract_ids($entity_type, $data);
    $id = $ids[0];
    $uuids = entity_get_uuid_by_id($entity_type, array($id));
    $uuid = !empty($uuids[$id]) ? $uuids[$id] : FALSE;
  }
  elseif (uuid_is_valid($data)) {
    $uuid = $data;
    $data = entity_uuid_load($entity_type, array($uuid));
    $data = reset($data);
  }

  if (is_array($data)) {
    $data = entity_uuid_load($entity_type, array($uuid));
    $data = reset($data);
  }
  elseif(!is_object($data)) {
    $data = FALSE;
  }

  if (!empty($data)) {
    $context->data = $data;
    if (!empty($entity['entity keys']['label'])) {
      $context->title = $data->{$entity['entity keys']['label']};
    }
    $id = $data->{$entity['entity keys']['id']};
    $uuids = entity_get_uuid_by_id($entity_type, array($id));
    $context->argument = !empty($uuids[$id]) ? $uuids[$id] : NULL;

    if ($entity['entity keys']['bundle']) {
      $context->restrictions['type'] = array($data->{$entity['entity keys']['bundle']});
    }
    return $context;
  }
}

function uuid_context_entity_uuid_settings_form($form, &$form_state) {
  $conf = &$form_state['conf'];
  $plugin = &$form_state['plugin'];

  $form['entity'] = array(
    '#title' => t('Enter the title or UUID of a @entity entity', array('@entity' => $plugin['keyword'])),
    '#type' => 'textfield',
    '#maxlength' => 512,
    '#autocomplete_path' => 'uuid/autocomplete/' . $plugin['keyword'],
    '#weight' => -10,
  );

  if (!empty($conf['uuid'])) {
    $info = entity_uuid_load($plugin['keyword'], array($conf['uuid']));
    $info = reset($info);
    if ($info) {
      $entity = entity_get_info($plugin['keyword']);
      $uri = entity_uri($plugin['keyword'], $info);
      if (is_array($uri) && $entity['entity keys']['label']) {
        $link = l(t("'%title' [%type uuid %uuid]", array('%title' => $info->{$entity['entity keys']['label']}, '%type' => $plugin['keyword'], '%uuid' => $conf['uuid'])), $uri['path'], array('attributes' => array('target' => '_blank', 'title' => t('Open in new window')), 'html' => TRUE));
      }
      elseif (is_array($uri)) {
        $link = l(t("[%type uuid %uuid]", array('%type' => $plugin['keyword'], '%uuid' => $conf['uuid'])), $uri['path'], array('attributes' => array('target' => '_blank', 'title' => t('Open in new window')), 'html' => TRUE));
      }
      elseif ($entity['entity keys']['label']) {
        $link = l(t("'%title' [%type uuid %uuid]", array('%title' => $info->{$entity['entity keys']['label']}, '%type' => $plugin['keyword'], '%uuid' => $conf['uuid'])), file_create_url($uri), array('attributes' => array('target' => '_blank', 'title' => t('Open in new window')), 'html' => TRUE));
      }
      else {
        $link = t("[%type uuid %uuid]", array('%type' => $plugin['keyword'], '%uuid' => $conf['uuid']));
      }
      $form['entity']['#description'] = t('Currently set to !link', array('!link' => $link));
    }
  }

  $form['uuid'] = array(
    '#type' => 'value',
    '#value' => $conf['uuid'],
  );

  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => $plugin['keyword'],
  );

  $form['set_identifier'] = array(
    '#type' => 'checkbox',
    '#default_value' => FALSE,
    '#title' => t('Reset identifier to entity label'),
    '#description' => t('If checked, the identifier will be reset to the entity label of the selected entity.'),
  );

  return $form;
}

/**
 * Validate an entity.
 */
function uuid_context_entity_uuid_settings_form_validate($form, &$form_state) {
  // Validate the autocomplete
  if (empty($form_state['values']['uuid']) && empty($form_state['values']['entity'])) {
    form_error($form['entity'], t('You must select an entity.'));
    return;
  }

  if (empty($form_state['values']['entity'])) {
    return;
  }

  $uuid = $form_state['values']['entity'];
  $preg_matches = array();
  $match = preg_match('/\[uuid: (' . UUID_PATTERN . ')\]/', $uuid, $preg_matches);
  if (!$match) {
    $match = preg_match('/^uuid: (' . UUID_PATTERN . ')/', $uuid, $preg_matches);
  }

  if ($match) {
    $uuid = $preg_matches[1];
  }

  if (uuid_is_valid($uuid)) {
    $entity = entity_uuid_load($form_state['values']['entity_type'], array($uuid));
    $entity = reset($entity);
  }
  else {
    $entity_info = entity_get_info($form_state['values']['entity_type']);
    $field = $entity_info['entity keys']['label'];
    $entity = entity_uuid_load($form_state['values']['entity_type'], FALSE, array($field => $uuid));
    $entity = reset($entity);
  }

  if (!$entity) {
    form_error($form['entity'], t('Invalid entity selected.'));
  }
  else {
    $ids = entity_extract_ids($form_state['values']['entity_type'], $entity);
    $id = $ids[0];
    $uuids = entity_get_uuid_by_id($form_state['values']['entity_type'], array($id));
    $uuid = !empty($uuids[$id]) ? $uuids[$id] : FALSE;

    form_set_value($form['uuid'], $uuid, $form_state);
  }
}

function uuid_context_entity_uuid_settings_form_submit($form, &$form_state) {
  if ($form_state['values']['set_identifier']) {
    $entity_info = entity_get_info($form_state['values']['entity_type']);
    $entity = entity_uuid_load($form_state['values']['entity_type'], array($form_state['values']['uuid']));
    $entity = reset($entity);
    $form_state['values']['identifier'] = $entity->{$entity_info['entity keys']['label']};
  }

  // This will either be the value set previously or a value set by the
  // validator.
  $form_state['conf']['uuid'] = $form_state['values']['uuid'];
}

/**
 * Provide a list of ways that this context can be converted to a string.
 */
function uuid_context_entity_uuid_convert_list($plugin) {
  $list = array();

  $entity = entity_get_info($plugin['context name']);
  if (isset($entity['token type'])) {
    $token = $entity['token type'];
  }
  else {
    $token = $plugin['context name'];
  }

  // Hack: we need either token.module or a core fix for this to work right,
  // until then, we just muscle it.
  if ($token == 'taxonomy_term') {
    $token = 'term';
  }

  $tokens = token_info();
  if (isset($tokens['tokens'][$token])) {
    foreach ($tokens['tokens'][$token] as $uuid => $info) {
      if (!isset($list[$uuid])) {
        $list[$uuid] = $info['name'];
      }
    }
  }
  return $list;
}

/**
 * Convert a context into a string.
 */
function uuid_context_entity_uuid_convert($context, $type, $options = array()) {
  $entity_type = $context->type[2];
  $entity = entity_get_info($entity_type);

  if (isset($entity['token type'])) {
    $token = $entity['token type'];
  }
  else {
    $token = $entity_type;
  }

  // Hack: we need either token.module or a core fix for this to work right,
  // until then, we just muscle it.
  if ($token == 'taxonomy_term') {
    $token = 'term';
  }

  $tokens = token_info();

  $values = token_generate($token, array($type => $type), array($token => $context->data), $options);
  if (isset($values[$type])) {
    return $values[$type];
  }
}
