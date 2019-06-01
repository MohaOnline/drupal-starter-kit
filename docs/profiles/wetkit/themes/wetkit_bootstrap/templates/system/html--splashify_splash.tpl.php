<?php
/**
 * @file
 * Default theme implementation to display the basic html structure of a single
 * Drupal page.
 *
 * Variables:
 * - $css: An array of CSS files for the current page.
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or
 *   'rtl'.
 * - $rdf_namespaces: All the RDF namespace prefixes used in the HTML document.
 * - $grddl_profile: A GRDDL profile allowing agents to extract the RDF data.
 * - $head_title: A modified version of the page title, for use in the TITLE
 *   tag.
 * - $head_title_array: (array) An associative array containing the string parts
 *   that were used to generate the $head_title variable, already prepared to be
 *   output as TITLE tag. The key/value pairs may contain one or more of the
 *   following, depending on conditions:
 *   - title: The title of the current page, if any.
 *   - name: The name of the site.
 *   - slogan: The slogan of the site, if any, and if there is no title.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $page_top: Initial markup from any modules that have altered the
 *   page. This variable should always be output first, before all other dynamic
 *   content.
 * - $page: The rendered page content.
 * - $page_bottom: Final closing markup from any modules that have altered the
 *   page. This variable should always be output last, after all other dynamic
 *   content.
 * - $classes String of classes that can be used to style contextually through
 *   CSS.
 *
 * @see bootstrap_preprocess_html()
 * @see template_preprocess()
 * @see template_preprocess_html()
 * @see template_process()
 *
 * @ingroup templates
 */
?><!DOCTYPE html>
<!--[if lt IE 9]><html<?php print $html_attributes; ?> class="no-js lt-ie9"><![endif]-->
<!--[if gt IE 8]><!-->
<html<?php print $html_attributes;?><?php print $rdf_namespaces;?>>
<!--<![endif]-->
<head>
  <link rel="profile" href="<?php print $grddl_profile; ?>" />
  <meta charset="utf-8">
  <meta content="width=device-width,initial-scale=1" name="viewport" >
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <!-- jQuery needs to be loaded first for IE6-8 -->
  <!--[if lt IE 9]>
    <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
  <![endif]-->
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body<?php print $body_attributes; ?>>
  <ul id="wb-tphp">
    <?php if ($wetkit_skip_link_id_1 && $wetkit_skip_link_text_1): ?>
      <li class="wb-slc">
        <a class="wb-sl" href="#<?php print $wetkit_skip_link_id_1; ?>"><?php print $wetkit_skip_link_text_1; ?></a>
      </li>
    <?php endif; ?>
    <?php if ($wetkit_skip_link_id_2 && $wetkit_skip_link_text_2): ?>
      <li class="wb-slc visible-md visible-lg">
        <a class="wb-sl" href="#<?php print $wetkit_skip_link_id_2; ?>"><?php print $wetkit_skip_link_text_2; ?></a>
      </li>
    <?php endif; ?>
  </ul>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>
