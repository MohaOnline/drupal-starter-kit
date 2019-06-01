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
?>
<header id="navbar" role="banner" class="<?php print $navbar_classes; ?>">
  <div id="wb-bnr">
    <div id="wb-bar">
      <div class="<?php print $container_class; ?>">
        <div class="row">
          <?php if ($site_name || $site_slogan || $logo): ?>
            <?php if ($logo && $logo_svg): ?>
              <img id="gcwu-sig" src='<?php print $logo_svg; ?>' alt="<?php print t('Government of Canada'); ?>" />
            <?php elseif ($logo): ?>
              <img alt="<?php print t('Government of Canada'); ?>" src="<?php print $logo; ?>"  />
            <?php endif; ?>
          <?php endif; ?>
          <?php print $menu_bar; ?>
          <section class="wb-mb-links col-xs-12 visible-sm visible-xs" id="wb-glb-mn">
            <h2><?php print t('Search and menus'); ?></h2>
            <ul class="pnl-btn list-inline text-right">
              <li>
                <a href="#mb-pnl" title="<?php print t('Search and menus'); ?>" aria-controls="mb-pnl" class="overlay-lnk btn btn-xs btn-default" role="button">
                  <span class="glyphicon glyphicon-search">
                    <span class="glyphicon glyphicon-th-list">
                      <span class="wb-inv"><?php print t('Search and menus'); ?></span>
                    </span>
                  </span>
                </a>
              </li>
            </ul>
            <div id="mb-pnl"></div>
          </section>
        </div>
      </div>
    </div>
    <div class="<?php print $container_class; ?>">
      <div class="row">
        <div id="wb-sttl" class="col-md-5">
          <?php if ($site_name || $site_slogan || $logo): ?>
            <a href="<?php print $site_name_url; ?>">
              <span <?php print $logo_class; ?>>
                <?php if ($site_name): ?>
                  <?php print $site_name; ?>
                <?php endif; ?>
              </span>
            </a>
          <?php endif; ?>
        </div>
        <img id="wmms" src="/profiles/wetkit/libraries/theme-gcwu-fegc/assets/wmms.svg" alt="<?php print t('Symbol of the Government of Canada'); ?>" />
        <section id="wb-srch" class="visible-md visible-lg">
          <h2><?php print t('Search'); ?></h2>
          <?php if ($search_box): ?>
            <?php print $search_box; ?>
          <?php endif; ?>
        </section>
      </div>
    </div>
  </div>
  <nav id="wb-sm" class="wb-menu visible-md visible-lg" data-trgt="mb-pnl">
    <div class="<?php print $container_class; ?> nvbar">
      <h2><?php print t('Topics menu'); ?></h2>
      <div class="row">
        <?php print render($page['mega_menu']); ?>
        <?php print render($secondary_nav); ?>
      </div>
    </div>
  </nav>
  <?php print render($page['header']); ?>
  <nav id="wb-bc" property="breadcrumb">
    <h2 class="wb-inv"><?php print t('You are here:'); ?></h2>
    <div class="<?php print $container_class; ?>">
      <div class="row">
        <?php print render($breadcrumb); ?>
      </div>
    </div>
  </nav>
</header>
<?php if (!empty($page['sidebar_first'])): ?>
  <aside class="col-sm-3" role="complementary">
    <?php print render($page['sidebar_first']); ?>
  </aside>
<?php endif; ?>
<main role="main" property="mainContentOfPage" class="<?php print $container_class; ?>">
  <?php if (empty($panels_layout)): ?>
    <?php if (!empty($page['highlighted'])): ?>
      <?php print render($page['highlighted']); ?>
    <?php endif; ?>
    <a id="main-content"></a>
    <?php print render($title_prefix); ?>
    <?php if (!empty($title)): ?>
      <h1 class="page-header" id="wb-cont"><?php print $title; ?></h1>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php if (!empty($tabs)): ?>
      <?php print render($tabs); ?>
    <?php endif; ?>
    <?php if (!empty($page['help'])): ?>
      <?php print render($page['help']); ?>
    <?php endif; ?>
    <?php if (!empty($action_links)): ?>
      <ul class="action-links"><?php print render($action_links); ?></ul>
    <?php endif; ?>
  <?php endif; ?>
  <?php if (!empty($messages)): ?>
    <?php print render($messages); ?>
  <?php endif; ?>
  <?php print render($page['content']); ?>
</main>
<?php if (!empty($page['sidebar_second'])): ?>
  <aside class="col-sm-3" role="complementary">
    <?php print render($page['sidebar_second']); ?>
  </aside>
<?php endif; ?>
<footer role="contentinfo" id="wb-info" class="visible-sm visible-md visible-lg wb-navcurr">
    <div class="<?php print $container_class; ?>">
      <nav>
        <h2><?php print t('About this site'); ?></h2>
        <?php print $page['menu_terms_bar']; ?>
        <div class="row">
        <?php print render($page['footer']); ?>
        </div>
      </nav>
    </div>
    <div id="gc-info">
      <div class="<?php print $container_class; ?>">
        <nav>
          <?php print $page['menu_footer_bar']; ?>
        </nav>
    </div>
  </div>
</footer>
