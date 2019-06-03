<?php

/**
 * Helper function to move a hook implementation to the top.
 */
function _dvg_global_module_implements_first($module, $module_hooks, &$implementations, $hook) {
  if (in_array($hook, $module_hooks) && isset($implementations[$module])) {
    $implementations = array($module => $implementations[$module]) + $implementations;
  }
}

/**
 * Helper function to move a hook implementation to the bottom.
 */
function _dvg_global_module_implements_last($module, $module_hooks, &$implementations, $hook) {
  if (in_array($hook, $module_hooks) && isset($implementations[$module])) {
    $group = $implementations[$module];
    unset($implementations[$module]);
    $implementations[$module] = $group;
  }
}

/**
 * Helper to remove a message from the session by string.
 */
function _dvg_global_remove_session_message($message, $type = 'status', $regex = FALSE) {
  $removed = FALSE;

  if (isset($_SESSION['messages'][$type])) {
    foreach ($_SESSION['messages'][$type] as $key => $value) {
      if (($regex) ? preg_match($message, $value) : ($message == $value)) {
        unset($_SESSION['messages'][$type][$key]);

        $removed = TRUE;
      }
    }

    if (empty($_SESSION['messages'][$type])) {
      unset($_SESSION['messages'][$type]);
    }
  }

  if (isset($_SESSION['messages']) && empty($_SESSION['messages'])) {
    unset($_SESSION['messages']);
  }

  return $removed;
}

/**
 * Helper function to set the active menu item + breadcrumb for a nid.
 */
function _dvg_global_set_breadcrumb_nid($nid, $node_title_only = FALSE) {
  if ($nid) {
    $breadcrumb = array(l(t('Home'), '<front>', array('purl' => array('disabled' => TRUE))));
    if (!$node_title_only) {
      $result = db_select('menu_links')
        ->fields('menu_links', array('p1', 'p2', 'p3', 'p4', 'p5', 'p6', 'p7', 'p8', 'p9'))
        ->condition('hidden', 0)
        ->condition('link_path', 'node/'. $nid)
        ->execute();
      while ($parents = $result->fetchAssoc()) {
        $set = FALSE;
        foreach (array_filter($parents) as $plid) {
          $parent = menu_link_load($plid);
          if ($parent && $parent['access'] && empty($parent['hidden']) && !empty($parent['title'])) {
            $set = TRUE;
            $breadcrumb[] = l($parent['title'], $parent['href']);
          }
        }
        // Only set the breadcrumb if one or more links were added to the
        // trail. If not, continue iterating through possible menu links.
        if ($set) {
          drupal_set_breadcrumb($breadcrumb);
          break;
        }
      }
    }

    // Fallback breadcrumb if the node is not part of a menu.
    if (count($breadcrumb) == 1) {
      $node = node_load($nid);
      $breadcrumb[] = l($node->title, 'node/' . $node->nid);
      drupal_set_breadcrumb($breadcrumb);
    }
  }
}

/**
 * Helper function that determines if a voc has terms.
 */
function _dvg_global_voc_has_terms($voc_name) {
  $voc = taxonomy_vocabulary_machine_name_load($voc_name);
  if ($voc) {
    $query = db_select('taxonomy_term_data', 't')
      ->condition('vid', $voc->vid)
      ->fields('t')
      ->execute();

    return $query->rowCount();
  }

  return FALSE;
}

/**
 * Implements hook_dvg_requirements().
 */
function _dvg_global_voc_requirements($voc_name, $voc_title) {
  $requirements = array();

  $count = _dvg_global_voc_has_terms($voc_name);
  $requirements['voc_' . $voc_name] = array(
    'title' => t('Vocabulary @voc', array('@voc' => $voc_title)),
  );

  if ($count > 0) {
    $requirements['voc_' . $voc_name] += array(
      'severity' => REQUIREMENT_OK,
      'value' => t('@count terms', array('@count' => $count)),
    );
  }
  else {
    $link = l($voc_title, 'admin/structure/taxonomy/' . $voc_name . '/add');
    $requirements['voc_' . $voc_name] += array(
      'severity' => REQUIREMENT_ERROR,
      'value' => t('No @voc found', array('@voc' => $voc_title)),
      'description' => t('No @voc added yet. Please add a !link', array('@voc' => $voc_title, '!link' => $link)),
    );
  }

  return $requirements;
}

/**
 * Clear all block and page caches. Can be used as submit function in a form.
 */
function _dvg_global_cache_clear_all() {
  cache_clear_all();
}

/**
 * Helper to determine if $path is a content path.
 */
function _dvg_global_is_content_page($path = NULL) {
  if (empty($path)) {
    $path = current_path();
  }

  $exceptions = array(
    'user*',
    'node/add/*',
    'admin/*',
    'appointments/*',
    'media/*',
    variable_get('site_403', ''),
    variable_get('site_404', ''),
  );
  drupal_alter('dvg_global_is_content_page', $exceptions, $path);

  foreach ($exceptions as $exception) {
    if (drupal_match_path(current_path(), $exception)) {
      return FALSE;
    }
  }

  return TRUE;
}
