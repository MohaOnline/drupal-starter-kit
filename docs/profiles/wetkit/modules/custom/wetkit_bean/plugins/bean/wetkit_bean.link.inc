<?php
/**
 * @file
 * A bean link plugin.
 */

class WetKitLinkBean extends BeanPlugin {

  /**
   * Implements BeanPlugin::values().
   */
  public function values() {
    $values = array(
      'links_settings' => array(
        'links_render' => 'list',
      ),
    );

    return array_merge(parent::values(), $values);
  }

  /**
   * Implements BeanPlugin::form().
   */
  public function form($bean, $form, &$form_state) {

    $form['links_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Links Settings'),
      '#tree' => TRUE,
      '#weight' => -8,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['links_settings']['links_render'] = array(
      '#type' => 'select',
      '#title' => t('Links render type'),
      '#description' => t('Decide how the links should be displayed.'),
      '#options' => array(
        'follow' => t('Follow'),
        'list' => t('List'),
      ),
      '#default_value' => isset($bean->links_settings['links_render']) ? $bean->links_settings['links_render'] : '',
      '#required' => TRUE,
    );

    return $form;
  }

  /**
   * Implements BeanPlugin::view().
   */
  public function view($bean, $content, $view_mode = 'full', $langcode = NULL) {

    $markup = '';
    $links_markup = '';

    // Links render type.
    $render_type = isset($bean->links_settings['links_render']) ? $bean->links_settings['links_render'] : '';

    // Tile Field.
    $title = field_get_items('bean', $bean, 'title_field');
    $title = field_view_value('bean', $bean, 'title_field', $title[0], array());
    $title = render($title);

    // Links Field.
    $links = field_get_items('bean', $bean, 'field_bean_link');
    foreach ($links as $key => $val) {
      $links_tmp = field_view_value('bean', $bean, 'field_bean_link', $links[$key], array());
      $links_markup .= '<li>' . render($links_tmp) . '</li>';
    }

    // Reset content.
    $content = array();

    // Rendered markup.
    if ($render_type == 'follow') {
      $markup = '<section class="pillars mrgn-tp-md mrgn-bttm-md">';
      $markup .= '<h2>' . $title . '</h2>';
      $markup .= '<ul>' . $links_markup . '</ul>';
      $markup .= '</section>';
    }
    else {
      $markup .= '<ul class="mrgn-bttm-0 list-unstyled">' . $links_markup . '</ul>';
    }

    $content['links']['#markup'] = $markup;

    return $content;
  }

  /**
   * Implements BeanPlugin::submit().
   */
  public function submit(Bean $bean) {
    return parent::submit($bean);
  }
}
