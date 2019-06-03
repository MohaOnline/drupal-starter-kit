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
 $node = node_load($row->node_og_membership_nid);
 if ($node->type == 'class') {
   $field_name = 'opigno_class_image';
 } else {
   $field_name = 'opigno_course_image';
 }
 $output = field_view_field('node', $node, $field_name, array('settings' => array('image_style' => 'course_thumbnail_image')));
?>


<?php print drupal_render($output); ?>
