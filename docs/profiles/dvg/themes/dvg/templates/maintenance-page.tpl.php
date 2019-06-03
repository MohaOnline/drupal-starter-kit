<!doctype html>
<!--[if lt IE 7]> <html lang="<?php print $language->language; ?>" class="html ie ie6"> <![endif]-->
<!--[if IE 7]>    <html lang="<?php print $language->language; ?>" class="html ie ie7"> <![endif]-->
<!--[if IE 8]>    <html lang="<?php print $language->language; ?>" class="html ie ie8"> <![endif]-->
<!--[if gt IE 8]><!--> <html lang="<?php print $language->language; ?>" class="html no-ie"> <!--<![endif]-->

<head>
  <meta name="viewport" content="width=device-width" />
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>

<body class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <!-- region: top -->
  <?php //if ($html = render($page['top'])): ?>
    <nav class="region r-top" id="top">
      <div class="inner">
        <?php //print $html; ?>
      </div>
    </nav>
  <?php //endif; ?>

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

      <!-- region: header -->
      <?php print render($page['header']); ?>

    </div>
  </header>

  <div id="wrapper">
    <div class="l-page" id="page">
      <div class="region r-content" id="content">
        <div class="inner">

          <?php if ($messages): ?>
            <div id="console"><?php print $messages; ?></div>
          <?php endif; ?>
          <?php if ($help): ?>
            <div id="help">
              <?php print $help; ?>
            </div>
          <?php endif; ?>

        <!-- region: sidebar_first -->
        <?php if ($sidebar_first): ?>
          <div id="sidebar-first" class="sidebar">
            <?php print $sidebar_first ?>
          </div>
        <?php endif; ?>

        <!-- region: content -->
        <div class="content">
                    <!-- page title -->
          <?php print render($title_prefix); ?>
          <?php if ($title): ?>
            <h1><?php print $title; ?></h1>
          <?php endif; ?>
          <?php print render($title_suffix); ?>
        
        <?php print $content; ?>
        </div>

        </div><!-- .inner -->
      </div><!-- #content -->

    </div><!-- #page-->
  </div>

</body>

</html>
