<?php
/**
 * Template file
 * variables:
 * $content -> the form that handles the submission of the signature
 * $user -> a drupal object representing the current user
 * $expire -> the expiration date of the signature period (NULL if no expiration)
 * $date -> the current date and time 
 */ 
?>
<div id = "sign_for_acknowledgement_checkbox" ><?php print $content ?></div>
<?php if ($expire) : ?>
  <div>
  <em>
  <?php
  print t('Note: you can sign up to ');
  print $expire;
  ?>
  </em>
  </div>
<?php endif; ?>
