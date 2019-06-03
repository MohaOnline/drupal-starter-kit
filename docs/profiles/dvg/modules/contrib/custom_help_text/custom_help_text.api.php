<?php
/**
 * @file
 *   Explanation of hooks.
 */

/**
 * Alter the options of the custom help text list page table.
 *
 * @param $options array All defined options for the table.
 * @param $form array Full form that stores all the info of the page form.
 */
function hook_custom_help_text_table_alter(&$options, $form) {
  $options['#attachment']['css'][] = 'custom.css';
}
