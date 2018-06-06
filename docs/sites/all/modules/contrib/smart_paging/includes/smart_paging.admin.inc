<?php
// $Id: smart_paging.admin.inc,v 1.1.2.2 2010/12/08 22:42:42 arpeggio Exp $

/**
 * @file
 * Admin interface callbacks/handlers to configure Smart Paging.
 */

/**
 * Smart Paging administration settings.
 *
 * @return
 *   Forms for store administrator to set configuration options.
 */
function smart_paging_admin_settings($form, &$form_state) {
  $form['smart_paging_enable_clean_url'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable clean URL pagination'),
    '#default_value' => variable_get('smart_paging_enable_clean_url', TRUE),
  );
  $form['smart_paging_path_prefix'] = array(
    '#type'          => 'textfield',
    '#title'         => t('URL paging prefix'),
    '#default_value' => variable_get('smart_paging_path_prefix', 'page'),
    '#description'   => t('It is the paging prefix (default is "page") shown at URL when navigating Smart Paging enabled pages: %url. Note: This works only if clean URL pagination is enabled.', array('%url' => 'http://example.com/node/1/<URL paging prefix>/0/1')),
    '#size'          => 50,
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_paging_enable_clean_url"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['smart_paging_use_js_pager'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use javascript pager as progressive enhancement for Drupal native pager'),
    '#description'   => t('Note: This works only if clean URL pagination is enabled.'),
    '#default_value' => variable_get('smart_paging_use_js_pager', TRUE),
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_paging_enable_clean_url"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['smart_paging_use_link_rel'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Pagination with rel="next" and rel="prev"'),
    '#default_value' => variable_get('smart_paging_use_link_rel', TRUE),
    '#description'   => t('This adds next and prev hints according to !google.', array('!google' => l(t('Google pagination policies'), 'http://googlewebmastercentral.blogspot.co.uk/2011/09/pagination-with-relnext-and-relprev.html'))),
  );
  $form['smart_paging_use_link_canonical'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Allow smart paging to set the canonical link'),
    '#default_value' => variable_get('smart_paging_use_link_canonical', TRUE),
    '#description'   => t('Uncheck if you do not want smart paging to set the canonical link.'),
  );
  $form['smart_paging_use_nopaging_canonical'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use the unpaged version as canonical'),
    '#default_value' => variable_get('smart_paging_use_nopaging_canonical', FALSE),
    '#description'   => t('Use {page one}?nopaging=1 as the canonical link for all pages.'),
  );  
  // Container for default values
  $form['smart_paging_defaults'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Default values'),
    '#description' => t('The selected values will be the initial values for all !smart_paging enabled entities. However, these values can be customized during content editing.', array('!smart_paging' => l(t('Smart Paging input filter'), 'admin/config/content/formats'))),
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE,
    '#attached'    => array(
      'js' => array(drupal_get_path('module', 'smart_paging') . '/js/smart_paging-global-config.js'),
    ),
  );
  $form['smart_paging_defaults']['smart_paging_method'] = array(
    '#type'          => 'select', 
    '#title'         => t('Default page break method'),
    '#default_value' => variable_get('smart_paging_method', SMART_PAGING_PLACEHOLDER_METHOD),
    '#options'       => _smart_paging_method_list(),
  );
  $form['smart_paging_defaults']['smart_paging_pagebreak'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Page break placeholder'),
    '#default_value' => variable_get('smart_paging_pagebreak', '<!--pagebreak-->'),
    '#description'   => t('HTML comment or valid HTML tag with unique identifier, eg. &lt;hr class="pagebreak" /&gt; or &lt;!--pagebreak--&gt.'),
    '#size'          => 50,
  );
  $form['smart_paging_defaults']['smart_paging_character_count'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Character limit'),
    '#description'   => t('Number of characters that will be shown for each page when "Automatic page break by character limit" is selected.'),
    '#default_value' => variable_get('smart_paging_character_count', SMART_PAGING_MAX_CHAR_LIMIT),
    '#size'          => 50,
  );
  $form['smart_paging_defaults']['smart_paging_word_count'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Word limit'),
    '#description'   => t('Number of words that will be shown for each page when "Automatic page break by word limit" is selected.'),
    '#default_value' => variable_get('smart_paging_word_count', SMART_PAGING_MAX_WORD_LIMIT),
    '#size'          => 50,
  );
  $form['smart_paging_defaults']['smart_paging_title_display_suffix'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display content title suffix'),
    '#description'   => t("Unchecking this option will stop display of content title suffix on web pages"),
    '#default_value' => variable_get('smart_paging_title_display_suffix', TRUE),
  );
  $form['smart_paging_defaults']['smart_paging_title_suffix'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Content title suffix'),
    '#description'   => t("Text that will appear next to content's sub pages title (default is %suffix), eg. Title%suffix 2, Title%suffix 3 and so on...", array('%suffix' => t(': Page '))),
    '#default_value' => variable_get('smart_paging_title_suffix', ': Page '),
    '#size'          => 50,
    '#states'        => array(
      'visible' => array(
        ':input[name="smart_paging_title_display_suffix"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['#validate'][] = 'smart_paging_admin_settings_validate';
  return system_settings_form($form);
}

function smart_paging_admin_settings_validate($form, &$form_state) {
  if (strpos($form_state['values']['smart_paging_pagebreak'], '<') === FALSE || strpos($form_state['values']['smart_paging_pagebreak'], '>') != (drupal_strlen($form_state['values']['smart_paging_pagebreak']) - 1)) {
    form_set_error('smart_paging_pagebreak', t('Page break placeholder is not a valid HTML tag or comment.'));
  }
  if (!is_numeric($form_state['values']['smart_paging_character_count'])) {
    form_set_error('smart_paging_character_count', t('Character limit should be numeric value.'));
  }
  if (!is_numeric($form_state['values']['smart_paging_word_count'])) {
    form_set_error('smart_paging_word_count', t('Word limit should be numeric value.'));
  }
}