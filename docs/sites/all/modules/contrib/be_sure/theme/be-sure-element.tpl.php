<?php

/**
 * @file
 * Template for one element.
 */
?>

<?php print $status; ?>
<div class="clearfix"></div>
<?php foreach ($items as $item): ?>
  <div class="messages <?php print $item['status']?>">
    <?php print $item['text']; ?>
  </div>
<?php endforeach; ?>
