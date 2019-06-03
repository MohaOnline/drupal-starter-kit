<?php
/**
 * @file
 * Spell suggestion template.
 *
 * Available variables:
 * - $spell: An array of spell information.
 */
?>
<?php if (!empty($spell)): ?>
  <div id="funnelback-spell">
    <p>Did you mean:
      <?php foreach ($spell as $suggestion): ?>
        <a href='?<?php print $suggestion['url'] ?>'><?php print $suggestion['text'] ?></a>
      <?php endforeach; ?>
    </p>
  </div>
<?php endif; ?>
