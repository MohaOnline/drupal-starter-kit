<?php
/**
 * @file
 * Template for multiple categories.
 */
?>

<div class="tabs clearfix">
  <ul id="be-sure-tabs" class="tabs primary primary-tabs">
    <li class="active">
      <?php print $first_title;?>
    </li>

    <?php foreach ($titles as $title): ?>
      <li>
        <?php print $title;?>
      </li>
    <?php endforeach;?>
  </ul>
</div>

<div class="clearfix"></div>

<div id="be-sure">
  <div id="<?php print $first_element_id?>" class="element">
    <?php print $first_element; ?>
  </div>

  <?php foreach ($elements as $id => $element): ?>
    <div id="<?php print $id?>" class="element" style="display: none">
      <?php print $element; ?>
    </div>
  <?php endforeach; ?>
</div>
