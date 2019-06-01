<?php

/**
 * @file
 * API documentation for WetKit Navbar.
 */

/**
 * Inform about additional module-specific caches that can be cleared.
 *
 * Administration menu uses this hook to gather information about available
 * caches that can be flushed individually. Each returned item forms a separate
 * menu link below the "Flush all caches" link in the icon menu.
 *
 * @return array
 *   An associative array whose keys denote internal identifiers for a
 *   particular caches (which can be freely defined, but should be in a module's
 *   namespace) and whose values are associative arrays containing:
 *   - title: The name of the cache, without "cache" suffix. This label is
 *     output as link text, but also for the "!title cache cleared."
 *     confirmation message after flushing the cache; make sure it works and
 *     makes sense to users in both locations.
 *   - callback: The name of a function to invoke to flush the individual cache.
 */
function hook_wetkit_navbar_cache_info() {
  $caches['update'] = array(
    'title' => t('Update data'),
    'callback' => '_update_cache_clear',
  );
  return $caches;
}
