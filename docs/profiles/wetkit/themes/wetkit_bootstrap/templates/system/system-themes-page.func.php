<?php
/**
 * @file
 * system-themes-page.func.php
 */

/**
 * Overrides HTML for the Appearance page.
 *
 * @param $variables
 *   An associative array containing:
 *   - theme_groups: An associative array containing groups of themes.
 *
 * @ingroup themeable
 */
function wetkit_bootstrap_system_themes_page($variables) {
  $theme_groups = $variables['theme_groups'];

  $output = '<div id="system-themes-page">';

  foreach ($variables['theme_group_titles'] as $state => $title) {
    if (!count($theme_groups[$state])) {
      // Skip this group of themes if no theme is there.
      continue;
    }
    // Start new theme group.
    $output .= '<div class="system-themes-list system-themes-list-'. $state .' clearfix"><h2>'. $title .'</h2>';

    foreach ($theme_groups[$state] as $theme) {

      // Theme the screenshot.
      $screenshot = $theme->screenshot ? theme('image', $theme->screenshot) : '<div class="no-screenshot">' . t('no screenshot') . '</div>';

      // Localize the theme description.
      $description = t($theme->info['description']);

      // Style theme info
      $notes = count($theme->notes) ? ' (' . join(', ', $theme->notes) . ')' : '';
      $theme->classes[] = 'theme-selector';
      $theme->classes[] = 'clearfix';
      $output .= '<div style="margin-bottom: 20px; padding-bottom: 20px; border-bottom: 1px solid #ccc;" class="row '. join(' ', $theme->classes) .'">' . '<div class="col-md-3">' . $screenshot . '</div><div class="theme-info col-md-9"><h3>' . $theme->info['name'] . ' ' . (isset($theme->info['version']) ? $theme->info['version'] : '') . $notes . '</h3><div class="theme-description">' . $description . '</div>';

      // Make sure to provide feedback on compatibility.
      if (!empty($theme->incompatible_core)) {
        $output .= '<div class="incompatible">' . t('This version is not compatible with Drupal !core_version and should be replaced.', array('!core_version' => DRUPAL_CORE_COMPATIBILITY)) . '</div>';
      }
      elseif (!empty($theme->incompatible_php)) {
        if (substr_count($theme->info['php'], '.') < 2) {
          $theme->info['php'] .= '.*';
        }
        $output .= '<div class="incompatible">' . t('This theme requires PHP version @php_required and is incompatible with PHP version !php_version.', array('@php_required' => $theme->info['php'], '!php_version' => phpversion())) . '</div>';
      }
      else {
        $output .= theme('links', array('links' => $theme->operations, 'attributes' => array('class' => array('operations', 'clearfix'))));
      }
      $output .= '</div></div>';
    }
    $output .= '</div>';
  }
  $output .= '</div>';

  return $output;
}
