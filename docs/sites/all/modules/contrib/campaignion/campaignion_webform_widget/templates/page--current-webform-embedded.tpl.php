<?php if ($messages): ?>
  <div id="messages"><div class="section middle clearfix">
    <?php print $messages; ?>
  </div></div> <!-- /.section, /#messages -->
<?php endif; ?>

<?php if ($page['widget']): ?>
  <div class="widget">
    <?php print render($page['widget']); ?>
  </div>
<?php endif; ?>
