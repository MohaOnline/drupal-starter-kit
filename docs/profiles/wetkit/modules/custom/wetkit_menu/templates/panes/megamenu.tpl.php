<?php
/**
 * @file panels-pane.tpl.php
 * Main panel pane template
 *
 * Variables available:
 * - $pane->type: the content type inside this pane
 * - $pane->subtype: The subtype, if applicable. If a view it will be the
 *   view name; if a node it will be the nid, etc.
 * - $title: The title of the content
 * - $content: The actual content
 * - $links: Any links associated with the content
 * - $more: An optional 'more' link (destination only)
 * - $admin_links: Administrative links associated with the content
 * - $feeds: Any feed icons or associated with the content
 * - $display: The complete panels display object containing all kinds of
 *   data including the contexts and all of the other panes being displayed.
 */
?>
<?php if ($pane_prefix): ?>
  <?php print $pane_prefix; ?>
<?php endif; ?>
<?php print render($title_prefix); ?>
  <?php if ($title): ?>
    <?php if (!theme_get_setting('wetkit_render_no_link')): ?>
      <h4><?php print $title; ?></h4>
    <?php else: ?>
      <h4 data-role="list-divider"><?php print $title; ?></h4>
    <?php endif; ?>
  <?php endif; ?>
<?php print render($title_suffix); ?>
<?php print render($content); ?>
<?php if ($pane_suffix): ?>
  <?php print $pane_suffix; ?>
<?php endif; ?>
