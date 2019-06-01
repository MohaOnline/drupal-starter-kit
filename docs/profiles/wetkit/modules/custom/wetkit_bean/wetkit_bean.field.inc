<?php

/**
 * @file
 * Implement a bean field formater.
 */


/**
 * Implements hook_field_formatter_info().
 */
function wetkit_bean_field_formatter_info() {
  return array(
    'wetkit_bean_linked' => array(
      'label' => t('Linked and wrapped bean'),
      'field types' => array('text'),
      'settings' => array('wetkit_bean_style' => '', 'wetkit_bean_link' => '', 'wetkit_bean_class' => ''),
    ),
    'wetkit_media_collection' => array(
      'label' => t('Media'),
      'field types' => array('field_collection'),
    ),
    'wetkit_slide_collection' => array(
      'label' => t('Slideshow'),
      'field types' => array('field_collection'),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function wetkit_bean_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $settings = $instance['display'][$view_mode]['settings'];
  $element = array();

  $wrap_tags = array(
    '_none' => t('- None -'),
    'div' => t('DIV'),
    'h1' => t('H1'),
    'h2' => t('H2'),
    'h3' => t('H3'),
    'h4' => t('H4'),
    'h5' => t('H5'),
    'h6' => t('H6'),
    'span' => t('SPAN'),
  );
  $element['wetkit_bean_style'] = array(
    '#title' => t('Wrap title in tag'),
    '#type' => 'select',
    '#default_value' => !empty($settings['wetkit_bean_style']) ? $settings['wetkit_bean_style'] : '_none',
    '#options' => $wrap_tags,
  );

  $link_types = array(
    'content' => t('Content'),
  );
  $element['wetkit_bean_link'] = array(
    '#title' => t('Link title to'),
    '#type' => 'select',
    '#default_value' => $settings['wetkit_bean_link'],
    '#empty_option' => t('Nothing'),
    '#options' => $link_types,
  );

  $element['wetkit_bean_class'] = array(
    '#title' => t('Tag classes'),
    '#type' => 'textfield',
    '#description' => t('A CSS class to use in the wrapper tag for the title.'),
    '#default_value' => $settings['wetkit_bean_class'],
    '#element_validate' => array('_wetkit_bean_validate_class'),
  );

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function wetkit_bean_field_formatter_settings_summary($field, $instance, $view_mode) {
  $settings = $instance['display'][$view_mode]['settings'];
  $summary = array();

  $tag = isset($settings['wetkit_bean_style']) && $settings['wetkit_bean_style'] != '' && $settings['wetkit_bean_style'] != '_none' ? $settings['wetkit_bean_style'] : t('- None -');
  $summary[] = t('Title wrap tag: @tag', array('@tag' => $tag));

  $link_types = array(
    'content' => t('Linked to content'),
  );

  // Display this setting only if field is linked.
  if (!empty($settings) && isset($link_types[$settings['wetkit_bean_link']])) {
    $summary[] = $link_types[$settings['wetkit_bean_link']];
  }

  // Display this setting only if wrapper has a class.
  if (isset($settings['wetkit_bean_class']) && $settings['wetkit_bean_class'] != '_none' && $settings['wetkit_bean_class'] != '') {
    $summary[] = t('Wrap tag classes: @classes', array('@classes' => $settings['wetkit_bean_class']));
  }

  return implode('<br />', $summary);
}


/**
 * Implements hook_field_formatter_view().
 */
function wetkit_bean_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  switch ($display['type']) {

    // Bean Linked.
    case 'wetkit_bean_linked':

      $settings = $display['settings'];
      $output = isset($items[0]) ? $items[0]['safe_value'] : '';

      if (!empty($output) && $settings['wetkit_bean_link'] == 'content') {
        $uri = entity_uri($entity_type, $entity);
        $output = l($output, $uri['path'], array('html' => TRUE));
      }

      $wrap_tag = empty($settings['wetkit_bean_style']) ? '_none' : $settings['wetkit_bean_style'];

      if ($wrap_tag != '_none') {
        $variables = array(
          'element' => array(
            '#tag' => $wrap_tag,
            '#value' => $output,
          ),
        );

        if (isset($entity->wetkit_bean_title_settings['classes'])) {
          $settings['wetkit_bean_class'] = $entity->wetkit_bean_title_settings['classes'];
        }

        if (!empty($settings['wetkit_bean_class'])) {
          $variables['element']['#attributes'] = array('class' => $settings['wetkit_bean_class']);
        }

        $output = theme('html_tag', $variables);
      }

      $element = array(
        array(
          '#markup' => $output,
        ),
      );
      break;

    // Slideshow.
    case 'wetkit_slide_collection':

      // Determine if the entity is a Bean Slide.
      $is_bean_slide = FALSE;
      if ($entity_type === 'bean' && ($info = $entity->getInfo())) {
        $is_bean_slide = $info['name'] === 'wetkit_slide';
      }

      $list_items = array();
      foreach ($items as $delta => $item) {
        if ($field_collection = field_collection_field_get_entity($item)) {
          // Useful data for those interested in altering.
          $list_item = array();
          $list_item['entity'] = $field_collection->view();
          $list_item['#entity_type'] = $entity_type;
          $list_item['#field'] = &$field;
          $list_item['#instance'] = &$instance;
          $list_item['#display'] = &$display;
          $list_item['#field_collection_item'] = $field_collection;
          $list_item['#theme_wrappers'] = array('field_collection_view');
          $list_item['#attributes']['class'][] = 'field-collection-view';

          // Add admin links for each item as contextual links.
          $list_item['contextual_links'] = array(
            '#type' => 'contextual_links',
            '#contextual_links' => array(
              'field_collection' => array(field_collection_field_get_path($field_collection->fieldInfo()), array($field_collection->item_id)),
            ),
            '#element' => $list_item,
          );

          if ($is_bean_slide) {
            // There's always one entry in field_collection_item, but its key varies.
            $key = array_keys($list_item['entity']['field_collection_item']);
            $key = array_pop($key);

            // Set (okay, monkey-patch) the image style for each slide image.
            $image_style = &$list_item['entity']['field_collection_item'][$key]['field_slide_image'][0]['#image_style'];
            if ($entity->data['image_style'] == '<none>') {
              $image_style = NULL;
            }
            else {
              $image_style = $entity->data['image_style'];
            }
          }

          $list_item['#attributes']['class'][] = 'contextual-links-region';
          $list_items[] = array(
            'data' => $list_item,
            'class' => array('item-' . $delta),
          );
        }
      }

      $element = array(
        '#theme' => 'wetkit_slide_collection',
        '#bean' => $entity,
        '#items' => $list_items,
        '#use_pager' => TRUE,
      );
      break;

    // Media.
    case 'wetkit_media_collection':

      // Determine if the entity is a Bean Slide.
      $is_bean_media = FALSE;
      if ($entity_type === 'bean' && ($info = $entity->getInfo())) {
        $is_bean_media = $info['name'] === 'wetkit_media';
      }

      $list_items = array();
      foreach ($items as $delta => $item) {
        if ($field_collection = field_collection_field_get_entity($item)) {
          // Useful data for those interested in altering.
          $list_item = array();
          $list_item['entity'] = $field_collection->view();
          $list_item['#entity_type'] = $entity_type;
          $list_item['#field'] = &$field;
          $list_item['#instance'] = &$instance;
          $list_item['#display'] = &$display;
          $list_item['#field_collection_item'] = $field_collection;
          $list_item['#theme_wrappers'] = array('field_collection_view');
          $list_item['#attributes']['class'][] = 'field-collection-view';

          // Add admin links for each item as contextual links.
          $list_item['contextual_links'] = array(
            '#type' => 'contextual_links',
            '#contextual_links' => array(
              'field_collection' => array(field_collection_field_get_path($field_collection->fieldInfo()), array($field_collection->item_id)),
            ),
            '#element' => $list_item,
          );

          if ($is_bean_media) {
            // There's always one entry in field_collection_item, but its key varies.
            $key = array_keys($list_item['entity']['field_collection_item']);
            $key = array_pop($key);

            // Set (okay, monkey-patch) the image style for each slide image.
            $image_style = &$list_item['entity']['field_collection_item'][$key]['field_media_poster'][0]['#image_style'];
            if ($entity->data['image_style'] == '<none>') {
              $image_style = NULL;
            }
            else {
              $image_style = $entity->data['image_style'];
            }
          }

          $list_item['#attributes']['class'][] = 'contextual-links-region';
          $list_items[] = array(
            'data' => $list_item,
            'class' => array('item-' . $delta),
          );
        }
      }

      $element = array(
        '#theme' => 'wetkit_media_collection',
        '#bean' => $entity,
        '#items' => $list_items,
        '#use_pager' => TRUE,
      );
      break;
  }

  return $element;
}

/**
 * Validate that a space-separated list of values are lowercase and appropriate for use as HTML classes.
 *
 * @see wetkit_bean_field_formatter_settings_form()
 */
function _wetkit_bean_validate_class($element, &$form_state) {
  $value = drupal_array_get_nested_value($form_state['values'], $element['#parents']);
  $classes = explode(' ', $value);

  foreach ($classes as $class) {
    if ($class != drupal_html_class($class)) {
      form_error($element, t('Wrapper classes contain illegal characters; classes should be lowercase and may contain letters, numbers, and dashes.'));
      return;
    }
  }
}
