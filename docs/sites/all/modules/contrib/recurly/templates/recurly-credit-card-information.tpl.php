<?php
/**
 * @file
 * Prints out a summary of an existing credit card with masked digits.
 */
?>
<div class="credit-card-information">
  <div class="credit-card-name"><?php print $first_name . ' ' . $last_name; ?></div>
  <div class="credit-card-date"><?php print check_plain($card_type) . ' ' . t('Exp: @date', array('@date' => sprintf('%1$02d', $month) . '/' . $year)); ?></div>
  <div class="credit-card-number"><?php print str_repeat('x', $mask_length) . $last_four; ?></div>
</div>
