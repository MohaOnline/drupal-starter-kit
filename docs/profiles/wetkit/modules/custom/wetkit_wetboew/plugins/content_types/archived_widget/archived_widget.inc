<?php
/**
 * @file
 * A panel plugin to provide administrative information about the site's
 * general widget panes.
 */

$plugin = array(
  'title' => t('Archived Widget Pane'),
  'description' => t('This will generate an archived status message when a piece of content is flagged as being archived.'),
  'content_types' => array('archived_widget'),
  'category' => t('WetKit'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'render callback' => 'wetkit_wetboew_archived_widget_content_type_render',
  'edit form' => 'wetkit_wetboew_archived_widget_edit_form',
  'single' => TRUE,
);

/**
 * Custom Pane for Archived Messages.
 */
function wetkit_wetboew_archived_widget_content_type_render($subtype, $conf, $panel_args, $context) {

  // First check that we have a proper context.
  if (!empty($context) && empty($context->data)) {
    return;
  }

  // Get the Node and confirm whether it is flagged as archive.
  $node = isset($context->data) ? clone($context->data) : NULL;
  $field_archived = field_get_items('node', $node, 'field_archived_content');

  // Render the archived widget markup.
  if ($field_archived['0']['value'] == '1') {
    $pane = new stdClass();
    $pane->content = '<section id="archived" class="alert alert-warning wb-inview" data-inview="archived-bnr">
      <h2>' . t('This page has been archived on the Web.') . '</h2>
      <p>' . t('Information identified as archived is provided for reference, research or recordkeeping purposes. It is not subject to the Government of Canada Web Standards and has not been altered or updated since it was archived. Please contact us to request a format other than those available.') . '</p>
    </section>
    <section id="archived-bnr" class="wb-overlay modal-content overlay-def wb-bar-t">
      <header><h2 class="wb-inv">' . t('Archived') . '</h2></header>
      <p><a href="#archived">' . t('This page has been archived on the Web.') . '</a></p>
    </section>';
    return $pane;
  }
  else {
    return;
  }
}

/**
 * Edit callback edit form for the content type.
 */
function wetkit_wetboew_archived_widget_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}

/**
 * Edit callback submit form for the content type.
 */
function wetkit_wetboew_archived_widget_edit_form_submit($form, &$form_state) {
  foreach (element_children($form) as $key) {
    if (!empty($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}
