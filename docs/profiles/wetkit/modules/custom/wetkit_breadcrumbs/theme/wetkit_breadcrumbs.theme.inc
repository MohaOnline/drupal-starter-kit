<?php
/**
 * @file
 * wetkit_breadcrumbs.theme.inc
 */

/**
 * Override default theme_breadcrumb().
 *
 * @param array $variables
 *   Contains array with breadcrumbs.
 *
 * @return bool|string
 *   Rendered breadcrumbs or FALSE for no breadcrumbs.
 */
function wetkit_breadcrumbs_breadcrumb($variables) {
  $breadcrumbs = $variables['breadcrumb'];
  if (!empty($breadcrumbs)) {

    // Hide breadcrumb navigation if it contains only one element.
    $hide_single_breadcrumb = variable_get('path_breadcrumbs_hide_single_breadcrumb', 0);
    if ($hide_single_breadcrumb && count($breadcrumbs) == 1) {
      return FALSE;
    }

    // Bootstrap 3 compatibility. See: https://drupal.org/node/2178565
    if (is_array($breadcrumbs[count($breadcrumbs) - 1])) {
      array_pop($breadcrumbs);
    }

    $path_breadcrumbs_data = path_breadcrumbs_load_variant(current_path());
    // Append page title when un-managed by path_breadcrumbs
    $show_unlinked_page_title = variable_get('path_breadcrumbs_show_page_title', 0);
    if ($show_unlinked_page_title && !isset($path_breadcrumbs_data->variant)) {
      // Only append if the last item is a link.
      $last_breadcrumb = end($breadcrumbs);
      if (stripos($last_breadcrumb, "href=") !== FALSE) {
        $page_title = drupal_get_title();
        // the title could be blank if we are displaying via the menu tree
        $page_title = (empty($page_title)) ? menu_get_active_title() : $page_title;
        $breadcrumbs[] = $page_title;
      }
    }

    // Add options for rich snippets.
    $elem_tag = 'li';
    $elem_property = '';
    $root_property = '';
    $options = array('html' => TRUE);
    $snippet = variable_get('path_breadcrumbs_rich_snippets', PATH_BREADCRUMBS_RICH_SNIPPETS_DISABLED);
    if ($snippet == PATH_BREADCRUMBS_RICH_SNIPPETS_RDFA) {

      // Add link options for RDFa support.
      $options['attributes'] = array('rel' => 'v:url', 'property' => 'v:title');
      $options['absolute'] = TRUE;

      // Set correct properties for RDFa support.
      $elem_property = ' typeof="v:Breadcrumb"';
      $root_property = ' xmlns:v="http://rdf.data-vocabulary.org/#"';
    }
    elseif ($snippet == PATH_BREADCRUMBS_RICH_SNIPPETS_MICRODATA) {

      // Add link options for microdata support.
      $options['attributes'] = array('itemprop' => 'url');
      $options['absolute'] = TRUE;

      // Set correct properties for microdata support.
      $elem_property = ' itemscope itemtype="http://data-vocabulary.org/Breadcrumb"';
      $elem_tag = 'div';

      // Add style that will display breadcrumbs wrapped in <div> inline.
      drupal_add_css(drupal_get_path('module', 'path_breadcrumbs') . '/css/path_breadcrumbs.css');
    }

    foreach ($breadcrumbs as $key => $breadcrumb) {

      // Build classes for the breadcrumbs.
      $classes = array('inline');
      $classes[] = $key % 2 ? 'even' : 'odd';
      if ($key == 0) {
        $classes[] = 'first';
      }
      if (count($breadcrumbs) == $key + 1) {
        $classes[] = 'last';
      }

      // For rich snippets support all links should be processed in the same way
      // even if they are provided not by Path Breadcrumbs module. So I have to
      // parse html code and create links again with new properties.
      preg_match('/href="([^"]+?)"/', $breadcrumb, $matches);

      // Remove base path from href.
      $href = '';
      if (!empty($matches[1])) {
        global $base_path;
        global $language;

        $base_string = rtrim($base_path, "/");

        // Append additional params to base string if clean urls are disabled.
        if (!variable_get('clean_url', 0)) {
          $base_string .= '?q=';
        }

        // Append additional params to base string for multilingual sites.
        // @note: Only core URL detection method supported.
        $enabled_negotiation_types = variable_get("language_negotiation_language", array());
        if (!empty($enabled_negotiation_types['locale-url']) && !empty($language->prefix)) {
          $base_string .= '/' . $language->prefix;
        }

        // Means that this is href to the frontpage.
        if ($matches[1] == $base_string || $matches[1] == '' || $matches[1] == '/') {
          $href = '';
        }
        // All hrefs exept frontpage.
        elseif (stripos($matches[1], "$base_string/") === 0) {
          $href = drupal_substr($matches[1], drupal_strlen("$base_string/"));
        }
        // Other cases.
        else {
          // HREF param can't starts with '/'.
          $href = stripos($matches[1], '/') === 0 ? drupal_substr($matches[1], 1) : $matches[1];
        }

        // If HREF param is empty it should be linked to a front page.
        $href = empty($href) ? '<front>' : $href;
      }

      // Get breadcrumb title from a link like "<a href = "/path">title</a>".
      $title = trim(strip_tags($breadcrumb, '<em><abbr>'));

      // Wrap title in additional element for microdata support.
      if ($snippet == PATH_BREADCRUMBS_RICH_SNIPPETS_MICRODATA) {
        $title = '<span itemprop="title">' . $title . '</span>';
      }

      // Support title attribute.
      if (preg_match('/<a\s.*?title="([^"]+)"[^>]*>/i', $breadcrumb, $attr_matches)) {
        $options['attributes']['title'] = $attr_matches[1];
      }
      else {
        unset($options['attributes']['title']);
      }

      // Decode url to prevent double encoding in l().
      $href = rawurldecode($href);
      // Move query params from $href to $options.
      $href = _path_breadcrumbs_clean_url($href, $options, 'none');

      // Build new text or link breadcrumb.
      $new_breadcrumb = !empty($href) ? l($title, $href, $options) : $title;

      // Replace old breadcrumb link with a new one.
      $breadcrumbs[$key] = '<' . $elem_tag . ' class="' . implode(' ', $classes) . '"' . $elem_property . '>' . $new_breadcrumb . '</' . $elem_tag . '>';
    }

    // Get breadcrumb delimiter and wrap it into <span> for customization.
    $delimiter = variable_get('path_breadcrumbs_delimiter', '»');
    $delimiter = '<span class="delimiter">' . trim($delimiter) . '</span>';

    $classes = array('breadcrumb');

    // Show contextual link if it is Path Breadcrumbs variant.
    $prefix = '';
    if (user_access('administer path breadcrumbs') && $path_breadcrumbs_data && isset($path_breadcrumbs_data->variant)) {
      $contextual_links = array(
        '#type' => 'contextual_links',
        '#contextual_links' => array('path_breadcrumbs' => array('admin/structure/path-breadcrumbs/edit', array($path_breadcrumbs_data->variant->machine_name))),
      );
      $prefix = drupal_render($contextual_links);
    }

    // Build final version of breadcrumb's HTML output.
    $delimiter = '';
    $output = '<div class="contextual-links-region">' . $prefix . '<ol class="' . implode(' ', $classes) . '"' . $root_property . '>' . implode(" $delimiter ", $breadcrumbs) . '</ol></div>';

    return $output;
  }

  // Return false if no breadcrumbs.
  return FALSE;
}
