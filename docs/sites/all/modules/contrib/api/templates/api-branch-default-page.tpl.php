<?php

/**
 * @file
 * Displays a branch overview page.
 *
 * Available variables:
 * - $branch: Information about the branch to display an overview of.
 *
 * Available variables in the $branch object:
 * - $branch->project: The machine name of the branch.
 * - $branch->title: A proper title for the branch.
 * - $branch->directories: The local included directories.
 * - $branch->excluded_directories: The local excluded directories.
 *
 * @ingroup themeable
 */
?>
<?php if (!empty($branch)) {
  $types = api_listing_types($branch);
  if ($types['groups']) {
?>
  <h3><?php print l(t('Topics'), 'api/' . $branch->project . '/groups/' . $branch->branch_name); ?></h3>
  <?php $out = api_page_listing($branch, 'group', FALSE); print drupal_render($out); ?>
<?php
  }
  if ($types['files']) {
?>
  <h3><?php print l(t('Files'), 'api/' . $branch->project . '/files/' . $branch->branch_name); ?></h3>
<?php
  }
  if ($types['globals']) {
?>
  <h3><?php print l(t('Globals'), 'api/' . $branch->project . '/globals/' . $branch->branch_name); ?></h3>
<?php
  }
  if ($types['constants']) {
?>
  <h3><?php print l(t('Constants'), 'api/' . $branch->project . '/constants/' . $branch->branch_name); ?></h3>
<?php
  }
  if ($types['functions']) {
?>
  <h3><?php print l(t('Functions'), 'api/' . $branch->project . '/functions/' . $branch->branch_name); ?></h3>
<?php
  }
  if ($types['classes']) {
?>
  <h3><?php print l(t('Classes and Interfaces'), 'api/' . $branch->project . '/classes/' . $branch->branch_name); ?></h3>
<?php
  }
  if ($types['namespaces']) {
?>
  <h3><?php print l(t('Namespaces'), 'api/' . $branch->project . '/namespaces/' . $branch->branch_name); ?></h3>
<?php
  }
  if ($types['deprecated']) {
?>
  <h3><?php print l(t('Deprecated'), 'api/' . $branch->project . '/deprecated/' . $branch->branch_name); ?></h3>
<?php
  }
?>
  <h3><?php print t('API search'); ?></h3>
  <?php $form = drupal_get_form('api_search_form', $branch);
        print drupal_render($form);
  ?>
  <?php print api_other_projects_link(); ?>
<?php } ?>

<?php if (user_access('administer API reference')) { ?>
  <p class="api-no-mainpage"><em><?php print t('A main page for this branch has not been indexed. A documentation comment with <code>@mainpage {title}</code> needs to exist, or has not been indexed yet. For older versions of Drupal core, this is available in the <a href="http://drupal.org/project/documentation/git-instructions">documentation project</a> in the <code>developer</code> subdirectory.'); ?></em></p>
<?php } ?>
