<?php
/**
 * @file
 * Theme function of Be sure module.
 */

/**
 * Process variables for be-sure-element.tpl.php.
 */
function template_preprocess_be_sure_element(&$vars) {
  $percent = $vars['status'];

  $vars['status'] = theme('be_sure_progress_bar', array(
    'percent' => $percent,
    'class' => $percent > 25 ? ($percent > 75 ? 'bes-success' : 'bes-normal') : 'bes-warning',
    'message' => t('!passed/!total issues resolved', array('!passed' => $vars['passed'], '!total' => count($vars['items']))),
  ));
}

/**
 * Process variables for be-sure-multiple.tpl.php.
 */
function template_preprocess_be_sure_multiple(&$vars) {
  foreach ($vars['titles'] as $id => $title) {
    $vars['titles'][$id] = l($title, '', array(
      'external' => TRUE,
      'fragment' => $id,
    ));
  }

  $vars['first_title'] = array_shift($vars['titles']);
  $vars['first_element_id'] = key($vars['elements']);
  $vars['first_element'] = array_shift($vars['elements']);
}

/**
 * Returns HTML for a progress bar.
 *
 * @fork of theme_progress_bar().
 */
function theme_be_sure_progress_bar($vars) {
  $output = '<div id="' . drupal_html_id('progress') . '" class="progress">';
  $output .= '<div class="bar"><div class="filled ' . $vars['class'] . '" style="width: ' . $vars['percent'] . '%"></div></div>';
  $output .= '<div class="percentage">' . $vars['percent'] . '%</div>';
  $output .= '<div class="message">' . $vars['message'] . '</div>';
  $output .= '</div>';

  return $output;
}
