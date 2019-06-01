<?php
/**
 * @file
 * Listing bean plugin.
 */

class WetKitHighlightBean extends BeanPlugin {
  /**
   * Declares default block settings.
   */
  public function values() {
    $values = array(
      'image_style' => 'highlighted',
      'wetkit_bean_highlight_type' => array(
        'classes' => '',
        'type' => 'global',
      ),
    );

    return array_merge(parent::values(), $values);
  }

  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {
    $form = array();

    $form['image_style'] = array(
      '#type' => 'select',
      '#title' => t('Image style'),
      '#options' => drupal_map_assoc(array_keys(image_styles())),
      '#default_value' => $bean->image_style,
      '#empty_value' => '<none>',
      '#weight' => 2,
    );
    $form['wetkit_bean_highlight_type'] = array(
      '#type' => 'fieldset',
      '#title' => t('Configure Highlight Bean'),
      '#tree' => TRUE,
      '#weight' => 1,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['wetkit_bean_highlight_type']['classes'] = array(
      '#description' => t('CSS classes to be added to the title field.'),
      '#type' => 'textfield',
      '#title' => t('CSS'),
      '#default_value' => isset($bean->wetkit_bean_highlight_type['classes']) ? $bean->wetkit_bean_highlight_type['classes'] : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => FALSE,
    );
    $form['wetkit_bean_highlight_type']['type'] = array(
      '#type' => 'select',
      '#title' => t('How should the highlight type be rendered?'),
      '#options' => array(
        'global' => t('Global'),
        'site' => t('Site'),
      ),
      '#default_value' => isset($bean->wetkit_bean_highlight_type['type']) ? $bean->wetkit_bean_highlight_type['type'] : 'global',
    );

    return $form;
  }

  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {

    // Settings.
    $image_style = isset($bean->image_style) ? $bean->image_style : '';
    if ($image_style == '<none>') {
      $image_style = '';
    }
    $classes = isset($bean->wetkit_bean_highlight_type['classes']) ? $bean->wetkit_bean_highlight_type['classes'] : '';
    $type = isset($bean->wetkit_bean_highlight_type['type']) ? $bean->wetkit_bean_highlight_type['type'] : '';

    // Tile Field.
    $title = field_get_items('bean', $bean, 'title_field');
    $title = field_view_value('bean', $bean, 'title_field', $title[0], array());
    $title = render($title);

    // Link Field.
    $link = field_get_items('bean', $bean, 'field_bean_highlight_link');
    $link = field_view_value('bean', $bean, 'field_bean_highlight_link', $link[0], array());
    $linktitle = $link['#element']['title'];
    $linkurl = $link['#element']['display_url'];

    // Image Field.
    $image = field_get_items('bean', $bean, 'field_bean_highlight_image');
    $image = field_view_value('bean', $bean, 'field_bean_highlight_image', $image[0], array('settings' => array('image_style' => $image_style)));
    $image = render($image);

    // Body Field.
    $body = field_get_items('bean', $bean, 'field_bean_highlight_description');
    $body = field_view_value('bean', $bean, 'field_bean_highlight_description', $body[0], array());
    $body = render($body);

    // Reset content.
    $content = array();

    // Rendered markup.
    if ($type == 'site') {
      $markup = '<section class="gc-nttvs"><a href="' . $linkurl . '" rel="external" title="' . $linktitle . '">';
      $markup .= $image;
      $markup .= '<h4>' . $title . '</h4>';
      $markup .= '</a>';
      $markup .= $body . '</section>';
    }
    else {
      $markup = '<section class="col-lg-4 col-md-6"><a href="' . $linkurl . '" rel="external" title="' . $linktitle . '">';
      $markup .= '<h3 class="' . $classes . '">' . $title . '</h3>';
      $markup .= $image;
      $markup .= '</a>';
      $markup .= $body . '</section>';
    }
    $content['highlight']['#markup'] = $markup;

    return $content;
  }
}
