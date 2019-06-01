<!DOCTYPE html><!--[if lt IE 9]><html class="no-js lt-ie9" lang="en" dir="ltr"><![endif]--><!--[if gt IE 8]><!-->
<html class="no-js" lang="en" dir="ltr"> <!--<![endif]-->
<head>
<meta charset="utf-8"> <!-- Web Experience Toolkit (WET) / Boîte à outils de l'expérience Web (BOEW)
    wet-boew.github.io/wet-boew/License-en.html / wet-boew.github.io/wet-boew/Licence-fr.html -->
<title><?php print $head_title; ?></title>
<meta content="width=device-width,initial-scale=1" name="viewport"> <!-- Meta data -->
<meta name="robots" content="noindex, nofollow, noarchive"> <!-- Meta data--> <!--[if gte IE 9 | !IE ]><!-->
<link href="/profiles/wetkit/libraries/theme-wet-boew/assets/favicon.ico" rel="icon" type="image/x-icon">
<link rel="stylesheet" href="/profiles/wetkit/libraries/theme-wet-boew/css/wet-boew.min.css"> <!--<![endif]-->
<link rel="stylesheet" href="/profiles/wetkit/libraries/theme-wet-boew/css/theme.min.css"> <!--[if lt IE 9]>
<link href="/profiles/wetkit/libraries/theme-wet-boew/assets/favicon.ico" rel="shortcut icon" />
<link rel="stylesheet" href="/profiles/wetkit/libraries/theme-wet-boew/css/ie8-wet-boew.min.css" />
<script src="/profiles/wetkit/modules/contrib/jquery_update/replace/jquery/1.8/jquery.min.js"></script>
<script src="/profiles/wetkit/libraries/wet-boew/js/ie8-wet-boew.min.js"></script>
<![endif]-->
<noscript><link rel="stylesheet" href="/profiles/wetkit/libraries/wet-boew/css/noscript.min.css" /></noscript>
</head>
<body vocab="http://schema.org/" typeof="WebPage">
<main role="main" property="mainContentOfPage" class="<?php print $container_class; ?>">
<div class="row mrgn-tp-lg">
<h1 class="wb-inv"><?php print $site_name; ?></h1>
<!-- MainContentStart -->
<?php if (!$db_down): ?>
<div class="col-md-12">
<section>
<?php if ($title): ?>
<h2><?php print $title; ?></h1>
<?php endif; ?>
<?php print $messages; ?>
<?php print $content; ?>
</section>
</div>
<div class="clear"></div>
<?php endif; ?>
<?php if ($db_down): ?>
<section class="col-md-6">
<h2><span class="glyphicon glyphicon-warning-sign mrgn-rght-md"></span><?php print $wxt_title_en; ?></h2>
<p><?php print $wxt_content_en; ?></p>
</section>
<section class="col-md-6" lang="fr">
<h2><span class="glyphicon glyphicon-warning-sign mrgn-rght-md"></span><?php print $wxt_title_fr; ?></h2>
<p><?php print $wxt_content_fr; ?></p>
</section>
<?php endif; ?>
<!-- MainContentEnd -->
</div>
</main>
</body>
</html>
