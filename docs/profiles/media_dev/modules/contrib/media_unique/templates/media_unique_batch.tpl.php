<?php if (!empty($sha1_status_message)) :?>
<div class="media_unique alert alert-block alert-success messages status">
  <?php print $sha1_status_message; ?>
</div>
<?php endif; ?>

<?php if (!empty($batch_form)) :?>
<div class="form">
  <?php print $batch_form; ?>
</div>
<?php endif; ?>
