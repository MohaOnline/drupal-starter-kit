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
<div id="bg">
  <img src="<?php print $variables['background']; ?>" alt="" />
</div>
<main role="main">
  <div class="sp-hb">
    <div class="sp-bx col-xs-12">
      <h1 property="name" class="wb-inv"><?php print t('Open Government'); ?></h1>
      <div class="row">
        <div class="col-xs-11 col-md-8">
          <img src="/profiles/wetkit/libraries/theme-gcweb/assets/sig-spl.svg" alt="<?php print t('Government of Canada / Gouvernement du Canada'); ?>" />
        </div>
      </div>
      <div class="row">
        <section class="col-xs-6 text-right">
          <h2 class="wb-inv"><?php print t('Government of Canada'); ?></h2>
          <p><a href="<?php print '/' . $language_prefix; ?>" class="btn btn-primary"><?php print t('English'); ?></a></p>
        </section>
        <section class="col-xs-6" lang="fr">
          <h2 class="wb-inv"><?php print t('Gouvernement du Canada'); ?></h2>
          <p><a href="<?php print '/' . $language_prefix_alt; ?>" class="btn btn-primary"><?php print t('Français'); ?></a></p>
        </section>
      </div>
    </div>
    <div class="sp-bx-bt col-xs-12">
      <div class="row">
        <div class="col-xs-7 col-md-8">
          <a href="http://www.canada.ca/en/transparency/terms.html" class="sp-lk"><?php print t('Terms & conditions'); ?></a>
          <span class="glyphicon glyphicon-asterisk"></span>
          <a href="http://www.canada.ca/fr/transparence/avis.html" class="sp-lk" lang="fr"><?php print t('Avis'); ?></a>
        </div>
        <div class="col-xs-5 col-md-4 text-right mrgn-bttm-md">
          <img src="/profiles/wetkit/libraries/theme-gcweb/assets/wmms-spl.svg" alt="<?php print t('Symbol of the Government of Canada / Symbole du gouvernement du Canada'); ?>" />
        </div>
      </div>
    </div>
  </div>
</main>
