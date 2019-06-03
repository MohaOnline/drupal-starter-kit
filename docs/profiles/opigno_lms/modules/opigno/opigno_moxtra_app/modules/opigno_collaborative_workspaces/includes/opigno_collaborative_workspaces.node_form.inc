<?php

/////////////
// FORM ALTER HOOK
/////////////////////////////

function opigno_collaborative_workspaces_form_node_form_alter(&$form, &$form_state, $form_id)
{
  if ($form_id == 'collaborative_workspace_node_form')
  {

    if(isset($form_state['node']->nid)) {
      // update
      $form['#validate'][] = 'opigno_collaborative_workspaces_form_update_validate';

      if (!isset($form_state['old_invited_users']))
        $form_state['old_invited_users'] = $form['field_users_invited'][LANGUAGE_NONE]['#default_value'];
    }
    else {
      // create
      $form['#validate'][] = 'opigno_collaborative_workspaces_form_create_validate';
    }


    // Hide some fields. This field will be completed automatically when the workspace will be created
    $form['field_collaborative_workspace_id']['#access'] = false;


    // Remove the creator of the collaborative workspace (he will be selected anyway in javascript)
    // Remove the anonymous user
    // Remove the admin
    unset($form['field_users_invited'][LANGUAGE_NONE]['#options'][ $form['#node']->uid ]);
    unset($form['field_users_invited'][LANGUAGE_NONE]['#options'][0]);
    unset($form['field_users_invited'][LANGUAGE_NONE]['#options'][1]);


    // Can't preview the node. Prevent some errors
    $form['actions']['preview']['#access'] = false;


    // Get classes and courses from the user
    $courses = array();
    $classes = array();

    $groups_is_member = og_get_groups_by_user(NULL, 'node');
    foreach($groups_is_member as $group_id) {
      $node = node_load($group_id);
      if ($node->type == 'course')
        $courses[$node->nid] = $node->title;
      else if ($node->type == 'class')
        $classes[$node->nid] = $node->title;
    }

    $options = array('_none' => '-None-');
    if (!empty($courses))
      $options['Course'] = $courses;
    if (!empty($classes))
      $options['Class'] = $classes;


    // Keep the classes and courses for the ajax callback (he needs them)
    $form['all_user_courses_classes'] = array(
      '#type' => 'value',
      '#value' => ($courses+$classes)
    );


    // Show the list of courses and classes
    $form['field_classes_courses'] = array(
      '#type' => 'select',
      '#title' => $form['field_users_invited'][LANGUAGE_NONE]['#title'],
      '#weight' => $form['field_users_invited'][LANGUAGE_NONE]['#weight'],
      '#options' => $options,
      '#ajax' => array(
        'callback' => 'opigno_collaborative_workspaces_form_field_invited_users_update_callback',
        'wrapper' => 'users-multiselect',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#attributes' => array(
        'onchange' => 'jQuery(".field_users_invited_multiselect_sel").selectAll();'
      )
    );

    // Attach CSS and Javascript for multiselect type field.
    $form['#attached']['js'][] = drupal_get_path('module', 'multiselect') . '/multiselect.js';
    $form['#attached']['css'][] = drupal_get_path('module', 'multiselect') . '/multiselect.css';

    // Create the multiselect using the simple user select data and hide the simple user select
    $form['field_users_invited_multiselect'] = array(
        '#type' => 'multiselect',
        '#multiple' => true,
        '#size' => 10,
        '#validated' => true,
        '#title' => '',
        '#weight' => $form['field_users_invited'][LANGUAGE_NONE]['#weight']+1,
        '#prefix' => '<div id="users-multiselect">',
        '#suffix' => '</div>',
      ) + $form['field_users_invited'][LANGUAGE_NONE];

    // Hide the users invited field (because we will show a multiselect instead
    $form['field_users_invited'][LANGUAGE_NONE]['#access'] = false;


    // Add all the users in the multiselect (only if the trigger element is from the "field_classes_courses" ajax)
    if ($form_state['triggering_element']['#name'] != 'field_classes_courses') {
      $form_state['values']['field_users_invited_multiselect'] = $form['field_users_invited_multiselect']['#default_value'];
      $form['field_users_invited_multiselect'] = opigno_collaborative_workspaces_form_field_invited_users_update_callback($form, $form_state);
    }


    // We are forced by the multiselect widget to put at least one element to the selected list. Else, the submit doesn't work.
    //  So let use this and add directly here the creator of the collaborative workspace
    $form['actions']['submit']['#attributes'] = array('onclick' => "valid_multi();");
    $form['actions']['delete']['#attributes'] = array('onclick' => "valid_multi();");
    drupal_add_js('function valid_multi() { jQuery(".field_users_invited_multiselect_sel").addOption({text:"",value:'. $form['#node']->uid .'}); }', array('type' => 'inline'));
  }
}

function opigno_collaborative_workspaces_form_field_invited_users_update_callback($form, &$form_state)
{
  global $user;

  if (is_numeric($form['field_classes_courses']['#value']))
    $users_ids = opigno_collaborative_workspaces_db_get_users_ids_from_group($form['field_classes_courses']['#value']);
  else if (user_access('invite all users to collaborative_workspace')) // Admin can see every users
    $users_ids = db_select('users', 'u')->fields('u', array('uid'))->execute()->fetchCol();
  else
    $users_ids = opigno_collaborative_workspaces_db_get_users_ids_from_groups(array_keys($form['all_user_courses_classes']['#value']));

  $selected_users_ids = $form_state['values']['field_users_invited_multiselect'];
  opigno_collaborative_workspaces_form_add_users_to_multiselect_field($users_ids, $selected_users_ids, $form);

  return $form['field_users_invited_multiselect'];
}


/////////////
// VALIDATION HANDLERS
////////////////////////////////////////

function opigno_collaborative_workspaces_form_create_validate($form, &$form_state)
{
  if (in_array('node_form_submit', $form_state['triggering_element']['#submit']))
  {
    opigno_collaborative_workspaces_form_refactor_multiselect_to_select_values($form_state, 'field_users_invited_multiselect', 'field_users_invited');

    $title = $form_state['values']['title'];
    $creator_id = $form_state['values']['uid'];

    $users_ids = opigno_collaborative_workspaces_form_get_users_to_invite_field($form_state);

    // Create the collaborative workspace here in _validate, so if there's an error from Moxtra, you can cancel the creation and show the error
    $response = opigno_collaborative_workspaces_create_collaborative_workspace($creator_id, $title);
    if ($response != null && $response->code == 'RESPONSE_SUCCESS') {
      $form_state['values']['field_collaborative_workspace_id'][LANGUAGE_NONE][0]['value'] = $response->data->id;

      opigno_collaborative_workspaces_add_users($creator_id, $response->data->id, $users_ids);
    }
  }
}

function opigno_collaborative_workspaces_form_update_validate($form, &$form_state)
{
  if (in_array('node_form_submit', $form_state['triggering_element']['#submit']))
  {
    opigno_collaborative_workspaces_form_refactor_multiselect_to_select_values($form_state, 'field_users_invited_multiselect', 'field_users_invited');

    $creator_id = $form_state['values']['uid'];
    $collaborative_workspace_id = $form_state['values']['field_collaborative_workspace_id'][LANGUAGE_NONE][0]['value'];
    $title = $form_state['values']['title'];

    $new_users_ids = opigno_collaborative_workspaces_form_get_users_to_invite_field($form_state);
    $old_users_ids = $form_state['old_invited_users'];

    $users_to_add = array_diff($new_users_ids, $old_users_ids);
    $users_to_remove = array_diff($old_users_ids, $new_users_ids);


    opigno_collaborative_workspaces_update_collaborative_workspace($creator_id, $collaborative_workspace_id, $title);

    if (!empty($users_to_add))
      opigno_collaborative_workspaces_add_users($creator_id, $collaborative_workspace_id, $users_to_add);

    foreach($users_to_remove as $user_to_remove)
      opigno_collaborative_workspaces_remove_user($creator_id, $collaborative_workspace_id, $user_to_remove);
  }
}



///////////////
// FIELD PROCESSING FUNCTIONS
////////////////////////////////

function opigno_collaborative_workspaces_form_add_user_in_users_to_invite_field(&$form_state, $uid)
{
  $users_ids = opigno_collaborative_workspaces_form_get_users_to_invite_field($form_state);
  if (!in_array($uid, $users_ids))
    if ($form_state['values']['field_users_invited'][LANGUAGE_NONE][0]['target_id'] == null)
      $form_state['values']['field_users_invited'][LANGUAGE_NONE][0] = array('target_id' => $uid);
    else
      $form_state['values']['field_users_invited'][LANGUAGE_NONE][] = array('target_id' => $uid);

}

function opigno_collaborative_workspaces_form_get_users_to_invite_field($form_state)
{
  return array_map(
    function ($entry) { return $entry['target_id']; },
    $form_state['values']['field_users_invited'][LANGUAGE_NONE]);
}

function opigno_collaborative_workspaces_form_refactor_multiselect_to_select_values(&$form_state, $multiselect_name, $select_name)
{
  $form_state['values'][$select_name][LANGUAGE_NONE] = array_map(function($entry) {
    return array('target_id' => $entry);
  }, $form_state['values'][$multiselect_name]);
}

function opigno_collaborative_workspaces_form_set_users_to_invite_field($users_ids, &$form_state)
{
  $form_state['values']['field_users_invited'][LANGUAGE_NONE] = array();
  foreach($users_ids as $user_id)
    opigno_collaborative_workspaces_form_add_user_in_users_to_invite_field($form_state, $user_id);
}

function opigno_collaborative_workspaces_form_add_users_to_multiselect_field($users_ids, $selected_users_ids, &$form)
{
  $form['field_users_invited_multiselect']['#options'] = array();

  $form['field_users_invited_multiselect']['#default_value'] = $selected_users_ids;

  // Need to add the already selected users in the multiselect. As they are in the #default_value, I need to add them
  //   in the #options in order to show them
  if (!empty($selected_users_ids))
    $users_ids = array_merge(array_values($users_ids), array_values($selected_users_ids));

  foreach($users_ids as $user_id) {
    // Add all the user, but not the creator of the node nor the anonymous user
    if ($form['#node']->uid != $user_id && $user_id != 1 && $user_id != 0) {
      $foreach_user = user_load($user_id);
      $form['field_users_invited_multiselect']['#options'][$user_id] = $foreach_user->name;
    }
  }

  // If the user has right to edit the content of the workspace, he can add himself to the workspace
  //   But if he's the creator, he will be in the workspace anyway
  global $user;
  if (user_access('edit collaborative_workspace content') && $form['#node']->uid != $user->uid) {
    $form['field_users_invited_multiselect']['#options'][$user->uid] = $user->name;
  }
}
