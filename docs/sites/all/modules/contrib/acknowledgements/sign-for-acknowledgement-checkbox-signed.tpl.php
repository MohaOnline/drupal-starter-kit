<?php
/**
 * Template file
 * variables:
 * $content -> the form (disabled) to show the checkbox
 * $user -> a drupal object representing the current user
 * $expire -> the expiration date of the signature period (NULL if no expiration)
 * $date -> the signature date and time
 * $annotation -> an annotation by the user  
 */ 
?>
<div id="sign_for_acknowledgement_checkbox"><?php print $content ?></div>
<ul>
<?php if ($agreement != '') : ?>
<li><em><?php print t('Agreement: ') . $agreement ?></em></li>
<?php endif; ?>
<?php if ($annotation != '') : ?>
<li><em><?php print t('Annotation: ') . $annotation ?></em></li>
<?php endif; ?>
<li><em><?php print t('Signature date: ') . $date ?></em></li>
<?php if ($expire) : ?>
<li><em><?php print t('Should be signed within: ') . $expire ?></em></li>
<?php endif; ?>
</ul>
