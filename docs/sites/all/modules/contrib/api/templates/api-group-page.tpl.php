<?php

/**
 * @file
 * Displays an API page for a topic/group, with list of items in that group.
 *
 * Available variables:
 * - $alternatives: List of alternate versions (branches) of this group.
 * - $hide_alternatives: TRUE to hide the list of alternative versions.
 * - $documentation: Documentation from the comment header of the constant.
 * - $see: Related API objects.
 * - $related_topics: Related topics documentation.
 * - $objects: Formatted list of member objects, if any.
 * - $defined: HTML reference to file that defines this group.
 *
 * @ingroup themeable
 */
?>

<?php
if (!$hide_alternatives) {
  print $alternatives;
}
?>

<?php print $documentation ?>

<?php if (!empty($see)) { ?>
  <h3><?php print t('See also') ?></h3>
  <?php print $see ?>
<?php } ?>

<?php if (!empty($related_topics)) { ?>
<h3><?php print t('Parent topics') ?></h3>
<?php print $related_topics ?>
<?php } ?>

<?php if (isset($defined)) { ?>
  <h3><?php print t('File'); ?></h3>
  <?php print $defined; ?>
<?php } ?>

<?php print $objects; ?>
