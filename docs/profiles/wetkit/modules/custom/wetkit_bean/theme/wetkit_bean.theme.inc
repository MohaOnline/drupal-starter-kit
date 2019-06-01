<?php
/**
 * @file
 * Bean example plugin theme templates for article listing.
 */

/**
 * Define theme templates.
 */
function theme_wetkit_highlight_settings(&$variables) {
  return l($variables['text'], $variables['path'], array('attributes' => array('class' => array('more-link'))));
}

/**
 * Themes a field collection to match a Tabbed Interface pattern.
 */
function theme_wetkit_slide_collection($variables) {
  $bean      = $variables['bean'];
  $items     = $variables['items'];
  $use_pager = $variables['use_pager'];

  // Collect image urls.
  $images = array();
  foreach ($items as $item_key => $data) {
    $key = array_keys($data['data']['entity']['field_collection_item']);
    $key = array_pop($key);
    $item = &$data['data']['entity']['field_collection_item'][$key];

    if (isset($item['field_slide_image']['#field_type'])) {
      // Only add url if an image has been added to this slide.
      if ($bean->data['image_style'] == '<none>') {
        $images[] = file_create_url($item['field_slide_image'][0]['#item']['uri']);
      }
      else {
        $images[] = image_style_url($item['field_slide_image'][0]['#image_style'], $item['field_slide_image'][0]['#item']['uri']);
      }
    } else {
      $images[] = '';
    }
  }

  // Collect title fields.
  $titles = array();
  foreach ($items as $item_key => $data) {
    $key = array_keys($data['data']['entity']['field_collection_item']);
    $key = array_pop($key);
    $item = $data['data']['entity']['field_collection_item'][$key];
    $titles[] = isset($item['field_slide_link']['#items'][0]['title']) ? $item['field_slide_link']['#items'][0]['title'] : '';
  }

  drupal_alter('wetkit_slide_slideshow', $titles, $items, $images, $bean);
  foreach ($items as $key => $data) {
    if (empty($images[$key])) {
      // This slide has no set image field, replace <figcaption> by a <section> element and add styling.
      if (isset($data['data']['entity']['field_collection_item'])) {
        $collection_id = key($data['data']['entity']['field_collection_item']);
        $instance_settings =& $items[$key]['data']['entity']['field_collection_item'][$collection_id]['#groups']['group_bean_slide_caption']->format_settings['instance_settings'];
        $instance_settings['element'] = 'section';
        $instance_settings['classes'] = 'mrgn-lft-md mrgn-tp-md mrgn-rght-md';
      }
    }
    $items[$key] = drupal_render($data['data']);
  }

  // Add unique id.
  $id = drupal_html_id('bean-slide');

  // Add settings for this slideshow.
  $data = $bean->data;
  $data['id'] = $id;
  $data['images'] = $images;

  // Support Tabbed Interface.
  if ($data['slideshow_plugin'] === 'wetkit_slide:tabbedinterface') {

    $settings = isset($data['plugin_settings']) ? $data['plugin_settings'] : array();

    // Plugin Effects.
    $effects = array(
      'carousel' => isset($settings['carousel']) ? $settings['carousel'] : '',
      'autoplay' => isset($settings['autoplay']) && $settings['autoplay'] ? 'playing' : '',
      'rotate_speed' => isset($settings['rotate_speed']) ? $settings['rotate_speed'] : '',
      'rotate_speed_custom' => isset($settings['rotate_speed_custom']) ? $settings['rotate_speed_custom'] : 3, // defaults to 3 seconds
      'animate_effect' => isset($settings['animate_effect']) ? $settings['animate_effect'] : '',
    );

    $tabs_attributes['class'] = array('wb-tabs', $effects['carousel'], $effects['autoplay']);

    if ($effects['rotate_speed'] == 'custom') {
      $tabs_attributes['data-wb-tabs']['interval'] = $effects['rotate_speed_custom'];
    }
    elseif (!empty($effects['rotate_speed'])) {
      $tabs_attributes['class'][] = $effects['rotate_speed'];
    }

    $title = t('Tab List');
    $header = theme('wetkit_slide_header', array('items' => $titles, 'title' => $title, 'type' => 'ul', 'attributes' => array('role' => array('tablist'))));
    $output = theme('wetkit_slide_tab_panels', array('images' => $images, 'items' => $items, 'title' => $title, 'animate_effect' => $effects['animate_effect'], 'attributes' => array('class' => array('tabpanels'))));
    $output = '<div' . drupal_attributes($tabs_attributes) . '>' . $header . $output . '</div>';
  }

  // Support FlexSlider and jQuery.Cycle.
  else {
    $output = theme('item_list', array('items' => $items, 'attributes' => array('class' => array('slides'))));
    $output = '<div id="' . $id . '" class="bean-slide clearfix">' . $output . '</div>';
  }

  drupal_add_js(array(
    'wetkitbean' => array(
      'slideshows' => array($data),
    ),
  ), 'setting');

  return $output;
}

/**
 * Themes the Tabbed Interface header.
 */
function theme_wetkit_slide_header($variables) {
  $items = $variables['items'];
  $title = $variables['title'];
  $type = $variables['type'];
  $attributes = $variables['attributes'];

  $output = '';
  if (!empty($items)) {
    $output .= "<$type" . drupal_attributes($attributes) . '>';

    $i = 0;
    foreach ($items as $item) {
      $i++;
      $li_classes = ($i == 1) ? ' class="active"' : '';
      $output .= '<li' . $li_classes . '><a href="#panel' . $i . '" title="' . $item . '">' . $i . "</a></li>\n";
    }

    $output .= "</$type>";
  }
  return $output;
}

/**
 * Themes the Tabbed Interface panels.
 */
function theme_wetkit_slide_tab_panels($variables) {
  $items = $variables['items'];
  $images = $variables['images'];
  $title = $variables['title'];
  $animate_effect = $variables['animate_effect'];
  $attributes = $variables['attributes'];

  $output = '';
  if (!empty($items)) {
    $output .= '<div' . drupal_attributes($attributes) . '>';
    $num_items = count($items);
    $i = 0;
    foreach ($items as $item_key => $item) {
      $attributes = array();
      $children = array();
      $data = '';
      $i++;
      if (is_array($item)) {
        foreach ($item as $key => $value) {
          if ($key == 'data') {
            $data = $value;
          }
          elseif ($key == 'children') {
            $children = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        if (empty($images[$item_key])){
          // Don't wrap in a <figure> element when no image is present.
          $data = $item;
        }
        else {
          $data = '<figure>' . $item . '</figure>';
        }
      }
      $attributes['id'][] = "panel$i";
      $attributes['role'][] = "tabpanel";

      if ($i == 1) {
        $attributes['class'][] = 'in';
      }
      else {
        $attributes['class'][] = 'out';
      }
      $attributes['class'][] = $animate_effect;
      $output .= '<div' . drupal_attributes($attributes) . '>' . $data . "</div>\n";

    }
    $output .= '</div>';
  }
  return $output;
}

/**
 * Themes a field collection to match a Video pattern.
 */
function theme_wetkit_media_collection($variables) {
  $bean      = $variables['bean'];
  $items     = $variables['items'];
  $use_pager = $variables['use_pager'];

  $media_attributes = array();
  $source_attributes = array();
  $track_attributes = array();
  $wrapper_attributes = array();
  $type_of_media = '';
  $output = '';

  // Collect media.
  foreach ($items as $item_key => $data) {
    $key = array_keys($data['data']['entity']['field_collection_item']);
    $key = array_pop($key);
    $item = $data['data']['entity']['field_collection_item'][$key];

    $source_attributes = array(
      'src' => file_create_url($item['field_media_file'][0]['#file']->uri),
      'type' => $item['field_media_file'][0]['#file']->filemime,
    );

    $audio_types = array(
      'audio/mpeg',
      'audio/ogg',
    );
    if (in_array($item['field_media_file'][0]['#file']->filemime, $audio_types)) {
      $type_of_media = 'audio';
    }
    else {
      $type_of_media = 'video';
    }
  }

  // Collect the poster.
  $poster = array();
  foreach ($items as $item_key => $data) {
    $key = array_keys($data['data']['entity']['field_collection_item']);
    $key = array_pop($key);
    $item = &$data['data']['entity']['field_collection_item'][$key];

    if ($bean->data['image_style'] == '<none>') {
      $poster[] = file_create_url($item['field_media_poster'][0]['#item']['uri']);
    }
    else {
      $poster[] = image_style_url($item['field_media_poster'][0]['#image_style'], $item['field_media_poster'][0]['#item']['uri']);
    }
  }

  // Attribute handling.
  if ($type_of_media == 'video') {
    $media_attributes['poster'] = $poster;
  }
  $wrapper_attributes['class'] = 'wb-mltmd ' . $bean->media_span;

  // Collect the caption.
  foreach ($items as $item_key => $data) {
    $key = array_keys($data['data']['entity']['field_collection_item']);
    $key = array_pop($key);
    $item = $data['data']['entity']['field_collection_item'][$key];

    $track_attributes = array(
      'data-type' => $item['field_media_caption'][0]['#file']->filemime,
      'kind' => 'captions',
      'src' => file_create_url($item['field_media_caption'][0]['#file']->uri),
    );
  }

  // Final Render.
  if ($type_of_media == 'audio') {
    $output .= '<div' . drupal_attributes($wrapper_attributes) . '><audio' . drupal_attributes($media_attributes) . '>';
    $output .= '<source' . drupal_attributes($source_attributes) . ' />';
    $output .= '</audio></div>';
  }
  else {
    $output .= '<div' . drupal_attributes($wrapper_attributes) . '><video' . drupal_attributes($media_attributes) . '>';
    $output .= '<source' . drupal_attributes($source_attributes) . ' />';
    $output .= '<track' . drupal_attributes($track_attributes) . ' />';
    $output .= '</video></div>';
  }

  return $output;
}

/**
 * Themes a slideout tab.
 */
function theme_wetkit_slideout(&$variables) {
  $show_title = $variables['show_title'];
  $hide_title = $variables['hide_title'];
  $icon = $variables['icon'];
  $body = $variables['content'];

  if ($variables['slideout_render'] == 'form') {
    $body = drupal_render($body);
  }
  else {
    $body = drupal_render_children($body);
  }

  $output = '<div id="slide-panel" class="">';
  $output .= '<a href="#" class="btn btn-primary" id="opener">' . theme('icon', $icon) . '</a>';
  $output .= $body;
  $output .= '</div>';

  return $output;
}
