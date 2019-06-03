<div class="clearfix wizard-title">
  <h1 class="page-title"><?php echo drupal_get_title(); ?></h1>
  <?php if (!empty($form['buttons'])) { echo render($form['buttons']); } ?>
</div>
<div class="wizard-head">
  <?php echo render($form['trail']); ?>
</div>
<div id="wizard-main"><?php
  hide($form['wizard_secondary']);
  echo drupal_render_children($form, element_children($form, TRUE));
?></div>
<?php echo render($form['wizard_secondary']); ?>
