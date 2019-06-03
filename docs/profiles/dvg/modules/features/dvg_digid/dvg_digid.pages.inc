<?php

/**
 * Very crude login confirmation page.
 */
function dvg_digid_login_login_confirm_page() {
  drupal_set_title(t('Log in with your DigiD credentials'));

  $query = (isset($_GET['destination'])) ? array('destination' => $_GET['destination']) : array();
  $query['confirm'] = 1;

  // Prevent indexing of this page.
  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'robots',
      'content' => 'noindex, nofollow',
    ),
  );
  drupal_add_html_head($element, 'dvg_digid_noindex');

  return l(t('DigiD login'), current_path(), array(
    'query' => $query,
    'attributes' => array('class' => array('digid-link')),
  ));
}

/**
 * Page callback to provide logging into Drupal with DigiD.
 */
function dvg_digid_login_page() {
  drupal_page_is_cacheable(FALSE);

  // Show a confirmation page?
  if (variable_get('dvg_digid_confirm', TRUE) && !isset($_GET['confirm'])) {
    return dvg_digid_login_login_confirm_page();
  }

  // Require authentication.
  _dvg_digid_user_login();
}
