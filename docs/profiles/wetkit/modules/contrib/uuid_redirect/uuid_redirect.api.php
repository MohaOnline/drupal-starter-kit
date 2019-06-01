<?php

/**
 * @file
 * Hooks provided by the UUID Redirect module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Indicate that a UUID redirect should not be performed.
 *
 * This hook is invoked when UUID Redirect is in the process of redirecting the
 * current page to an external site. Modules may return TRUE here to indicate
 * that the redirect should be skipped and that the normal page (on the current
 * site) should be displayed instead.
 *
 * This hook will be invoked once for each entity in the current path. For
 * example, if the current page corresponds to a hypothetical menu item like:
 *
 * node/%node/author/%user/edit
 *
 * the hook will be invoked once for the node entity, and once for the user
 * entity. If there are no entities in the path (e.g., the redirect is for a
 * static path rather than a dynamic one), the hook will not be invoked, since
 * this module's primary use case involves dynamic redirects of entity-related
 * paths.
 *
 * @param $entity_type
 *   The type of entity (for example, 'node' or 'user') which appears in the
 *   current path.
 * @param $entity
 *   The fully-loaded entity which appears in the current path.
 * @param $menu_info
 *   An array of additional information about the entity (mostly derived from
 *   the menu path), containing the following keys:
 *   - load function: The name of the menu load function corresponding to this
 *     entity, e.g. 'node_load'.
 *   - bundle restrictions: An array of bundle names that the redirect is
 *     intended to be confined to. (This is primarily used by UUID Redirect's
 *     own implementation of this hook, as in the example code below.)
 *   - revision: A boolean indicating whether this entity is believed to
 *     represent a revision of the entity rather than the main entity itself.
 *
 * @return
 *   This hook should return TRUE to indicate that the redirect should be
 *   skipped. No return value is necessary otherwise.
 */
function hook_uuid_redirect_skip_redirect($entity_type, $entity, $menu_info) {
  // Do not redirect if we are only redirecting certain bundles and this isn't
  // one of them.
  if (!empty($menu_info['bundle restrictions'])) {
    $entity_info = entity_get_info($entity_type);
    if (isset($entity_info['entity keys']['bundle'])) {
      $bundle_key = $entity_info['entity keys']['bundle'];
      if (isset($entity->{$bundle_key}) && !in_array($entity->{$bundle_key}, $menu_info['bundle restrictions'])) {
        return TRUE;
      }
    }
  }
}

/**
 * @} End of "addtogroup hooks".
 */
