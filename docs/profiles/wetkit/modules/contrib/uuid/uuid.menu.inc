<?php

/**
 * @file
 * Contains menu item registration for the content tool.
 *
 * The menu items registered are AJAX callbacks for the things like
 * autocomplete and other tools needed by the content types.
 */

/**
 * Helper function for autocompletion of entity titles.
 */
function uuid_autocomplete_entity($type, $string = '') {
  if ($string != '') {
    $entity = entity_get_info($type);
    global $user;

    // @todo verify the query logic here, it's untested.
    // Set up the query
    $query = db_select($entity['base table'], 'b');
    if ($entity['entity keys']['label']) {
      $query->fields('b', array($entity['entity keys']['uuid'], $entity['entity keys']['label']))->range(0, 10);
    }
    else {
      $query->fields('b', array($entity['entity keys']['uuid']))->range(0, 10);
    }

    $preg_matches = array();
    $match = preg_match('/\[uuid: (' . UUID_PATTERN . ')\]/', $string, $preg_matches);
    if (!$match) {
      $match = preg_match('/^uuid: (' . UUID_PATTERN . ')/', $string, $preg_matches);
    }
    if ($match) {
      $query->condition('b.' . $entity['entity keys']['uuid'], $preg_matches[1]);
    }
    elseif ($entity['entity keys']['label']) {
      $query->condition('b.' . $entity['entity keys']['label'], '%' . db_like($string) . '%', 'LIKE');
    }

    $matches = array();
    if ($type == 'node') {
      if (!user_access('bypass node access')) {
        // If the user is able to view their own unpublished nodes, allow them
        // to see these in addition to published nodes.
        if (user_access('view own unpublished content')) {
          $query->condition(db_or()
            ->condition('b.status', NODE_PUBLISHED)
            ->condition('b.uid', $user->uid)
          );
        }
        else {
          // If not, restrict the query to published nodes.
          $query->condition('b.status', NODE_PUBLISHED);
        }
      }

      $query->addTag('node_access');
      $query->join('users', 'u', 'b.uid = u.uid');
      $query->addField('u', 'name', 'name');

      foreach ($query->execute() as $nodeish) {
        $name = empty($nodeish->name) ? variable_get('anonymous', t('Anonymous')) : check_plain($nodeish->name);
        $matches[$nodeish->title . " [uuid: $nodeish->uuid]"] = '<span class="autocomplete_title">' . check_plain($nodeish->title) . '</span> <span class="autocomplete_user">(' . t('by @user', array('@user' => $name)) . ')</span>';
      }
    }
    else {
      foreach ($query->execute() as $item) {
        $id = $item->{$entity['entity keys']['uuid']};
        if ($entity['entity keys']['label']) {
          $matches[$item->{$entity['entity keys']['label']} . " [uuid: $id]"] = '<span class="autocomplete_title">' . check_plain($item->{$entity['entity keys']['label']}) . '</span>';
        }
        else {
          $matches["[uuid: $id]"] = '<span class="autocomplete_title">' . check_plain($item->{$entity['entity keys']['uuid']}) . '</span>';
        }
      }
    }
    drupal_json_output($matches);
  }
}
