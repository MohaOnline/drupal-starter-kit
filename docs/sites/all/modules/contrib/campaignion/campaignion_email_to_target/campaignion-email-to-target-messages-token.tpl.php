<?php

/**
 * @file
 * Displays all messages sent to targets.
 *
 * Available variables:
 * - $messages: An array of mails sent to targets.
 * - $submission: The submission of the email to target action.
 */
?>
<?php
$first = TRUE;
foreach ($messages as $message): ?>
<?php if (!$first): ?>
  <hr>
<?php endif; ?>
<div class="e2t-message">
<h3><?php echo t('Message to: @name with subject line “@subject”', ['@name' => $message->display, '@subject' => $message->subject]); ?></h3>
<?php echo _filter_autop($message->header); ?>
<?php echo _filter_autop($message->message); ?>
<?php echo _filter_autop($message->footer); ?>
</div>
<?php
  $first = FALSE;
endforeach;
