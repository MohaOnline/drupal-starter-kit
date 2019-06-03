<?php

/**
 * @file
 * Default template for commerce account operation block.
 */
?>

<ul>
  <?php if (user_access('deposit funds', $user)) { ?>

    <li><?php print l(t('Deposit funds'), 'user/funds/deposit', array('attributes' => array('class' => array('operation-link')))); ?></li>

  <?php } ?>

  <?php if (user_access('transfer funds', $user)) { ?>

    <li><?php print l(t('Transfer funds'), 'user/funds/transfer', array('attributes' => array('class' => array('operation-link')))); ?></li>

  <?php } ?>

  <?php if (user_access('create escrow payment', $user)) { ?>

    <li><?php print l(t('Create escrow payment'), 'user/funds/escrow-payments/create-escrow', array('attributes' => array('class' => array('operation-link')))); ?></li>

  <?php } ?>

  <?php if (user_access('withdraw funds', $user) && $variables['enabled_methods']) { ?>

    <li><?php print l(t('Withdraw funds'), 'user/funds/withdraw', array('attributes' => array('class' => array('operation-link')))); ?></li>

  <?php } ?>

  <?php if (user_access('create escrow payment', $user)) { ?>

    <li><?php print l(t('Manage escrow payments'), 'user/funds/escrow-payments', array('attributes' => array('class' => array('operation-link')))); ?></li>

  <?php } ?>

  <?php if (user_access('withdraw funds', $user) && $variables['enabled_methods']) { ?>

    <li><?php print l(t('Configure withdrawal methods'), 'user/funds/manage/withdrawal-methods', array('attributes' => array('class' => array('operation-link')))); ?></li>

  <?php } ?>

  <?php if (user_access('withdraw funds', $user) && $variables['enabled_methods']) { ?>

    <li><?php print l(t('View withdrawal requests'), 'user/funds/withdrawals', array('attributes' => array('class' => array('operation-link')))); ?></li>

  <?php } ?>

  <?php if (user_access('view own transactions', $user)) { ?>

    <li><?php print l(t('View all transactions'), 'user/funds/transactions', array('attributes' => array('class' => array('operation-link')))); ?></li>

  <?php } ?>
</ul>
