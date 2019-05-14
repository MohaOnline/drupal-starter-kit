<?php
/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * The doctype, html, head and body tags are not in this template. Instead they
 * can be found in the html.tpl.php template in this directory.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/bartik.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 *
 * @see bootstrap_preprocess_page()
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see bootstrap_process_page()
 * @see template_process()
 * @see html.tpl.php
 *
 * @ingroup themeable
 */

  global $theme;
?>
<?php print $boxed_cnt; ?>

<?php if (!empty($secondary_nav) || !empty($page['secondary_header'])): ?>
<header id="secondary-header" role="banner" class="<?php print $glazed_free_secondary_header_classes; ?>">
  <div class="<?php print $cnt_secondary_header ?> secondary-header-container">
    <?php print $cnt_secondary_header_squeeze_start; ?>
        <?php if (!empty($page['secondary_header'])): ?>
          <?php print render($page['secondary_header']); ?>
        <?php endif; ?>
        <?php if (!isset($hide_navigation) && !empty($secondary_nav)): ?>
           <?php  print render($secondary_nav); ?>
        <?php endif; ?>
    <?php print $cnt_secondary_header_squeeze_end; ?>
  </div>
</header>
<?php endif; ?>

<?php if (!isset($hide_header)): ?>
<header id="navbar" role="banner" class="<?php print $glazed_free_header_classes; ?>"<?php if (isset($header_affix)): print $header_affix; endif; ?>>
  <div class="<?php print $cnt_header_nav ?> navbar-container">
    <?php print $cnt_header_nav_squeeze_start; ?>
        <div class="navbar-header">
          <?php if ($logo OR !empty($site_name)): ?>
          <div class="wrap-branding">
          <?php if ($logo): ?>
            <a class="logo navbar-btn" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
              <img src="<?php print $logo; ?>" id="logo" alt="<?php print t('Home'); ?>" />
            </a>
          <?php endif; ?>

          <?php if (!empty($site_name)): ?>
            <a class="name navbar-brand" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
          <?php endif; ?>
          </div>
          <?php endif; ?>

          <?php if ((!isset($hide_navigation)) && !empty($primary_nav) || !empty($page['navigation'])): ?>
            <a id="glazed-menu-toggle" href="#"><span></span></a>
          <?php endif; ?>
        </div>

        <?php if ((!isset($hide_navigation)) && !empty($primary_nav) || !empty($page['navigation'])): ?>
          <nav role="navigation" id="glazed-main-menu" class="glazed-main-menu">
            <?php if (!empty($primary_nav)): ?>
               <?php  print render($primary_nav); ?>
            <?php endif; ?>
            <?php if (!empty($page['navigation'])): ?>
              <?php print render($page['navigation']); ?>
            <?php endif; ?>
          </nav>
        <?php endif; ?>
    <?php print $cnt_header_nav_squeeze_end; ?>
  </div>
</header>
<?php endif; ?>

<div class="wrap-containers">

  <?php print render($page['slider']); ?>

  <?php if ((!empty($title) OR !empty($breadcrumb) OR render($page['header'])) && (!isset($hide_page_title))): ?>
  <div class="page-title-full-width-container<?php print $cnt_page_title_full_width; ?>" id="page-title-full-width-container">
    <header role="banner" id="page-title" class="<?php print $cnt_page_title; ?> page-title-container">
      <?php print render($title_prefix); ?>
      <?php if (!empty($title)): ?>
      <h1 class="<?php print $glazed_free_title_classes; ?>"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php print render($page['header']); ?>
    </header> <!-- /#page-title -->
    <?php if (!empty($breadcrumb) && theme_get_setting('page_title_breadcrumbs', $theme)):
    print '<div class="hidden-xs ' .  $cnt_page_title . ' breadcrumb-container">' . $breadcrumb . '</div>';
    endif; ?>
  </div>

  <?php endif; ?>

  <?php if (!empty($page['highlighted'])): ?>
  <div class="container highlighted-container">
    <div class="row container-row">
      <div class="col-sm-12 container-col">
        <div class="highlighted jumbotron"><?php print render($page['highlighted']); ?></div>
      </div>
    </div>
  </div>
  <?php endif; ?>

  <?php if (!empty($page['content_top'])): ?>
  <div class="<?php print $cnt_content_top ?> content-top-container">
    <?php print $cnt_content_top_squeeze_start; ?>
      <div class="content-top"><?php print render($page['content_top']); ?></div>
    <?php print $cnt_content_top_squeeze_end; ?>
  </div>
  <?php endif; ?>
  <div<?php print $content_container_class; ?>>
    <div<?php print $content_row_class; ?>>
      <?php if (!empty($page['sidebar_first'])): ?>
        <aside class="col-sm-3" role="complementary">
          <?php print render($page['sidebar_first']); ?>
        </aside>  <!-- /#sidebar-first -->
      <?php endif; ?>

      <section<?php print $content_column_class; ?>>
        <a id="main-content"></a>
        <?php print $messages; ?>
        <?php if (!empty($tabs)): ?>
          <?php print render($tabs); ?>
        <?php endif; ?>
        <?php if (!empty($page['help'])): ?>
          <?php print render($page['help']); ?>
        <?php endif; ?>
        <?php if (!empty($action_links)): ?>
          <ul class="action-links"><?php print render($action_links); ?></ul>
        <?php endif; ?>
        <?php print render($page['content']); ?>
      </section>

      <?php if (!empty($page['sidebar_second'])): ?>
        <aside class="col-sm-3" role="complementary">
          <?php print render($page['sidebar_second']); ?>
        </aside>  <!-- /#sidebar-second -->
      <?php endif; ?>

    </div>
  </div>

  <?php if (!empty($page['content_bottom'])): ?>
  <div class="<?php print $cnt_content_bottom ?> content-bottom-container">
    <?php print $cnt_content_bottom_squeeze_start; ?>
      <div class="content-bottom"><?php print render($page['content_bottom']); ?></div>
    <?php print $cnt_content_bottom_squeeze_end; ?>
  </div>
  <?php endif; ?>
</div>

<!-- /#Sticky Footer -->
<?php if (!empty($page['footer'])): ?>
<footer class="glazed-footer clearfix">
  <div class="<?php print $cnt_footer ?> footer-container">
    <?php print $cnt_footer_squeeze_start; ?>
        <?php print render($page['footer']); ?>
        <?php print $sooperthemes_attribution_link; ?>
    <?php print $cnt_footer_squeeze_end; ?>
  </div>
</footer>
<?php endif; ?>

<?php print $boxed_cnt_end; ?>
