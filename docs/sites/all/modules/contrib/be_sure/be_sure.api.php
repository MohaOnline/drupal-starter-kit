<?php
/**
 * @file
 * Hooks provided by the Be sure module.
 */

/**
 * Allow you to define own sure item with categories and elements.
 *
 * @return array
 *   Array of sure items, key of each element will be used as part of path.
 *   Each item must contain the following values:
 *    - title
 *    - description
 *    - array of elements (more than 0)
 *
 *   Each category (element) must contain the following values:
 *    - title
 *    - array of items
 *
 *   Each item must contain 'ok' and 'nok' texts, also it should include
 *   valid callback function for checking the item.
 *   Callback function must return bool value.
 */
function hook_sure_info() {
  $items['example'] = array(
    'title' => 'Example',
    'description' => 'Description of Example',
    'elements' => array(
      array(
        'title' => 'Example category 1',
        'items' => array(
          array(
            'ok' => 'Something enabled and well configured',
            'nok' => 'Something disabled or not well configured',
            'callback' => 'module_example_category_1_something',
          ),
        ),
      ),
      array(
        'title' => 'Example category 2',
        'items' => array(
          array(
            'ok' => 'Some value greater or equal than 100',
            'nok' => 'Some value less than 100',
            'callback' => 'module_example_category_2_other',
          ),
        ),
      ),
    ),
  );

  return $items;
}
