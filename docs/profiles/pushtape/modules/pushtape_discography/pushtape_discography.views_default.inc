<?php
/**
 * @file
 * pushtape_discography.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function pushtape_discography_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'albums';
  $view->description = 'A view of all published albums.';
  $view->tag = 'pushtape';
  $view->base_table = 'node';
  $view->human_name = 'Albums';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Albums';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No releases yet';
  $handler->display->display_options['empty']['area']['content'] = 'No albums yet. Add a <a href="?q=node/add/track">new track!</a>';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Content: Artwork */
  $handler->display->display_options['fields']['field_artwork']['id'] = 'field_artwork';
  $handler->display->display_options['fields']['field_artwork']['table'] = 'field_data_field_artwork';
  $handler->display->display_options['fields']['field_artwork']['field'] = 'field_artwork';
  $handler->display->display_options['fields']['field_artwork']['label'] = '';
  $handler->display->display_options['fields']['field_artwork']['element_class'] = '[type]';
  $handler->display->display_options['fields']['field_artwork']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_artwork']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_artwork']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_artwork']['settings'] = array(
    'image_style' => 'artwork_small',
    'image_link' => 'content',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'album' => 'album',
  );
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'albums';
  $handler->display->display_options['menu']['title'] = 'Albums';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Artwork */
  $handler->display->display_options['fields']['field_artwork']['id'] = 'field_artwork';
  $handler->display->display_options['fields']['field_artwork']['table'] = 'field_data_field_artwork';
  $handler->display->display_options['fields']['field_artwork']['field'] = 'field_artwork';
  $handler->display->display_options['fields']['field_artwork']['label'] = '';
  $handler->display->display_options['fields']['field_artwork']['element_class'] = '[type]';
  $handler->display->display_options['fields']['field_artwork']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_artwork']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_artwork']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_artwork']['settings'] = array(
    'image_style' => 'artwork_thumbnail',
    'image_link' => 'content',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $export['albums'] = $view;

  $view = new view();
  $view->name = 'releases';
  $view->description = 'This is a combination view of all albums and single tracks.';
  $view->tag = 'pushtape';
  $view->base_table = 'node';
  $view->human_name = 'Releases';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Releases';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No releases yet';
  $handler->display->display_options['empty']['area']['content'] = 'No releases yet. Add a <a href="?q=node/add/track">new track!</a>';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Content: Artwork */
  $handler->display->display_options['fields']['field_artwork']['id'] = 'field_artwork';
  $handler->display->display_options['fields']['field_artwork']['table'] = 'field_data_field_artwork';
  $handler->display->display_options['fields']['field_artwork']['field'] = 'field_artwork';
  $handler->display->display_options['fields']['field_artwork']['label'] = '';
  $handler->display->display_options['fields']['field_artwork']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_artwork']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_artwork']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_artwork']['settings'] = array(
    'image_style' => 'artwork_small',
    'image_link' => 'content',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'AND',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'album' => 'album',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'node';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['filters']['status_1']['group'] = 2;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'track' => 'track',
  );
  $handler->display->display_options['filters']['type_1']['group'] = 2;
  /* Filter criterion: Content: Album (field_album) */
  $handler->display->display_options['filters']['field_album_target_id']['id'] = 'field_album_target_id';
  $handler->display->display_options['filters']['field_album_target_id']['table'] = 'field_data_field_album';
  $handler->display->display_options['filters']['field_album_target_id']['field'] = 'field_album_target_id';
  $handler->display->display_options['filters']['field_album_target_id']['operator'] = 'empty';
  $handler->display->display_options['filters']['field_album_target_id']['group'] = 2;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'releases';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Releases';
  $handler->display->display_options['menu']['weight'] = '-1';
  $handler->display->display_options['menu']['name'] = 'main-menu';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Artwork */
  $handler->display->display_options['fields']['field_artwork']['id'] = 'field_artwork';
  $handler->display->display_options['fields']['field_artwork']['table'] = 'field_data_field_artwork';
  $handler->display->display_options['fields']['field_artwork']['field'] = 'field_artwork';
  $handler->display->display_options['fields']['field_artwork']['label'] = '';
  $handler->display->display_options['fields']['field_artwork']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_artwork']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_artwork']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_artwork']['settings'] = array(
    'image_style' => 'artwork_thumbnail',
    'image_link' => 'content',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $export['releases'] = $view;

  $view = new view();
  $view->name = 'tracklist';
  $view->description = 'An ordered tracklist view for an album node display.';
  $view->tag = 'pushtape';
  $view->base_table = 'node';
  $view->human_name = 'Tracklist';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['type'] = 'ol';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_audio' => 'field_audio',
    'title' => 'title',
    'field_artist' => 'field_artist',
  );
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_tracklist_node']['id'] = 'reverse_field_tracklist_node';
  $handler->display->display_options['relationships']['reverse_field_tracklist_node']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_tracklist_node']['field'] = 'reverse_field_tracklist_node';
  $handler->display->display_options['relationships']['reverse_field_tracklist_node']['required'] = TRUE;
  /* Field: Content: Audio */
  $handler->display->display_options['fields']['field_audio']['id'] = 'field_audio';
  $handler->display->display_options['fields']['field_audio']['table'] = 'field_data_field_audio';
  $handler->display->display_options['fields']['field_audio']['field'] = 'field_audio';
  $handler->display->display_options['fields']['field_audio']['label'] = '';
  $handler->display->display_options['fields']['field_audio']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_audio']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_audio']['type'] = 'pushtape_player';
  $handler->display->display_options['fields']['field_audio']['settings'] = array(
    'text' => 'empty',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Artist */
  $handler->display->display_options['fields']['field_artist']['id'] = 'field_artist';
  $handler->display->display_options['fields']['field_artist']['table'] = 'field_data_field_artist';
  $handler->display->display_options['fields']['field_artist']['field'] = 'field_artist';
  $handler->display->display_options['fields']['field_artist']['label'] = '';
  $handler->display->display_options['fields']['field_artist']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_artist']['type'] = 'taxonomy_term_reference_plain';
  /* Sort criterion: Content: Tracklist (field_tracklist:delta) */
  $handler->display->display_options['sorts']['delta']['id'] = 'delta';
  $handler->display->display_options['sorts']['delta']['table'] = 'field_data_field_tracklist';
  $handler->display->display_options['sorts']['delta']['field'] = 'delta';
  /* Contextual filter: Content: Album (field_album) */
  $handler->display->display_options['arguments']['field_album_target_id']['id'] = 'field_album_target_id';
  $handler->display->display_options['arguments']['field_album_target_id']['table'] = 'field_data_field_album';
  $handler->display->display_options['arguments']['field_album_target_id']['field'] = 'field_album_target_id';
  $handler->display->display_options['arguments']['field_album_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_album_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_album_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_album_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_album_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $export['tracklist'] = $view;

  $view = new view();
  $view->name = 'tracks';
  $view->description = 'A view of all published tracks.';
  $view->tag = 'pushtape';
  $view->base_table = 'node';
  $view->human_name = 'Tracks';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Tracks';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No releases yet';
  $handler->display->display_options['empty']['area']['content'] = 'No releases yet. Add a <a href="?q=node/add/track">new track!</a>';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Content: Artwork */
  $handler->display->display_options['fields']['field_artwork']['id'] = 'field_artwork';
  $handler->display->display_options['fields']['field_artwork']['table'] = 'field_data_field_artwork';
  $handler->display->display_options['fields']['field_artwork']['field'] = 'field_artwork';
  $handler->display->display_options['fields']['field_artwork']['label'] = '';
  $handler->display->display_options['fields']['field_artwork']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_artwork']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_artwork']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_artwork']['settings'] = array(
    'image_style' => 'artwork_small',
    'image_link' => 'content',
  );
  /* Field: Content: Album */
  $handler->display->display_options['fields']['field_album']['id'] = 'field_album';
  $handler->display->display_options['fields']['field_album']['table'] = 'field_data_field_album';
  $handler->display->display_options['fields']['field_album']['field'] = 'field_album';
  $handler->display->display_options['fields']['field_album']['label'] = '';
  $handler->display->display_options['fields']['field_album']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_album']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_album']['hide_alter_empty'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'track' => 'track',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'tracks';
  $handler->display->display_options['menu']['title'] = 'Tracks';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Artwork */
  $handler->display->display_options['fields']['field_artwork']['id'] = 'field_artwork';
  $handler->display->display_options['fields']['field_artwork']['table'] = 'field_data_field_artwork';
  $handler->display->display_options['fields']['field_artwork']['field'] = 'field_artwork';
  $handler->display->display_options['fields']['field_artwork']['label'] = '';
  $handler->display->display_options['fields']['field_artwork']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_artwork']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_artwork']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_artwork']['settings'] = array(
    'image_style' => 'artwork_thumbnail',
    'image_link' => 'content',
  );
  /* Field: Content: Album */
  $handler->display->display_options['fields']['field_album']['id'] = 'field_album';
  $handler->display->display_options['fields']['field_album']['table'] = 'field_data_field_album';
  $handler->display->display_options['fields']['field_album']['field'] = 'field_album';
  $handler->display->display_options['fields']['field_album']['label'] = '';
  $handler->display->display_options['fields']['field_album']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_album']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_album']['hide_alter_empty'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'track' => 'track',
  );
  $export['tracks'] = $view;

  return $export;
}
