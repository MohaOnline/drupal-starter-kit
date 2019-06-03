<?php
/**
 * @file
 * Displays the summary message for thank you pages and redirects.
 *
 * Available variables:
 * - $element: The renderable array.
 * - $after_submit: The field item that’s evaluated after form-submit.
 * - $after_confirm: The field item that’s evaluated after clicking on a
 *   confirmation link.
 *
 * @see template_preprocess_campaignion_wizard_thank_summary()
 *
 * @ingroup themeable
 */
?>
<?php
if ($after_submit['type'] == 'node') {
  echo t('After submitting the last form step your supporters will see the page <a href="@page_href">@page_title</a>.', [
    '@page_href' => url("node/{$after_submit['node_reference_nid']}"),
    '@page_title' => $after_submit['node']->title,
  ]);
}
else {
  echo t('After submitting the last form step your supporters will be redirected to one of the following pages:');
  echo render($after_submit['redirects']);
}
?>
<?php
if ($after_confirm) {
  if ($after_confirm['type'] == 'node') {
    echo t('After clicking on the confirmation link your supporters will see the page <a href="@page_href">@page_title</a>.', [
      '@page_href' => url("node/{$after_confirm['node_reference_nid']}"),
      '@page_title' => $after_confirm['node']->title,
    ]);
  }
  else {
    echo t('After clicking on the confirmation link your supporters will be redirected to one of the following pages:');
    echo render($after_confirm['redirects']);
  }

}
?>
