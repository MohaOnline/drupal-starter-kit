<?php

/**
 * @file
 * Module file.
 */

/**
 * Callback menu.
 */
function drupal_dashboard_system() {
  drupal_add_js(drupal_get_path('module', 'drupal_dashboard') . '/drupal_dashboard.js');
  // Node.
  $node_header[] = array('data' => t('Content Type (Count of publish and unpublish nodes)'), 'colspan' => 3);
  $content_types = node_type_get_types();
  $node_count = array();
  $node_count['header'][] = t('<b>Type</b>');
  $node_count['header'][] = t('<b>Published Node Count</b>');
  $node_count['header'][] = t('<b>Unpublished Node Count</b>');
  $counter = 1;
  foreach ($content_types as $key => $value) {
    $node_published = _drupal_dashboard_module_node_status($key, 1);
    $node_unpublished = _drupal_dashboard_module_node_status($key, 0);
    $node_count[$counter][] = $value->name;
    $node_count[$counter][] = $node_published->count;
    $node_count[$counter][] = $node_unpublished->count;
    $counter++;
  }

  $node_count[$counter][] = '';
  $node_count[$counter][] = t('More  =>') . l(t('Click Here'), 'admin/content');
  $node_count[$counter][] = '';
  $output = '<div id="node_count" onclick=drupal_dashboard_click_toggle("node_count")>'
            . theme('table', array(
              'header' => $node_header,
              'rows' => $node_count,
              'empty' => t('No categories available. <a href="@link">Add category</a>.',
                      array('@link' => url('admin/content'))),
              )
                    ) . '</div>';

  // Blocks.
  $blocks = array();
  $blocks['block'][] = t('<b>Content Type</b>');
  $blocks['block'][] = t('<b>Blocks Count</b>');
  $blocks = array_merge($blocks, _drupal_dashboard_blockinfo($content_types));
  $blocks['more'][] = '';
  $blocks['more'][] = t('More  =>') . l(t('Click Here'), 'admin/structure/block');
  $block_header[] = array('data' => t('Blocks (No of blocks per content types)'), 'colspan' => 2);
  $output .= '<div id="block_count" onclick=drupal_dashboard_click_toggle("block_count")>'
          . theme('table', array(
            'header' => $block_header,
            'rows' => $blocks,
            'empty' => t('No categories available. <a href="@link">Add category</a>.',
                    array('@link' => url('admin/structure/block'))),
            )
                  ) . '</div>';

  // Modules.
  $module_header[] = array('data' => t('Module (Count of enable module)'), 'colspan' => 2);
  $module_data['header'][] = t('<b>Module</b>');
  $module_data['header'][] = t('<b>Count</b>');
  $module_data[1][] = t('Module Enable');
  $module_data[1][] = _drupal_dashboard_module_theme_status('module', 1)->count;
  $module_data[2][] = '';
  $module_data[2][] = t('More  =>') . l(t('Click Here'), 'admin/modules');
  $output .= '<div id="module_count" onclick=drupal_dashboard_click_toggle("module_count")>'
          . theme('table', array(
            'header' => $module_header,
            'rows' => $module_data,
            'empty' => t('No categories available. <a href="@link">Add category</a>.',
                    array('@link' => url('admin/modules'))),
            )
                  ) . '</div>';

  // Redirect.
  if (module_exists('redirect')) {
    $redirect_count = _drupal_dashboard_loop_redirect()->count;
    $redirect_header[] = array('data' => t('Redirect (Total redirects and loop redirects)'), 'colspan' => 2);
    $redirect_array = array();
    $redirect_array[0][] = t('No. of Redirects');
    $redirect_array[0][] = _drupal_dashboard_count_redirect()->count;
    $redirect_array[1][] = t('NO. of Loop Redirects');
    $redirect_array[1][] = (!empty($redirect_count) ? $redirect_count : 0);
    $redirect_array[2][] = '';
    $redirect_array[2][] = t('More  =>') . l(t('Click Here'), 'admin/config/search/redirect/settings');
    $output .= '<div id="redirect_count" onclick=drupal_dashboard_click_toggle("redirect_count")>'
            . theme('table', array(
              'header' => $redirect_header,
              'rows' => $redirect_array,
              'empty' => t('No categories available. <a href="@link">Add category</a>.',
                      array('@link' => url('admin/config/search/redirect/settings'))),
              )
                    ) . '</div>';
  }

  // Theme.
  $theme_header[] = array('data' => t('Theme (No of enable/disable themes)'), 'colspan' => 2);
  $theme_data['header'][] = t('<b>Theme</b>');
  $theme_data['header'][] = t('<b>Count</b>');
  $theme_data[1][] = t('Theme Enable');
  $theme_data[1][] = _drupal_dashboard_module_theme_status('theme', 1)->count;
  $theme_data[2][] = t('Theme Disable');
  $theme_data[2][] = _drupal_dashboard_module_theme_status('theme', 0)->count;
  $theme_data[3][] = '';
  $theme_data[3][] = t('More  =>') . l(t('Click Here'), 'admin/appearance');
  $output .= '<div id="theme_count"  onclick=drupal_dashboard_click_toggle("theme_count")>'
          . theme('table', array(
            'header' => $theme_header,
            'rows' => $theme_data,
            'empty' => t('No categories available. <a href="@link">Add category</a>.',
                    array('@link' => url('admin/appearance'))),
            )
                  ) . '</div>';
  // Taxonomy.
  $vocab_header[] = array('data' => t('Taxonomy (Taxonomy and term counts)'), 'colspan' => 4);
  $vocab = taxonomy_get_vocabularies();
  $term_data = array();
  $term_data['header'][] = t('<b>Vocab</b>');
  $term_data['header'][] = t('<b>Vid</b>');
  $term_data['header'][] = t('<b>Term Count</b>');
  $term_data['header'][] = t('<b>List Link</b>');
  if (!empty($vocab)) {
    foreach ($vocab as $key => $value) {
      // Pass the vocabulary ID.
      $terms_array = taxonomy_get_tree($value->vid);
      $terms_count = count($terms_array);
      $term_data[$key][] = $value->name . '<br />' . $value->description;
      $term_data[$key][] = $value->vid;
      $term_data[$key][] = $terms_count;
      $term_data[$key][] = l(t('Check'), 'admin/structure/taxonomy/' . $value->machine_name);
    }
  }

  $output .= '<div id="taxo_count" onclick=drupal_dashboard_click_toggle("taxo_count")>'
          . theme('table', array(
            'header' => $vocab_header,
            'rows' => $term_data,
            'empty' => t('No categories available. <a href="@link">Add category</a>.',
                    array('@link' => url('admin/structure/taxonomy'))),
            )
                  ) . '</div>';


  // Url Aliases.
  if (module_exists('pathauto')) {
    $alias_header[] = array('data' => t('Url Alias (Total no of url aliases)'), 'colspan' => 2);
    $alias_array = array();
    $alias_array[0][] = t('Total Url Alias');
    $alias_array[0][] = _drupal_dashboard_pathauto()->count;
    $alias_array[1][] = t('Node Url Alias');
    $alias_array[1][] = _drupal_dashboard_pathauto('node')->count;
    $alias_array[2][] = t('Term Url Alias');
    $alias_array[2][] = _drupal_dashboard_pathauto('taxonomy')->count;
    $alias_array[3][] = t('User Url Alias');
    $alias_array[3][] = _drupal_dashboard_pathauto('user')->count;
    $alias_array[4][] = '';
    $alias_array[4][] = t('More  =>') . l(t('Click Here'), 'admin/config/search/path');
    $output .= '<div id="alias_count" onclick=drupal_dashboard_click_toggle("alias_count")>'
            . theme('table', array(
              'header' => $alias_header,
              'rows' => $alias_array,
              'empty' => t('No categories available. <a href="@link">Add category</a>.',
                      array('@link' => url('admin/config/search/path'))),
              )
                    ) . '</div>';
  }

  // User.
  $roles = user_roles();
  $user_roles = array();
  $user_roles['user'][] = t('<b>Roles</b>');
  $user_roles['user'][] = t('<b>Count</b>');
  foreach ($roles as $rid => $name) {
    $result = db_query("SELECT COUNT(u.uid) as count FROM {users} u INNER JOIN {users_roles} ur ON
       u.uid=ur.uid WHERE ur.rid = :rid AND u.status = 1", array(':rid' => $rid))->fetch();
    $user_roles[$rid][] = $name;
    $user_roles[$rid][] = $result->count;
  }
  $result = db_query("SELECT COUNT(u.uid) as count FROM {users} u WHERE u.status = 0")->fetch();
  $user_roles['inactive'][] = 'Inactive Users';
  $user_roles['inactive'][] = $result->count;

  $user_roles['more'][] = '';
  $user_roles['more'][] = t('More  =>') . l(t('Click Here'), 'admin/people');
  $user_header[] = array('data' => t('Users (Total no of users as per role)'), 'colspan' => 2);
  $output .= '<div id="user_count" onclick=drupal_dashboard_click_toggle("user_count")>'
          . theme('table', array(
            'header' => $user_header,
            'rows' => $user_roles,
            'empty' => t('No categories available. <a href="@link">Add category</a>.',
                    array('@link' => url('admin/people/permissions/roles'))),
            )
                  ) . '</div>';

  return $output;
}

/**
 * Helper function to get modules and theme count.
 */
function _drupal_dashboard_module_theme_status($type, $status) {
  $result = db_query('SELECT COUNT(s.name) AS count FROM {system} s WHERE s.type = :type AND s.status = :status', array(':type' => $type, ':status' => $status))->fetch();
  return $result;
}

/**
 * Helper function for node counts.
 */
function _drupal_dashboard_module_node_status($type, $status) {
  $result = db_query('SELECT COUNT(n.nid) AS count FROM {node} n WHERE n.type = :type AND status = :status', array(':type' => $type, ':status' => $status))->fetch();
  return $result;
}

/**
 * Helper function to get loop redirects if any.
 */
function _drupal_dashboard_loop_redirect() {
  $result = db_query('SELECT r.rid, r.language, r.source, r.redirect FROM {redirect} r INNER JOIN {url_alias} u ON r.source = u.alias AND r.redirect = u.source AND r.language = u.language')->fetch();
  return $result;
}

/**
 * Helper function to get redirect counts.
 */
function _drupal_dashboard_count_redirect() {
  $result = db_query('SELECT count(*) AS count FROM {redirect}')->fetch();
  return $result;
}

/**
 * Helper function to get url alias counts.
 */
function _drupal_dashboard_pathauto($source = NULL) {
  if ($source != NULL) {
    $result = db_query('SELECT count(*) AS count FROM {url_alias} WHERE source LIKE :source', array(':source' => $source . "%"))->fetch();
  }
  else {
    $result = db_query('SELECT count(*) AS count FROM {url_alias}')->fetch();
  }
  return $result;
}

/**
 * Helper function to get block info.
 */
function _drupal_dashboard_blockinfo($content_types) {
  $blocks = array();
  $result = db_query("select type,count(*) AS count from block_node_type group by type;")->fetchAll();
  foreach ($result as $key => $value) {
    $blocks[$key][] = $content_types[$value->type]->name;
    $blocks[$key][] = $value->count;
  }
  return $blocks;
}
