<?php
/**
 * @file
 * wetkit_wetboew.libraries.inc
 */

/**
 * Implements hook_library().
 */
function wetkit_wetboew_library() {

  $path = drupal_get_path('module', 'wetkit_wetboew');

  // Ensure that each 3rd party library dependency has a default variant.
  // Convert Libraries module data structures to library data structures.
  $wxt_libraries = array(
    'wet-boew' => 'WxT Core',
    'theme-wet-boew' => 'WxT Usability Theme',
    'theme-base' => 'WxT Base Theme',
    'theme-gc-intranet' => 'WxT Intranet Theme for Government of Canada',
    'theme-gcweb' => 'WxT Canada Theme for Government of Canada',
    'theme-gcweb-v5' => 'WxT Canada Theme (v5) for Government of Canada',
    'theme-gcwu-fegc' => 'WxT Internet Theme for Government of Canada',
    'theme-ogpl' => 'WxT Open Government Platform Theme',
  );

  foreach ($wxt_libraries as $wxt_library => $name) {
    $libraries[$wxt_library] = _wetkit_wetboew_convert_libraries_to_library(libraries_detect($wxt_library), array());
  }

  return $libraries;
}

/**
 * Implements hook_libraries_info().
 */
function wetkit_wetboew_libraries_info() {
  $libraries = array();

  if ((variable_get('wetkit_wetboew_minified', 'minified') == 'unminified')) {
    $variant_order = array('source', 'minified');
  }
  else {
    $variant_order = array('minified', 'source');
  }

  $common = array(
    'version callback' => '_wetkit_wetboew_libraries_get_version',
    'variant order' => $variant_order,
  );

  $wxt_libraries = array(
    'wet-boew' => 'WxT Core',
    'theme-wet-boew' => 'WxT Usability Theme',
    'theme-base' => 'WxT Base Theme',
    'theme-gc-intranet' => 'WxT Intranet Theme for Government of Canada',
    'theme-gcweb' => 'WxT Canada Theme for Government of Canada',
    'theme-gcweb-v5' => 'WxT Canada Theme (v5) for Government of Canada',
    'theme-gcwu-fegc' => 'WxT Internet Theme for Government of Canada',
    'theme-ogpl' => 'WxT Open Government Platform Theme',
  );

  foreach ($wxt_libraries as $wxt_library => $name) {
    // WxT Core
    if ($wxt_library == 'wet-boew') {
      $libraries[$wxt_library] = array(
        'name' => 'Web Experience Toolkit Core',
        'vendor url' => 'https://github.com/wet-boew/wet-boew',
        'download url' => 'https://github.com/wet-boew/wet-boew/downloads',
        'version arguments' => array(
          'variants' => array(
            'source' => array(
              'file' => 'js/wet-boew.js',
              'pattern' => '#\s+[Vv]?([0-9\.]+)#',
              'lines' => 5,
            ),
            'minified' => array(
              'file' => 'js/wet-boew.min.js',
              'pattern' => '#\s+[Vv]?([0-9\.]+)#',
              'lines' => 5,
            ),
          ),
        ),
        'versions' => array(
          // Means ">=4.0.2": matches 4.0.2, 4.0.3, etc.
          '4.0.2' => array(
            'variants' => array(
              'source' => array(
                'files' => array(
                  'js' => array(
                    'js/wet-boew.js' => array(
                      'browsers' => array(
                        'IE' => 'gte IE 9',
                        '!IE' => TRUE,
                      ),
                      'every_page' => TRUE,
                      'preprocess' => FALSE,
                      'scope' => 'footer',
                      'weight' => 5,
                    ),
                    'js/ie8-wet-boew.js' => array(
                      'browsers' => array(
                        'IE' => 'lt IE 9',
                        '!IE' => FALSE,
                      ),
                      'every_page' => TRUE,
                      'preprocess' => FALSE,
                      'scope' => 'header',
                      'weight' => 6,
                    ),
                    'js/ie8-wet-boew2.js' => array(
                      'browsers' => array(
                        'IE' => 'lt IE 9',
                        '!IE' => FALSE,
                      ),
                      'every_page' => TRUE,
                      'preprocess' => TRUE,
                      'scope' => 'footer',
                      'weight' => 6,
                    ),
                  ),
                ),
                // Without a variant callback, the variant is assumed to be
                // installed.
                'variant callback' => '_wetkit_wetboew_libraries_variant_exists',
                'variant arguments' => array('js/wet-boew.js')
              ),
              'minified' => array(
                'files' => array(
                  'js' => array(
                    'js/wet-boew.min.js' => array(
                      'browsers' => array(
                        'IE' => 'gte IE 9',
                        '!IE' => TRUE,
                      ),
                      'every_page' => TRUE,
                      'preprocess' => FALSE,
                      'scope' => 'footer',
                      'weight' => 5,
                    ),
                    'js/ie8-wet-boew.min.js' => array(
                      'browsers' => array(
                        'IE' => 'lt IE 9',
                        '!IE' => FALSE,
                      ),
                      'every_page' => TRUE,
                      'preprocess' => FALSE,
                      'scope' => 'header',
                      'weight' => 6,
                    ),
                    'js/ie8-wet-boew2.min.js' => array(
                      'browsers' => array(
                        'IE' => 'lt IE 9',
                        '!IE' => FALSE,
                      ),
                      'every_page' => TRUE,
                      'preprocess' => TRUE,
                      'scope' => 'footer',
                      'weight' => 6,
                    ),
                  ),
                ),
                // Without a variant callback, the variant is assumed to be
                // installed.
                'variant callback' => '_wetkit_wetboew_libraries_variant_exists',
                'variant arguments' => array('js/wet-boew.min.js')
              ),
            ),
          ),
        ),
      );
    }
    else {
      $libraries[$wxt_library] = array(
        'name' => 'Web Experience Toolkit Theme',
        'vendor url' => 'https://github.com/wet-boew/wet-boew',
        'download url' => 'https://github.com/wet-boew/wet-boew/downloads',
        'version arguments' => array(
          'variants' => array(
            'source' => array(
              'file' => 'css/wet-boew.css',
              'pattern' => '#\s+[Vv]?([0-9\.]+)#',
              'lines' => 6,
            ),
            'minified' => array(
              'file' => 'css/wet-boew.min.css',
              'pattern' => '#\s+[Vv]?([0-9\.]+)#',
              'lines' => 6,
            ),
          ),
        ),
        'versions' => array(
          // Means ">=4.0.2": matches 4.0.2, 4.0.3, etc.
          '4.0.2' => array(
            'variants' => array(
              'source' => array(
                'files' => array(
                  'css' => array(
                    'css/theme.css' => array(
                      'browsers' => array(
                        'IE' => 'gte IE 9',
                        '!IE' => TRUE,
                      ),
                      'group' => CSS_DEFAULT,
                      'every_page' => TRUE,
                      'preprocess' => TRUE,
                      'weight' => -50,
                    ),
                    'css/ie8-theme.css' => array(
                      'browsers' => array(
                        'IE' => 'lt IE 9',
                        '!IE' => FALSE,
                      ),
                      'group' => CSS_DEFAULT,
                      'every_page' => TRUE,
                      'preprocess' => FALSE,
                      'weight' => -50,
                    ),
                  ),
                  'js' => array(
                    'js/theme.js' => array(
                      'browsers' => array(
                        'IE' => 'gte IE 9',
                        '!IE' => TRUE,
                      ),
                      'every_page' => TRUE,
                      'preprocess' => FALSE,
                      'scope' => 'footer',
                      'weight' => 5,
                    ),
                  ),
                ),
                // Without a variant callback, the variant is assumed to be
                // installed.
                'variant callback' => '_wetkit_wetboew_libraries_variant_exists',
                'variant arguments' => array('css/wet-boew.css')
              ),
              'minified' => array(
                'files' => array(
                  'css' => array(
                    'css/theme.min.css' => array(
                      'browsers' => array(
                        'IE' => 'gte IE 9',
                        '!IE' => TRUE,
                      ),
                      'group' => CSS_DEFAULT,
                      'every_page' => TRUE,
                      'preprocess' => TRUE,
                      'weight' => -50,
                    ),
                    'css/ie8-theme.min.css' => array(
                      'browsers' => array(
                        'IE' => 'lt IE 9',
                        '!IE' => FALSE,
                      ),
                      'group' => CSS_DEFAULT,
                      'every_page' => TRUE,
                      'preprocess' => FALSE,
                      'weight' => -50,
                    ),
                  ),
                  'js' => array(
                    'js/theme.min.js' => array(
                      'every_page' => TRUE,
                      'preprocess' => FALSE,
                      'scope' => 'footer',
                      'weight' => 10,
                    ),
                  ),
                ),
                // Without a variant callback, the variant is assumed to be
                // installed.
                'variant callback' => '_wetkit_wetboew_libraries_variant_exists',
                'variant arguments' => array('css/wet-boew.min.css')
              ),
            ),
          ),
        ),
      );
    }

    // WxT specific theme markup.
    if ($wxt_library == 'theme-wet-boew') {
      foreach ($variant_order as $variant) {
        // theme-wet-boew has js residing in wet-boew
        unset($libraries[$wxt_library]['versions']['4.0.2']['variants'][$variant]['files']['js']);
      }
    }

    // GC Internet specific theme markup.
    if ($wxt_library == 'theme-gcwu-fegc') {
      foreach ($variant_order as $variant) {
        if (strpos(current_path(),'splash') !== false) {
          unset($libraries[$wxt_library]['versions']['4.0.2']['variants'][$variant]['files']['css']['css/theme.min.css']);
          $css = ($variant == 'source') ? 'css/theme-sp-pe.css' : 'css/theme-sp-pe.min.css';
          $libraries[$wxt_library]['versions']['4.0.2']['variants'][$variant]['files']['css'][$css] = array(
            'group' => CSS_DEFAULT,
            'every_page' => TRUE,
            'preprocess' => TRUE,
            'weight' => 5,
          );
        }
      }
    }

    // GCWEB specific theme markup.
    if ($wxt_library == 'theme-gcweb' || $wxt_library == 'theme-gcweb-v5') {
      foreach ($variant_order as $variant) {
        $css = ($variant == 'source') ? 'css/messages.css' : 'css/messages.min.css';
        $libraries[$wxt_library]['versions']['4.0.2']['variants'][$variant]['files']['css'][$css] = array(
          'group' => CSS_DEFAULT,
          'every_page' => TRUE,
          'preprocess' => TRUE,
          'weight' => 5,
        );
        $ie8_css = ($variant == 'source') ? 'css/messages-ie.min.css' : 'css/messages-ie.min.css';
        $libraries[$wxt_library]['versions']['4.0.2']['variants'][$variant]['files']['css'][$ie8_css] = array(
          'browsers' => array(
            'IE' => 'lt IE 9',
            '!IE' => FALSE,
          ),
          'group' => CSS_DEFAULT,
          'every_page' => TRUE,
          'preprocess' => FALSE,
          'weight' => 5,
        );
      }
    }

    $libraries[$wxt_library] += $common;
  }

  return $libraries;
}

/**
 * Libraries API variant callback.
 */
function _wetkit_wetboew_libraries_variant_exists($library, $variant_name, $required_file) {
  return file_exists($library['library path'] . '/' . $required_file);
}

/**
 * Determines the version of a wet-boew library.
 *
 * This is used in case different variants of the library are shipped separately
 * and, thus, different variants can contain different versions.
 *
 * @param array $library
 *   An associative array containing all information about the library. The
 *   library is assumed to have the following non-standard keys:
 *   - variant order: An array of variant names, ordered from the most preferred
 *     variant to the least preferred.
 * @param array $options
 *   An associative array with the following keys:
 *   - variants: An array of options for libraries_get_version() keyed by
 *     variant name.
 */
function _wetkit_wetboew_libraries_get_version(&$library, $options = array()) {
  $versions = array();
  foreach ($library['variant order'] as $variant_name) {
    $variant = $library['version arguments']['variants'][$variant_name];
    // Use the libraries get version function to determine the version string.
    $versions[$variant_name] = libraries_get_version($library, $variant);
  }

  // If no versions could be found for any of the variant, there is no version
  // to return. If different versions have been found, there is no way to
  // determine the correct one. We cannot use the information on the preferred
  // variants because we cannot guarantee that a less preferred variant will not
  // be loaded. Null values are fine. Either that variant file doesn't exist
  // or id doesn't contain version information. As long as the there is no
  // conflicting version information, the check should pass.
  $versions = array_filter($versions, '_wetkit_wetboew_libraries_filter_null_values');
  $version = array_unique($versions);
  $vcount = count($version);
  if ($vcount == 1) {
    // A version number exists, so suppress any errors that any individual
    // variant might have raised.
    unset($library['error']);
    unset($library['error message']);
    return array_shift($version);
  }
  elseif ($vcount > 1) {
    $output = array();
    foreach ($versions as $name => $v) {
      $output[] = t('@name (@v)', array(
        '@name' => $name,
        '@v' => $v,
      ));
    }

    $library['error'] = 'inconsistent versions';
    $library['error message'] = t('The library\'s variants returned inconsistent versions: @variant_info', array(
      '@variant_info' => implode(', ', $output),
    ));
  }
  // If the version count is zero, then let the error from libraries_get_version
  // propagate through.
}

/**
 * Returns the variant that should be loaded based on order preference.
 *
 * @param array $library
 *   A libraries module library definition array.
 * @return string
 *   The name of the variant that should be loaded.
 */
function _wetkit_wetboew_libraries_get_preferred_variant_name($library) {
  if (!empty($library['variant order'])) {
    foreach ($library['variant order'] as $name) {
      if ($variant = $library['variants'][$name]) {
        if ($variant['installed']) {
          return $name;
        }
      }
    }
  }
  return NULL;
}

/**
 * Determines if an item is empty or not.
 *
 * @param string $item
 *   A version number string.
 * @return boolean
 *   Whether the $item's value is empty or not.
 */
function _wetkit_wetboew_libraries_filter_null_values($item) {
  return !empty($item);
}

/**
 * Converts a libraries module array to a hook_library array.
 *
 * @todo Libraries API should automatically register all libraries in
 *   hook_library(). See https://drupal.org/node/1386368
 *
 * @return Array
 *  Returns a standard Drupal library definition structure.
 */
function _wetkit_wetboew_convert_libraries_to_library($library, $options = array()) {

  // If the library wasn't installed, don't bother converting it.
  if (!$library['installed']) {
    return array();
  }

  $converted = array();
  $files = array();

  // Get the library files from one of the installed variants.
  if ($name = _wetkit_wetboew_libraries_get_preferred_variant_name($library)) {
    $files = $library['variants'][$name]['files'];
  }

  // Define the library if files exist for it.
  if (!empty($files)) {
    // This is the basic structure expected by hook_library().
    $converted = array(
      'title' => $library['name'],
      'website' => $library['vendor url'],
      'version' => $library['version'],
    );
    foreach ($files as $type => $paths) {
      foreach($paths as $filename => $data) {
        $converted[$type][$library['library path'] . '/' . $filename] = $data;
      }
    }
  }

  return $converted;
}
