<?php
/**
 * @file
 * Listing bean plugin.
 */

class WetKitSearchBean extends BeanPlugin {
  /**
   * Declares default block settings.
   */
  public function values() {
    $values = array(
      'search_placeholder' => array(
        'placeholder_text' => t('e.g. population'),
      ),
      'search_filter' => 'q',
    );

    return array_merge(parent::values(), $values);
  }

  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {
    $form = array();

    $form['search_placeholder'] = array(
      '#type' => 'fieldset',
      '#title' => t('Placeholder Value'),
      '#tree' => TRUE,
      '#weight' => -8,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['search_placeholder']['placeholder_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Enter the text to be used in the placeholder text area.'),
      '#default_value' => isset($bean->search_placeholder['placeholder_text']) ? $bean->search_placeholder['placeholder_text'] : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => FALSE,
    );

    $form['search_filter'] = array(
      '#type' => 'textfield',
      '#title' => t('Enter the filter name for search'),
      '#default_value' => isset($bean->search_filter) ? $bean->search_filter : 'q',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => FALSE,
    );

    return $form;
  }

  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {

    // Title Field.
    $title = field_get_items('bean', $bean, 'title_field');
    $title = field_view_value('bean', $bean, 'title_field', $title[0], array());
    $title = render($title);

    // Search Path.
    $search_path = field_get_items('bean', $bean, 'field_bean_search_path');
    $search_path = field_view_value('bean', $bean, 'field_bean_search_path', $search_path[0], array());
    $search_path = render($search_path);

    // Body Path.
    $body = field_get_items('bean', $bean, 'field_bean_wetkit_body');
    $body = field_view_value('bean', $bean, 'field_bean_wetkit_body', $body[0], array());
    $body = render($body);

    // Background Styles.
    $background = isset($bean->title_settings['backgrounds']) ? $bean->title_settings['backgrounds'] : '';

    // Placeholder Value.
    $placeholder = isset($bean->search_placeholder['placeholder_text']) ? $bean->search_placeholder['placeholder_text'] : '';

    // Filter Value.
    $filter = isset($bean->search_filter) ? $bean->search_filter : 'q';

    // Unique ID for Accessibility.
    $unique_id = 'gcwu-srch-delta-' . $bean->bid;

    // Reset content.
    $content = array();

    // Rendered markup.
    $markup = '<div class="wb-srch-multi"><form class="form-inline" method="get" action="' . $search_path . '">';
    $markup .= isset($body) ? '<div>' . $body . '</div>' : '';
    $markup .= '<div class="form-item form-group"><p>';
    $markup .= '<label class="wb-invisible" for="' . $unique_id . '">' . t('Search Data') . '</label>';
    $markup .= '<input title="" class="form-control search" placeholder="' . $placeholder . '" data-toggle="tooltip" type="search" id="' . $unique_id . '" name="' . $filter . '" value="" size="15" maxlength="128" autocomplete="off">';
    $markup .= '<button class="btn btn-primary btn-small form-submit" data-icon="search" type="submit" value="search"><span class="glyphicon-search glyphicon"></span><span class="wb-inv">' . t('Search') . '</span></button>';
    $markup .= '</p></div></form></div>';

    $content['search_bean_box']['#markup'] = $markup;

    return $content;
  }
}
