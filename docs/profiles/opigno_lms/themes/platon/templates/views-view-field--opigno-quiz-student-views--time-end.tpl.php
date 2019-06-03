<?php

/**
 * @file
 * This template is used to print a single field in a view.
 *
 * It is not actually used in default Views, as this is registered as a theme
 * function which has better performance. For single overrides, the template is
 * perfectly okay.
 *
 * Variables available:
 * - $view: The view object
 * - $field: The field handler object that can process the input
 * - $row: The raw SQL result that can be used
 * - $output: The processed output that will normally be used.
 *
 * When fetching output from the $row, this construct should be used:
 * $data = $row->{$field->field_alias}
 *
 * The above will guarantee that you'll always get the correct data,
 * regardless of any changes in the aliasing that might happen if
 * the view is modified.
 */
$duration = ($row->quiz_node_results_is_evaluated) ? $row->quiz_node_results_time_end - $row->quiz_node_results_time_start : 0 ;
$duration = sprintf('%02d:%02d:%02d', ($duration / 3600), ($duration / 60 % 60), $duration % 60);
?>

<div><?php print $output; ?></div>
<?php if ($row->quiz_node_results_is_evaluated): ?>
  <div><?= t('Duration:') . ' ' . $duration ?></div>
<?php endif; ?>
