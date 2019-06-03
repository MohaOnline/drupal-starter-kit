<!-- region: top -->
<nav class="region r-top" id="top">
  <div class="inner">
    <?php if ($html = render($page['top'])): ?>
      <?php print $html; ?>
    <?php endif; ?>
  </div>
</nav>

<!-- region: header -->
<header class="region r-header" id="header">
  <div class="inner">
    <?php if (!empty($logo)): ?>
      <div class="logo">
          <?php if (drupal_is_front_page()): ?>
            <h1 class="element-invisible"><?php print check_plain($site_name); ?></h1>
          <?php endif; ?>
          <a href="<?php print url('<front>'); ?>"><img src="<?php print $svg_logo; ?>" onerror="this.src='<?php print $logo; ?>'" alt="<?php print $site_name;?>"/></a>
      </div>
    <?php endif; ?>

    <?php if (!empty($site_slogan)): ?>
      <div class="slogan"><?php print $site_slogan; ?></div>
    <?php endif; ?>

    <?php print render($page['header']); ?>
  </div>
</header>

<!-- dvg_popup -->
<?php if (!empty($page['popup'])): ?>
  <?php print render($page['popup']); ?>
<?php endif; ?>

<!-- system: breadcrumb -->
<nav class="l-breadcrumbs" id="breadcrumbs">
  <div class="inner">
    <div class="wrapper-breadcrumb">
      <?php print render($breadcrumb); ?>
    </div>
  </div>
</nav>

<!-- region: navigation -->
<?php if ($html = render($page['navigation'])): ?>
  <nav class="region r-navigation" id="navigation">
    <div class="inner">
      <?php print $html; ?>
    </div>
  </nav>
<?php endif; ?>

<div id="wrapper">

  <!-- region: content_top -->
  <?php if ($html = render($page['content_top'])): ?>
    <section class="region r-content-top clearfix" id="content-top">
      <div class="inner">
        <?php print $html; ?>
      </div>
    </section>
  <?php endif; ?>

  <div class="l-page" id="page">

    <div class="region r-content" id="content" tabindex="-1">
      <div class="inner">

        <!-- drupal tabs -->
        <?php if ($html = render($tabs)): ?>
          <div class="tabs">
            <?php print $html; ?>
          </div>
        <?php endif; ?>

        <!-- page title -->
        <?php print render($title_prefix); ?>
        <?php if ($title): ?>
          <h1><?php print $title; ?></h1>
        <?php endif; ?>
        <?php print render($title_suffix); ?>

        <!-- region: above_content -->
        <?php if ($html = render($page['above_content'])): ?>
          <div class="region r-above-content" id="above-content">
            <?php print $html; ?>
          </div>
        <?php endif; ?>

        <!-- system: messages -->
        <?php if ($html = render($messages)): ?>
          <div class="l-messages" id="messages"><?php print $html; ?></div>
        <?php endif; ?>

        <!-- region: content -->
        <?php print render($page['content']); ?>

        <!-- region: below_content -->
        <?php if ($html = render($page['below_content'])): ?>
          <div class="region r-below-content" id="below-content">
            <?php print $html; ?>
          </div>
        <?php endif; ?>

      </div><!-- .inner -->
    </div><!-- #content -->

  </div><!-- #page-->

  <!-- region: content_bottom -->
  <?php if($html = render($page['content_bottom'])): ?>
    <section class="region r-content-bottom" id="content-bottom">
      <div class="inner">
        <?php print $html; ?>
      </div>
    </section>
  <?php endif; ?>

  </div><!-- #wrapper-->

<!-- region: footer -->
<footer class="region r-footer" id="footer">
  <div class="inner">

    <!-- region: footer_top -->
    <?php if ($html = render($page['footer_top'])): ?>
      <div class="region r-footer-top" id="footer-top">
        <div class="inner">
          <?php print $html; ?>
        </div>
      </div>
    <?php endif; ?>

    <!-- region: footer_bottom -->
    <?php if ($html = render($page['footer_bottom'])): ?>
    <div class="region r-footer-bottom" id="footer-bottom">
      <div class="inner">
        <?php print $html; ?>
      </div>
    </div>
    <?php endif; ?>

  </div>
</footer>
