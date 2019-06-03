<?php

/**
 * @file
 * Configuration options for image_lazy_loader Formatter settings
 */

/**
 * Menu callback; Settings administration.
 */
function image_lazy_loader_admin_settings() {
  $form['image_lazy_loader_animate_library'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Animate.css Library'),
    '#description'   => t('Disable if animate.css library is added in the current theme.<br> Source: https://daneden.github.io/animate.css/'),
    '#default_value' => variable_get('image_lazy_loader_animate_library', 0),
    '#suffix'        => '<br>',
    '#prefix'        => '<h1>Animate.css</h1>'    
  );
  return system_settings_form($form);
}

// function image_lazy_loader_admin_settings($form, &$form_state) {
//   $form_state['storage']['classes'] =
//     isset($form_state['storage']['classes']) ? $form_state['storage']['classes'] : 0;

//   // Pre
//   $classes_array = array();
//   for ($i = 0; $i <= $form['storage']['#default_value']; $i++) {
//     $classes_array[] = $form['storage']['#default_value'][$i];
//   }

//   $form['storage'] =  array(
//     '#type'          => 'textfield',
//     '#title'         => t('Classes storaged'),
//     '#default_value' => variable_get('classes')
//   );

//   $form['image_lazy_loader_animate_library'] = array(
//     '#type'          => 'checkbox',
//     '#title'         => t('Animate.css Library'),
//     '#description'   => t('Disable if animate.css library is added in the current theme.<br> Source: https://daneden.github.io/animate.css/'),
//     '#default_value' => variable_get('image_lazy_loader_animate_library', 0),
//     '#suffix'        => '<br><hr>',
//     '#prefix'        => '<h1>Animate.css</h1>'
//   );

//   $form['classes'] = array(
//     '#type' => 'container',
//     '#tree' => TRUE,
//     '#prefix' => '<div id="classes">',
//     '#suffix' => '</div>',
//   );

//   if (isset($form_state['storage']['classes'])) {
//     for ($i = 0; $i <= $form_state['storage']['classes']; $i++) {
//       $form['classes'][$i] = array(
//         '#type' => 'fieldset',
//         '#tree' => TRUE,
//       );

//       $form['classes'][$i]['image_lazy_loader_background_img'] =  array(
//       '#type'          => 'textfield',
//       '#title'         => t('Add classes'),
//       '#description'   => t('Disable if animate.css library is added in the current theme.<br> Source: https://daneden.github.io/animate.css/'),
//       '#default_value' => variable_get('image_lazy_loader_background_img'),
//       '#suffix'        => '<br>',
//       '#prefix'        => '<br><h1>Background images</h1><p>If you are displaying images as "background-img" inline style in templates</p>'
//       );
//     }
//   }

//   $form['add_class'] = array(
//     '#type' => 'button',
//     '#value' => variable_get('image_lazy_loader_background_img'),
//     '#href' => '',
//     '#ajax' => array(
//       'callback' => 'image_lazy_loader_ajax_add_class',
//       'wrapper' => 'classes',
//      ),
//   );

//   $form_state['storage']['classes']++;
//   drupal_set_message('<pre>'. print_r($form['storage'], true) .'</pre>');
  
//   return system_settings_form($form);
// }

// function image_lazy_loader_ajax_add_class($form, $form_state) {
//   return $form['classes'];
// }