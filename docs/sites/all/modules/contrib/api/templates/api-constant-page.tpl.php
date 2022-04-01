<?php

/**
 * @file
 * Displays an API page for a constant.
 *
 * Available variables:
 * - $alternatives: List of alternate versions (branches) of this constant.
 * - $documentation: Documentation from the comment header of the constant.
 * - $override: If this is an override, the text to show for that.
 * - $namespace: Name of the namespace for this function, if any.
 * - $see: See also documentation.
 * - $deprecated: Deprecated documentation.
 * - $defined: HTML reference to file that defines this constant.
 * - $class: The text for the class section.
 * - $call_links: Links to functions that use this constant.
 * - $code: HTML-formatted declaration of this constant.
 * - $related_topics: List of related groups/topics.
 * - $branch: Object with information about the branch.
 * - $object: Object with information about the constant.
 *
 * Available variables in the $branch object:
 * - $branch->project: The machine name of the branch.
 * - $branch->title: A proper title for the branch.
 * - $branch->directories: The local included directories.
 * - $branch->excluded_directories: The local excluded directories.
 *
 * Available variables in the $object object:
 * - $object->title: Display name.
 * - $object->object_type: For this template it will be 'constant'.
 * - $object->branch_id: An identifier for the branch.
 * - $object->file_name: The path to the file in the source.
 * - $object->summary: A one-line summary of the object.
 * - $object->code: Escaped source code.
 * - $object->see: HTML-formatted additional references.
 *
 * @see api_preprocess_api_object_page()
 *
 * @ingroup themeable
 */
?>
<?php print $alternatives; ?>

<?php print $documentation ?>

<?php print $override; ?>

<?php if (!empty($deprecated)) { ?>
<div class="api-deprecated">
  <h3><?php print t('Deprecated') ?></h3>
  <?php print $deprecated ?>
</div>
<?php } ?>

<?php if (!empty($see)) { ?>
  <h3><?php print t('See also') ?></h3>
  <?php print $see ?>
<?php } ?>

<?php if (!empty($related_topics)) { ?>
<h3><?php print t('Related topics') ?></h3>
<?php print $related_topics ?>
<?php } ?>

<?php
foreach ($call_links as $link) {
  print $link;
} ?>

<h3><?php print t('File'); ?></h3>
<?php print $defined; ?>

<?php if ($class) : ?>
  <h3><?php print t('Class'); ?></h3>
  <?php print $class; ?>
<?php endif; ?>

<?php if ($namespace) : ?>
  <h3><?php print t('Namespace'); ?></h3>
  <?php print $namespace; ?>
<?php endif; ?>

<h3><?php print t('Code'); ?></h3>
<?php print $code; ?>
