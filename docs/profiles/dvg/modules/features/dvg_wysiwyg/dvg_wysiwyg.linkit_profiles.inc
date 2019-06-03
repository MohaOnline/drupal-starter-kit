<?php
/**
 * @file
 * dvg_wysiwyg.linkit_profiles.inc
 */

/**
 * Implements hook_default_linkit_profiles().
 */
function dvg_wysiwyg_default_linkit_profiles() {
  $export = array();

  $linkit_profile = new LinkitProfile();
  $linkit_profile->disabled = FALSE; /* Edit this to true to make a default linkit_profile disabled initially */
  $linkit_profile->api_version = 1;
  $linkit_profile->name = 'fields_default';
  $linkit_profile->admin_title = 'Fields Default';
  $linkit_profile->admin_description = '';
  $linkit_profile->profile_type = '2';
  $linkit_profile->data = array(
    'search_plugins' => array(
      'entity:node' => array(
        'enabled' => 1,
        'weight' => '-10',
      ),
      'entity:taxonomy_term' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'entity:search_api_server' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'entity:file' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'entity:search_api_index' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'entity:user' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
    ),
    'entity:node' => array(
      'result_description' => '',
      'bundles' => array(
        'page' => 0,
        'block' => 0,
        'external_link' => 0,
        'general_text' => 0,
        'guide' => 0,
        'menu_page' => 0,
        'news' => 0,
        'plan' => 0,
        'profile' => 0,
        'profile_group' => 0,
        'task' => 0,
        'webform' => 0,
      ),
      'group_by_bundle' => 1,
      'include_unpublished' => 0,
    ),
    'entity:search_api_server' => array(
      'result_description' => '',
    ),
    'entity:search_api_index' => array(
      'result_description' => '',
    ),
    'entity:file' => array(
      'result_description' => '',
      'bundles' => array(
        'file_category_item' => 0,
        'dvg_document' => 0,
        'dvg_image' => 0,
        'pdf' => 0,
      ),
      'group_by_bundle' => 0,
      'show_scheme' => 0,
      'group_by_scheme' => 0,
      'url_type' => 'entity',
      'image_extra_info' => array(
        'thumbnail' => 'thumbnail',
        'dimensions' => 'dimensions',
      ),
    ),
    'entity:taxonomy_term' => array(
      'result_description' => '',
      'bundles' => array(
        'related_pages_title' => 0,
        'spatial_plan' => 0,
        'appointment_button_title' => 0,
        'owms_audience' => 0,
        'owms_authority' => 0,
        'owms_uniform_product_name' => 0,
        'webform_button_title' => 0,
        'file_category' => 0,
        'image_license' => 0,
        'town_council' => 0,
      ),
      'group_by_bundle' => 0,
    ),
    'entity:user' => array(
      'result_description' => '',
    ),
    'insert_plugin' => array(
      'plugin' => 'raw_url',
      'url_method' => '1',
    ),
    'attribute_plugins' => array(
      'accesskey' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'target' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'class' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'rel' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'id' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'title' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
    ),
    'autocomplete' => array(
      'charLimit' => '3',
      'wait' => '350',
      'remoteTimeout' => '10000',
    ),
  );
  $export['fields_default'] = $linkit_profile;

  $linkit_profile = new LinkitProfile();
  $linkit_profile->disabled = FALSE; /* Edit this to true to make a default linkit_profile disabled initially */
  $linkit_profile->api_version = 1;
  $linkit_profile->name = 'filtered_html_default';
  $linkit_profile->admin_title = 'Filtered HTML Default';
  $linkit_profile->admin_description = '';
  $linkit_profile->profile_type = '1';
  $linkit_profile->data = array(
    'text_formats' => array(
      'filtered_html' => 'filtered_html',
      'plain_text' => 0,
    ),
    'search_plugins' => array(
      'entity:node' => array(
        'enabled' => 1,
        'weight' => '-10',
      ),
      'entity:taxonomy_term' => array(
        'enabled' => 0,
        'weight' => '-9',
      ),
      'entity:search_api_server' => array(
        'enabled' => 0,
        'weight' => '-8',
      ),
      'entity:user' => array(
        'enabled' => 0,
        'weight' => '-7',
      ),
      'entity:search_api_index' => array(
        'enabled' => 0,
        'weight' => '-6',
      ),
      'entity:file' => array(
        'enabled' => 0,
        'weight' => '-5',
      ),
    ),
    'entity:node' => array(
      'result_description' => '',
      'bundles' => array(
        'general_text' => 0,
        'page' => 0,
        'guide' => 0,
        'menu_page' => 0,
        'news' => 0,
        'plan' => 0,
        'profile' => 0,
        'profile_group' => 0,
        'task' => 0,
        'webform' => 0,
      ),
      'group_by_bundle' => 1,
      'include_unpublished' => 0,
    ),
    'entity:search_api_server' => array(
      'result_description' => '',
      'include_unpublished' => 0,
    ),
    'entity:search_api_index' => array(
      'result_description' => '',
      'include_unpublished' => 0,
    ),
    'entity:file' => array(
      'result_description' => '',
      'bundles' => array(
        'image' => 0,
        'video' => 0,
        'audio' => 0,
        'document' => 0,
      ),
      'group_by_bundle' => 0,
      'show_scheme' => 0,
      'group_by_scheme' => 0,
      'image_extra_info' => array(
        'thumbnail' => 'thumbnail',
        'dimensions' => 'dimensions',
      ),
    ),
    'entity:taxonomy_term' => array(
      'result_description' => '',
      'bundles' => array(
        'related_pages_title' => 0,
        'appointment_button_title' => 0,
        'owms_audience' => 0,
        'owms_authority' => 0,
        'owms_uniform_product_name' => 0,
        'webform_button_title' => 0,
        'file_category' => 0,
        'image_license' => 0,
        'spatial_plan' => 0,
      ),
      'group_by_bundle' => 0,
    ),
    'entity:user' => array(
      'result_description' => '',
    ),
    'insert_plugin' => array(
      'url_method' => '2',
    ),
    'attribute_plugins' => array(
      'rel' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'accesskey' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'title' => array(
        'enabled' => 1,
        'weight' => '-10',
      ),
      'class' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'id' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'target' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
    ),
    'autocomplete' => array(
      'charLimit' => '3',
      'wait' => '350',
      'remoteTimeout' => '10000',
    ),
  );
  $export['filtered_html_default'] = $linkit_profile;

  $linkit_profile = new LinkitProfile();
  $linkit_profile->disabled = FALSE; /* Edit this to true to make a default linkit_profile disabled initially */
  $linkit_profile->api_version = 1;
  $linkit_profile->name = 'full_html_default';
  $linkit_profile->admin_title = 'Full HTML Default';
  $linkit_profile->admin_description = '';
  $linkit_profile->profile_type = '1';
  $linkit_profile->data = array(
    'text_formats' => array(
      'filtered_html' => 0,
      'plain_text' => 0,
    ),
    'search_plugins' => array(
      'entity:node' => array(
        'enabled' => 1,
        'weight' => '-10',
      ),
      'entity:taxonomy_term' => array(
        'enabled' => 0,
        'weight' => '-9',
      ),
      'entity:search_api_index' => array(
        'enabled' => 0,
        'weight' => '-8',
      ),
      'entity:search_api_server' => array(
        'enabled' => 0,
        'weight' => '-7',
      ),
      'entity:file' => array(
        'enabled' => 0,
        'weight' => '-6',
      ),
      'entity:user' => array(
        'enabled' => 0,
        'weight' => '-4',
      ),
    ),
    'entity:node' => array(
      'result_description' => '',
      'bundles' => array(
        'general_text' => 0,
        'page' => 0,
        'guide' => 0,
        'menu_page' => 0,
        'news' => 0,
        'plan' => 0,
        'profile' => 0,
        'profile_group' => 0,
        'task' => 0,
        'webform' => 0,
      ),
      'group_by_bundle' => 1,
      'include_unpublished' => 0,
    ),
    'entity:search_api_server' => array(
      'result_description' => '',
      'include_unpublished' => 0,
    ),
    'entity:search_api_index' => array(
      'result_description' => '',
      'include_unpublished' => 0,
    ),
    'entity:file' => array(
      'result_description' => '',
      'bundles' => array(
        'image' => 0,
        'video' => 0,
        'audio' => 0,
        'document' => 0,
      ),
      'group_by_bundle' => 0,
      'show_scheme' => 0,
      'group_by_scheme' => 0,
      'image_extra_info' => array(
        'thumbnail' => 'thumbnail',
        'dimensions' => 'dimensions',
      ),
    ),
    'entity:taxonomy_term' => array(
      'result_description' => '',
      'bundles' => array(
        'related_pages_title' => 0,
        'appointment_button_title' => 0,
        'owms_audience' => 0,
        'owms_authority' => 0,
        'owms_uniform_product_name' => 0,
        'webform_button_title' => 0,
        'file_category' => 0,
        'image_license' => 0,
        'spatial_plan' => 0,
      ),
      'group_by_bundle' => 0,
    ),
    'entity:user' => array(
      'result_description' => '',
    ),
    'insert_plugin' => array(
      'url_method' => '2',
    ),
    'attribute_plugins' => array(
      'rel' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'accesskey' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'title' => array(
        'enabled' => 1,
        'weight' => '-10',
      ),
      'class' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'id' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
      'target' => array(
        'enabled' => 0,
        'weight' => '-10',
      ),
    ),
    'autocomplete' => array(
      'charLimit' => '3',
      'wait' => '350',
      'remoteTimeout' => '10000',
    ),
  );
  $export['full_html_default'] = $linkit_profile;

  return $export;
}
