<?php

/**
 * Build the email based on the $message and $submission objects. This template
 * is a text-only template. Email-protest messages are text-only!
 */

?>
<?php echo $message->header; ?>
<?php echo $message->message; ?>
<?php echo $message->footer; ?>
