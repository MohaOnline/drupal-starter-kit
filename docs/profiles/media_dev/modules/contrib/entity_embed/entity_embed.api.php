<?php

/**
 * @file
 * Hooks provided by the entity_embed module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the placeholder context for an embedded entity.
 *
 * @param array &$context
 *   The context array.
 * @param callable &$callback
 *   The callback function to be used.
 * @param $entity
 *   The entity being rendered.
 */
function hook_entity_embed_context_alter(&$context, &$callback, $entity) {

}

/**
 * Act on an entity before it is about to be rendered in an embed.
 *
 * @param $entity
 *   The entity object.
 * @param array $context
 *   The context array.
 */
function hook_entity_preembed($entity, $context) {
  if (isset($context['overrides']) && is_array($context['overrides'])) {
    foreach ($context['overrides'] as $key => $value) {
      $entity->key = $value;
    }
  }
}

/**
 * Alter the result of entity_view().
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * block content structure has been built.
 *
 * @param array &$build
 *   A renderable array of data, as returned from entity_view().
 * @param $entity
 *   The entity object.
 * @param array $context
 *   The context array.
 */
function hook_entity_embed_alter(&$build, $entity, $context) {
  // Remove the contextual links on all entites that provide them.
  if (isset($build['#contextual_links'])) {
    unset($build['#contextual_links']);
  }
}

/**
 * @} End of "addtogroup hooks".
 */
