<?php print render($page['content_top']); ?>
<?php print render($title_prefix); ?>
<?php if ($title): ?>
  <h1><?php print $title; ?></h1>
<?php endif; ?>
<?php print render($title_suffix); ?>
<?php print render($page['content']); ?>
<?php if ($html = render($page['content_bottom'])): ?>
  <hr />
  <?php print $html; ?>
<?php endif; ?>
