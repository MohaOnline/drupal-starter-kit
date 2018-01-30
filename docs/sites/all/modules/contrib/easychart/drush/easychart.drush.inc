<?php

/**
 * Implements hook_drush_command().
 */

function easychart_drush_command() {
  $items = array();

  $items['easychart-dependencies'] = array(
    'aliases' => array('ec-dependencies'),
    'callback' => 'easychart_drush_dependencies',
    'description' => dt('Download and install the javascript dependencies for the Easychart module.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Easychart dependencies. If omitted Drush will use the default location (sites/all/libraries).'),
    ),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function easychart_drush_help($section) {
  switch ($section) {
    case 'drush:easychart-plugin':
      return dt('Download and install the Highcharts javascript library and the Easychart plugin from github.com/bestuurszaken/easychart, default location is sites/all/libraries.');
  }
}

function easychart_drush_dependencies() {
  $args = func_get_args();
  $path = (!empty($args[0]) ? $args[0] : 'sites/all/libraries');

  // version check
  $highcharts = _get_lib_info('highcharts');
  $easychart = _get_lib_info('easychart');

  // regex
  $re = "/[^0-9]/";
  $subst = "";

  // Installed release.
  $inst_hc = (int)preg_replace($re, $subst, $highcharts['current_lib_info']['version']);

  // Recommended release.
  $recom_hc = (int)preg_replace($re, $subst, $highcharts['current_lib_info']['recommended']['version']);

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();

  // Easychart plugin.

  chdir($path);

  if ($filepath = drush_download_file($easychart['current_lib_info']['latest']['uri'])) {
    $filename = basename($filepath);
    $dirname = 'easychart';

    // Remove any existing Easychart plugin directory.
    if (is_dir($dirname)) {
      drush_delete_dir($dirname, TRUE);
      drush_log(dt('An existing Easychart plugin was deleted from @path.', array('@path' => $path)), 'notice');
    }

    drush_op('mkdir', $dirname);
    drush_tarball_extract($filename, $dirname);
    chdir($dirname);

    if (drush_shell_exec('mv $(ls -t | head -n1) ___temp')) {
      if (drush_shell_exec('mv ___temp/* .')) {
        if (drush_shell_exec('rm -rf ___temp/')) {
          if (drush_shell_exec('rm ../' . $filename)) {
            drush_log(dt('Latest Easychart plugin version has been installed in @path.', array('@path' => $path)), 'success');
          }
          else {
            drush_log(dt('Drush was unable to install the Easychart plugin to @path.', array('@path' => $path)), 'error');
          }
        }
      }
    }
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);

  // Highcharts plugin.
  if ($inst_hc == $recom_hc) {
    drush_log(dt('Highcharts javascript library is up to date (@curr).', array('@curr' => $highcharts['current_lib_info']['version'])), 'ok');
  }
  else if ($inst_hc < $recom_hc) {
    chdir($path);

    if (!$highcharts['current_lib_info']['installed']) {
      $msg = dt('The required Highcharts javascript library version could not be found. Download and install version @recom now?', array('@recom' => $highcharts['current_lib_info']['recommended']['version'], '@current' => $highcharts['current_lib_info']['version']));
    }
    else {
      $msg = dt('Highcharts javascript library update version @recom available. Currently installed: v@current. Update now to @recom?', array('@recom' => $highcharts['current_lib_info']['recommended']['version'], '@current' => $highcharts['current_lib_info']['version']));
    }

    if (drush_confirm($msg)) {
      //$uri = 'http://code.highcharts.com/zips/Highcharts-' . substr($highcharts['available'][0]->name, 1) . '.zip';

      if ($filepath = drush_download_file($highcharts['current_lib_info']['recommended']['uri'])) {
        $filename = basename($filepath);
        $dirname = 'highcharts';

        // Remove any existing Highcharts plugin directory.
        if (is_dir($dirname)) {
          drush_delete_dir($dirname, TRUE);
          drush_log(dt('An existing Highcharts plugin was deleted from @path.', array('@path' => $path)), 'notice');
        }

        drush_op('mkdir', $dirname);
        drush_tarball_extract($filename, $dirname);
      }

      if (is_dir($dirname)) {
        drush_log(dt('Highcharts plugin version @recom has been installed in @path.', array('@recom' => $highcharts['current_lib_info']['recommended']['version'], '@path' => $path)), 'success');
      }
      else {
        drush_log(dt('Drush was unable to install the Highcharts plugin to @path.', array('@path' => $path)), 'error');
      }
    }
    else {
      drush_log(dt('Please update asap to Highcharts version @recom.', array('@recom' => $highcharts['current_lib_info']['recommended']['version'])), 'notice');
    }
  }
  // Set working directory back to the previous working directory.
  chdir($olddir);

}

/*
 * Get the information for each required library.
 */
function _get_lib_info($lib) {
  $output = array();
  $output['current_lib_info'] = libraries_detect($lib);
  return $output;
}
