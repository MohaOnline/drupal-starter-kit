<?php

/**
 * @file
 * Display functions.
 */

/**
 * Renders an embedded entity.
 *
 * @param $entity_type
 *   The type of entity, i.e. 'node', 'user'.
 * @param $entity
 *   The entity to be rendered.
 * @param array $context
 *   (optional) Array of context values, corresponding to the attributes on
 *   the embed HTML tag.
 *
 * @return string
 *   The HTML of the entity rendered with the display plugin.
 */
function entity_embed_render_entity($entity_type, $entity, array $context = array()) {
  // Support the deprecated view-mode data attribute.
  if (isset($context['data-view-mode']) && !isset($context['data-entity-embed-display']) && !isset($context['data-entity-embed-settings'])) {
    $context['data-entity-embed-display'] = 'default';
    $context['data-entity-embed-settings'] = array('view_mode' => $context['data-view-mode']);
  }

  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  // Merge in default attributes.
  $context += array(
    'data-entity-id' => $id,
    'data-entity-type' => $entity_type,
    'data-entity-embed-display' => 'default',
    'data-entity-embed-settings' => array(),
  );

  // Allow modules to alter the entity prior to embed rendering.
  drupal_alter(array("{$context['data-entity-type']}_embed_context", 'entity_embed_context'), $context, $entity);

  // Build and render the display plugin, allowing modules to alter the
  // result before rendering.
  $build = render_entity_embed_display_plugin(
    $entity_type,
    $entity,
    $context['data-entity-embed-display'],
    $context['data-entity-embed-settings'],
    $context
  );

  // Maintain data-align if it is there.
  if (isset($context['data-align'])) {
    $build['#attributes']['data-align'] = $context['data-align'];
  }
  elseif ((isset($context['class']))) {
    $build['#attributes']['class'][] = $context['class'];
  }

  // Maintain data-caption if it is there.
  if (isset($context['data-caption'])) {
    $build['#attributes']['data-caption'] = $context['data-caption'];
  }

  // @todo Should this hook get invoked if $build is an empty array?
  drupal_alter(array("{$context['data-entity-type']}_embed", 'entity_embed'), $build, $entity, $context);
  $entity_output = drupal_render($build);

  return $entity_output;
}

/**
 * Renders an entity using an EntityEmbedDisplay plugin.
 *
 * @param $entity_type
 *   The type of entity, i.e. 'node', 'user'.
 * @param $entity
 *   The entity to be rendered.
 * @param string $plugin_id
 *   The EntityEmbedDisplay plugin ID.
 * @param array $plugin_configuration
 *   (optional) Array of plugin configuration values.
 * @param array $context
 *   (optional) Array of additional context values, usually the embed HTML
 *   tag's attributes.
 *
 * @return array
 *   A render array for the display plugin.
 */
function render_entity_embed_display_plugin($entity_type, $entity, $plugin_id, array $plugin_configuration = array(), array $context = array()) {
  // Check if the display plugin is accessible.
  if (!entity_access('view', $entity_type, $entity)) {
    return array();
  }

  // Build and render the entity.
  $render = entity_view($entity_type, array($entity), $plugin_configuration['view_mode'], $context['data-langcode']);

  // Workaround for file_entity module that does not have the patch in
  // https://www.drupal.org/node/2365821 applied yet.
  if ($entity_type === 'file' && isset($render['files'])) {
    $render = array('file' => reset($render));
  }

  return $render;
}
