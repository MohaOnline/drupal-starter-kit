<?php

/**
 * Form callback for devel_generate support.
 */
function course_generate_form($form, $form_state) {

  $form['course_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Course types'),
    '#options' => drupal_map_assoc(course_get_types()),
    '#required' => TRUE,
  );

  $question_options = array();
  foreach (course_get_handlers('object') as $module => $objects) {
    foreach ($objects as $objname => $object) {
      $course_objects["$module-$objname"] = $object['name'];
    }
  }

  $form['course_object_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Object types'),
    '#options' => $course_objects,
    '#required' => TRUE,
  );

  $form['course_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of courses to generate'),
    '#default_value' => 50,
    '#size' => 10,
    '#required' => TRUE,
    '#description' => t('Course authors will be randomly assigned.'),
  );

  $form['course_object_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of objects per course'),
    '#default_value' => 10,
    '#size' => 10,
    '#required' => TRUE,
    '#description' => t('Course authors will be randomly assigned.'),
  );

  $form['course_enrollments'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of enrollments per course'),
    '#default_value' => 50,
    '#size' => 10,
    '#required' => TRUE,
    '#description' => t('Enrollments will be randomly assigned to users already in the system.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate')
  );

  return $form;
}

/**
 * Submit callback for devel_generate support.
 */
function course_generate_form_submit($form, &$form_state) {
  module_load_include('inc', 'devel_generate', 'devel_generate.fields');
  module_load_include('inc', 'devel_generate', 'devel_generate');

  $course_types = array_filter($form_state['values']['course_types']);
  $course_limit = $form_state['values']['course_limit'];
  $course_objects = array_filter($form_state['values']['course_object_types']);
  $course_object_limit = $form_state['values']['course_object_limit'];
  $course_enrollments = $form_state['values']['course_enrollments'];

  for ($i = 1; $i <= $course_limit; $i++) {
    course_generate(array_rand($course_types), $course_objects, $course_object_limit, $course_enrollments);
  }
}

/**
 * Generate random course data.
 *
 * @todo associate results with random users
 *
 * @param type $course_type
 * @param type $question_types
 * @param type $question_limit
 * @param type $course_results
 */
function course_generate($course_type, $course_object_types, $course_object_limit, $course_enrollments) {
  $users = array_filter(devel_get_users());

  $node = (object) array(
      'type' => $course_type,
      'title' => devel_create_greeking(rand(5, 10), TRUE),
      'uid' => $users[array_rand($users)],
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
  );

  // Populate all core fields on behalf of field.module
  module_load_include('inc', 'devel_generate', 'devel_generate.fields');
  devel_generate_fields($node, 'node', $node->type);
  unset($node->og_group_ref);
  node_save($node);

  for ($i = 1; $i <= $course_object_limit; $i++) {
    course_generate_objects($node, array_rand($course_object_types));
  }

  for ($i = 1; $i <= $course_enrollments; $i++) {
    if ($uid = array_pop($users)) {
      $course_enrollment = entity_create('course_enrollment', array(
        'type' => 'course_enrollment',
        'nid' => $node->nid,
        'uid' => $uid,
      ));
      $course_enrollment->save();
    }
    else {
      drupal_set_message("Did not have enough users. Could only create $i enrollments.");
      break;
    }
  }

  drupal_set_message('Generated course: ' . l($node->title, 'node/' . $node->nid));
}

/**
 * Generate Course objects.
 * @param type $course
 * @param type $object_type
 */
function course_generate_objects($node, $course_object_type) {
  list($module, $object_type) = explode('-', $course_object_type);
  $co = course_get_course_object($module, $object_type);
  $co->setCourse($node->nid);
  $co->save();
}
