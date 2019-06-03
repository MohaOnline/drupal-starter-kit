<?php

/**
 * @file
 * Default template for commerce funds balance.
 */
?>

<?php if ($variables['balance']) { ?>

  <span><strong>Balance: </strong><?php print commerce_currency_format($variables['balance'], commerce_default_currency()); ?></span>

<?php } else { ?>

  <span><strong>Balance: </strong><?php print commerce_currency_format(0, commerce_default_currency()); ?></span>

<?php } ?>
