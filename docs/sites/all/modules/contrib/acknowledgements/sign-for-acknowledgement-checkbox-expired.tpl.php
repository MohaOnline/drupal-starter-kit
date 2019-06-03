<?php
/**
 * Template file
 * variables:
 * $content -> the main text-message
 * $user -> a drupal object representing the current user
 * $expire -> the expiration date of the signature period (NULL if no expiration)
 * $date -> the current date and time 
 */ 
?>
<div id="sign_for_acknowledgement_checkbox"><?php print $content ?></div>
<?php if ($expire) : ?>
<em><?php print t('You could sign within: ') . $expire ?></em>
<?php endif; ?>
