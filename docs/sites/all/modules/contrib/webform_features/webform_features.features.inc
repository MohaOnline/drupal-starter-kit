<?php

/**
 * @file
 * Webform Features features integration.
 */

/**
 * Implements hook_features_export_options().
 */
function webform_features_export_options() {
  $options = array();
  $types = node_type_get_names();

  $query = db_select('node', 'n')
    ->fields('n', array('nid', 'title', 'type'))
    ->condition('type', webform_variable_get('webform_node_types'), 'IN')
    ->orderBy('type', 'ASC')
    ->orderBy('title', 'ASC');
  $query->join('webform', 'w', 'n.nid = w.nid');
  $query->fields('w', array('machine_name'))
    ->isNotNull('machine_name')
    ->condition('machine_name', '', '!=');
  $results = $query->execute()->fetchAll();

  foreach ($results as $result) {
    $options[$result->machine_name] = t('@type: @title', array(
      '@type' => $types[$result->type],
      '@title' => $result->title,
    ));
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function webform_features_export($data, &$export, $module_name) {
  $pipe = array();
  $export['dependencies']['webform'] = 'webform';
  $export['dependencies']['webform_features'] = 'webform_features';

  foreach ($data as $machine_name) {
    $node = webform_features_machine_name_load($machine_name);
    if (empty($node)) {
      continue;
    }

    $pipe['node'][$node->type] = $node->type;

    $export['features']['webform'][$machine_name] = $machine_name;
  }

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function webform_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '$webforms = array();';

  foreach ($data as $machine_name) {
    $node = webform_features_machine_name_load($machine_name, TRUE);
    if (empty($node)) {
      continue;
    }

    $export = clone $node;

    // Use date instead of created, in the same format used by node_object_prepare.
    $export->date = format_date($export->created, 'custom', 'Y-m-d H:i:s O');

    // Don't cause conflicts with nid/vid/revision_timestamp/changed fields.
    unset($export->nid);
    unset($export->vid);
    unset($export->changed);
    unset($export->created);
    unset($export->date);
    unset($export->revision_timestamp);
    unset($export->last_comment_timestamp);
    unset($export->webform['nid']);
    unset($export->webform['next_serial']);
    unset($export->path['source']);

    // Manage components.
    $components_machine_names = array();
    foreach ($export->webform['components'] as $key => $component) {
      $components_machine_names[$component['cid']] = $component['machine_name'];
    }
    $components_array = array();
    foreach ($export->webform['components'] as $key => $component) {
      if (!empty($component['pid'])) {
        $component['parent_machine_name'] = $components_machine_names[$component['pid']];
      }
      unset($component['nid']);
      unset($component['cid']);
      unset($component['pid']);
      $components_array[$component['machine_name']] = $component;
    }
    $export->webform['components'] = $components_array;

    // Manage conditionals.
    $conditionals_array = array();
    foreach ($export->webform['conditionals'] as $key => $conditional) {
      // Remove IDs.
      unset($conditional['nid']);

      // Convert rule source component IDs to machine names.
      foreach ($conditional['rules'] as &$rule) {
        if ($rule['source_type'] == 'component') {
          if (array_key_exists($rule['source'], $components_machine_names)) {
            $rule['source'] = $components_machine_names[$rule['source']];
            $rule['source_component_machine_name'] = TRUE;
          }
        }
        unset($rule['nid']);
      }
      // Convert action target component IDs to machine names.
      foreach ($conditional['actions'] as &$action) {
        if ($action['target_type'] == 'component') {
          if (array_key_exists($action['target'], $components_machine_names)) {
            $action['target'] = $components_machine_names[$action['target']];
            $action['target_component_machine_name'] = TRUE;
          }
        }
        unset($action['nid']);
      }

      $conditionals_array[] = $conditional;
    }
    $export->webform['conditionals'] = $conditionals_array;

    // Manage emails.
    $emails_array = array();
    foreach ($export->webform['emails'] as $key => $email) {
      // Remove IDs.
      unset($email['nid']);
      unset($email['eid']);

      // Handle parameters linked to a component.
      $email['components_machine_names'] = array();
      foreach (array('email', 'subject', 'from_name', 'from_address') as $field_name) {
        $email['components_machine_names'][$field_name] = FALSE;
        if (is_numeric($email[$field_name])) {
          $email[$field_name] = $components_machine_names[$email[$field_name]];
          $email['components_machine_names'][$field_name] = TRUE;
        }
      }

      $emails_array[] = $email;
    }
    $export->webform['emails'] = $emails_array;

    // Clean file fields to avoid exporting issues
    // Copied from UUID Features uuid_features_file_field_export().
    $fields = field_info_instances('node', $export->type);
    foreach ($fields as $field_instance) {
      $field = &$export->{$field_instance['field_name']};
      $info = field_info_field($field_instance['field_name']);
      if (in_array($info['type'], array('file', 'image'))) {
        $field = array();
      }
    }

    $code[] = "  \$webforms['{$machine_name}'] = " . features_var_export($export) . ";";
  }

  $code[] = "return \$webforms;";
  $code = implode("\n", $code);
  return array('webform_defaults' => $code);
}

/**
 * Implements hook_features_revert().
 */
function webform_features_revert($module) {
  webform_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 */
function webform_features_rebuild($module) {
  $nodes = module_invoke($module, 'webform_defaults');
  if (!empty($nodes)) {
    module_load_include('inc', 'node', 'node.pages');
    module_load_include('inc', 'webform', 'includes/webform.components');
    module_load_include('inc', 'webform', 'includes/webform.emails');

    foreach ($nodes as $data) {
      if (empty($data['webform']) || empty($data['webform']['machine_name'])) {
        continue;
      }
      $webform_machine_name = $data['webform']['machine_name'];

      $node = (object) $data;
      $node->webform['emails'] = array();
      $node->webform['components'] = array();
      node_object_prepare($node);

      // Find the matching machine name, with a fresh cache.
      $existing = webform_features_machine_name_load($webform_machine_name);
      if (empty($existing)) {
        $node = node_submit($node);
        node_save($node);
        $existing = $node;
      }

      // Reinject base properties.
      $node->nid = $existing->nid;
      $node->vid = $existing->vid;
      $node->webform['nid'] = $existing->nid;
      if (!empty($node->path)) {
        $node->path['source'] = 'node/' . $existing->nid;
      }

      // Manage components.
      $existing_components_machine_names = array();
      foreach ($existing->webform['components'] as $component) {
        $existing_components_machine_names[$component['machine_name']] = $component['cid'];
      }
      _webform_features_rebuild_components($node, $data['webform']['components'], $existing_components_machine_names);

      // Manage conditionals, conditionals_rules and conditionals_actions.
      db_delete('webform_conditional')
        ->condition('nid', $node->nid)
        ->execute();
      db_delete('webform_conditional_rules')
        ->condition('nid', $node->nid)
        ->execute();
      db_delete('webform_conditional_actions')
        ->condition('nid', $node->nid)
        ->execute();

      if (!empty($data['webform']['conditionals'])) {
        foreach ($data['webform']['conditionals'] as $index => $conditional) {
          // Replace IDs.
          $conditional['nid'] = $node->nid;

          // Convert rule source component machine names to IDs.
          foreach ($conditional['rules'] as &$rule) {
            if (!empty($rule['source_component_machine_name'])) {
              $rule['source'] = $existing_components_machine_names[$rule['source']];
              unset($rule['source_component_machine_name']);
            }
            $rule['nid'] = $node->nid;
          }
          // Convert action target component machine names to IDs.
          foreach ($conditional['actions'] as &$action) {
            if (!empty($action['target_component_machine_name'])) {
              $action['target'] = $existing_components_machine_names[$action['target']];
              unset($action['target_component_machine_name']);
            }
            $action['nid'] = $node->nid;
          }

          $node->webform['conditionals'][$index] = $conditional;
        }
      }

      // Manage emails.
      db_delete('webform_emails')
        ->condition('nid', $node->nid)
        ->execute();
      if (!empty($data['webform']['emails'])) {
        foreach ($data['webform']['emails'] as $email) {
          $email['nid'] = $node->nid;
          foreach (array_filter($email['components_machine_names']) as $field_name => $active) {
            $email[$field_name] = $existing_components_machine_names[$email[$field_name]];
          }
          unset($email['components_machine_names']);
          $email['eid'] = webform_email_insert($email);
          $node->webform['emails'][$email['eid']] = $email;
        }
      }

      // Final save.
      node_save($node);
    }
  }
}

/**
 * Recursively build components.
 */
function _webform_features_rebuild_components($node, &$components, &$existing_components_machine_names) {
  if (!empty($components)) {
    // First pass, manage parents and new components.
    foreach ($components as $component) {
      $component['cid'] = _webform_features_rebuild_component($node, $components, $existing_components_machine_names, $component);
    }
    // Second pass, attach components to the webform.
    foreach ($components as $component) {
      $node->webform['components'][$component['cid']] = $component;
    }
  }
}

/**
 * Build a component starting by its parent.
 */
function _webform_features_rebuild_component($node, &$components, &$existing_components_machine_names, &$component) {
  $component['pid'] = 0;
  if (!empty($component['parent_machine_name'])) {
    $component['pid'] = _webform_features_rebuild_component($node, $components, $existing_components_machine_names, $components[$component['parent_machine_name']]);
  }
  $component['nid'] = $node->nid;
  if (empty($existing_components_machine_names[$component['machine_name']])) {
    webform_component_insert($component);
    $existing_components_machine_names[$component['machine_name']] = $component['cid'];
  }
  else {
    $component['cid'] = $existing_components_machine_names[$component['machine_name']];
  }
  $components[$component['machine_name']] = $component;
  return $component['cid'];
}
