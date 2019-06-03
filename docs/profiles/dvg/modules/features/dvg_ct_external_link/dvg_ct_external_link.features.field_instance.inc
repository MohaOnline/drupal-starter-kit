<?php
/**
 * @file
 * dvg_ct_external_link.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function dvg_ct_external_link_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-external_link-field_alternate_keywords'.
  $field_instances['node-external_link-field_alternate_keywords'] = array(
    'bundle' => 'external_link',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Use alternative writing styles of keywords, so this page will score higher on internal search results.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'search_results' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_alternate_keywords',
    'label' => 'Alternate keywords',
    'required' => 0,
    'settings' => array(
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'node-external_link-field_boost_keywords'.
  $field_instances['node-external_link-field_boost_keywords'] = array(
    'bundle' => 'external_link',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'These keywords have a higher boost, so the content is shown higher in the search results.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'search_results' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_boost_keywords',
    'label' => 'Boost Keywords',
    'required' => 0,
    'settings' => array(
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 10,
    ),
  );

  // Exported field_instance: 'node-external_link-field_external_url'.
  $field_instances['node-external_link-field_external_url'] = array(
    'bundle' => 'external_link',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'A link to an external website. For example, <em>http://domain.com</em>.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => 1,
      ),
      'search_results' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_external_url',
    'label' => 'External URL',
    'required' => 1,
    'settings' => array(
      'absolute_url' => 1,
      'attributes' => array(
        'class' => '',
        'configurable_class' => 0,
        'configurable_title' => 0,
        'rel' => 'nofollow',
        'target' => 'default',
        'title' => '',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'rel_remove' => 'default',
      'title' => 'none',
      'title_label_use_field_label' => 0,
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'link',
      'settings' => array(),
      'type' => 'link_field',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-external_link-field_menu_description'.
  $field_instances['node-external_link-field_menu_description'] = array(
    'bundle' => 'external_link',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'search_results' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_menu_description',
    'label' => 'Menu description',
    'required' => 1,
    'settings' => array(
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 3,
      ),
      'type' => 'text_textarea',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-external_link-field_menu_image'.
  $field_instances['node-external_link-field_menu_image'] = array(
    'bundle' => 'external_link',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => 'menu_image',
        ),
        'type' => 'image',
        'weight' => 0,
      ),
      'search_results' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_menu_image',
    'label' => 'Menu image',
    'required' => 0,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => '',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '446x283',
      'title_field' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'media',
      'settings' => array(
        'allowed_schemes' => array(
          'ef' => 0,
          'private' => 0,
          'public' => 'public',
        ),
        'allowed_types' => array(
          'dvg_document' => 0,
          'dvg_image' => 'dvg_image',
          'file_category_item' => 0,
          'pdf' => 0,
        ),
        'browser_plugins' => array(
          'dvg_media_browser--media_browser_1' => 'dvg_media_browser--media_browser_1',
          'dvg_media_browser--media_browser_my_files' => 0,
          'upload' => 'upload',
        ),
        'manualcrop_crop_info' => 0,
        'manualcrop_default_crop_area' => 0,
        'manualcrop_enable' => 1,
        'manualcrop_inline_crop' => 0,
        'manualcrop_instant_crop' => FALSE,
        'manualcrop_instant_preview' => 0,
        'manualcrop_keyboard' => 1,
        'manualcrop_maximize_default_crop_area' => 0,
        'manualcrop_require_cropping' => array(
          'menu_image' => 'menu_image',
        ),
        'manualcrop_styles_list' => array(
          'menu_image' => 'menu_image',
        ),
        'manualcrop_styles_mode' => 'include',
        'manualcrop_thumblist' => 0,
      ),
      'type' => 'media_generic',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-external_link-field_search_result'.
  $field_instances['node-external_link-field_search_result'] = array(
    'bundle' => 'external_link',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The displayed text when a visitor sees this page as a given search result.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'search_results' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_search_result',
    'label' => 'Search result text',
    'required' => 1,
    'settings' => array(
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 8,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Alternate keywords', array(), array('context' => 'field_alternate_keywords:external_link:label'));
  t('Use alternative writing styles of keywords, so this page will score higher on internal search results.', array(), array('context' => 'field_alternate_keywords:external_link:description'));
  t('Boost Keywords', array(), array('context' => 'field_boost_keywords:external_link:label'));
  t('These keywords have a higher boost, so the content is shown higher in the search results.', array(), array('context' => 'field_boost_keywords:external_link:description'));
  t('External URL', array(), array('context' => 'field_external_url:external_link:label'));
  t('A link to an external website. For example, <em>http://domain.com</em>.', array(), array('context' => 'field_external_url:external_link:description'));
  t('Menu description', array(), array('context' => 'field_menu_description:external_link:label'));
  t('Menu image', array(), array('context' => 'field_menu_image:external_link:label'));
  t('Search result text', array(), array('context' => 'field_search_result:external_link:label'));
  t('The displayed text when a visitor sees this page as a given search result.', array(), array('context' => 'field_search_result:external_link:description'));
  return $field_instances;
}
