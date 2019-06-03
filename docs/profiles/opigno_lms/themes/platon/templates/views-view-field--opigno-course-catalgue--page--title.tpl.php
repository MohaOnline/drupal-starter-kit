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
 $node = node_load($row->nid);
 if (!empty($node->opigno_commerce_product) && !empty($node->opigno_commerce_price)) {
  $view->result[$view->row_index]->field_group_group[0]['rendered']['#title'] = t('Buy access');
  $view->result[$view->row_index]->field_group_group[0]['rendered']['#options']['attributes']['title'] = t('Buy access');
  $view->result[$view->row_index]->field_group_group[0]['rendered']['#options']['attributes']['class'][0] = 'group buy';
  $amount = commerce_currency_format($node->opigno_commerce_price['und'][0]['amount'],$node->opigno_commerce_price['und'][0]['currency_code']);
}
?>
<div class="content-top">
  <div class="title">
    <?php print $output; ?>
  </div>
  <div class="content-more default-visible">
    <a href="#"><?php print t('view') ?></a>
  </div>
  <?php if (isset($amount)): ?>
    <div class="content-price default-visible">
      <span><?php print $amount ?></span>
    </div>
  <?php endif; ?>
  <div class="default-hidden">
    <?php
      $body = $view->render_field('body', $view->row_index);
      $body = str_replace('<br>', ' ', $body);
      $body = strip_tags($body);
    ?>
    <?php print views_trim_text(array('max_length' => 75,'ellipsis' => true,'word_boundary' => true,'html' => true), $body) ?>
  </div>
  <div class="default-hidden">
    <a href="#" class="close-btn"></a>
  </div>
</div>

<div class="content-bottom">
  <div class="pictogram clearfix">
    <?php print $view->render_field('group_group', $view->row_index) ?>
  </div>
</div>
