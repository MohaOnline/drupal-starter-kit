<?php

use Drupal\campaignion_action\Redirects\Redirect;

/**
 * Theme callback implementation
 * Copied from webform.emails.inc and adapted
 */
function theme_campaignion_wizard_email_form($variables) {
  $form = $variables['form'];

  // Loop through fields, rendering them into radio button options.
  foreach (array('from_address', 'from_name') as $field) {

    $form[$field . '_custom']['#attributes']['class'][] = 'webform-set-active';
    $form[$field . '_custom']['#theme_wrappers'] = [];
    $form[$field . '_option']['custom']['#theme_wrappers'] = ['webform_inline_radio'];
    $form[$field . '_option']['custom']['#title'] = t('!title: !field', ['!title' => $form[$field . '_option']['custom']['#title'], '!field' => drupal_render($form[$field . '_custom'])]);

    if (isset($form[$field . '_option']['#options']['default'])) {
      $form[$field . '_option']['default']['#theme_wrappers'] = ['webform_inline_radio'];
    }
  }

  $details = '';
  $details .= drupal_render($form['from_address_option']);
  $details .= drupal_render($form['from_name_option']);
  $details .= drupal_render($form['subject_custom']);

  $form['details'] = array(
    '#type'        => 'fieldset',
    '#title'       => '',
    '#weight'      => 10,
    '#children'    => $details,
    '#collapsible' => FALSE,
    '#parents'     => array('details'),
    '#groups'      => array('details' => array()),
    '#attributes'  => array(),
  );

  // Re-sort the elements since we added the details fieldset.
  $form['#sorted'] = FALSE;
  $children = element_children($form, TRUE);
  return drupal_render_children($form, $children);

}

/**
 * Prepare variables for the thank you page summary on the confirmation step.
 *
 * Default template: campaignion-wizard-thank-summary.tpl.php.
 *
 * @param array $vars
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used are: #items, #node and #double_optin.
 *
 */
function template_preprocess_campaignion_wizard_thank_summary(&$vars) {
  $node = $vars['element']['#node'];
  $items = $vars['element']['#items'];
  foreach ($items as $delta => &$item) {
    switch ($item['type']) {
      case 'node':
        $item['node'] = node_load($item['node_reference_nid']);
        break;

      case 'redirect':
        $item['redirects'] = [
          '#theme' => 'links',
          '#links' => [],
        ];
        foreach (Redirect::byNid($node->nid, $delta) as $redirect) {
          $r = $redirect->toArray();
          $item['redirects']['#links'][] = [
            'title' => $r['prettyDestination'],
            'href' => $r['destination'],
          ];
        }
        break;
    }
  }

  $pages = [];
  if ($vars['element']['#double_optin']) {
    $vars['after_submit'] = $items[0];
    $vars['after_confirm'] = $items[1];
  }
  else {
    $vars['after_submit'] = $items[1];
    $vars['after_confirm'] = NULL;
  }
}
