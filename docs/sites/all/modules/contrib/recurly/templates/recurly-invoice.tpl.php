<?php
/**
 * @file
 * Print out an individual invoice. Typically displayed under
 * user/x/subscriptions/invoice/[invoice-uuid]
 */
drupal_add_css(drupal_get_path('module', 'recurly') . '/css/recurly-invoice.css');
?>

<?php if ($error_message): ?>
  <?php print $error_message; ?>
<?php endif; ?>

<div class="invoice">
  <div class="invoice-pdf"><?php print l(t('View PDF'), $pdf_path); ?></div>
  <?php if (isset($error_message)): ?>
    <div class="messages error"><?php print $error_message; ?></div>
  <?php endif; ?>
  <div class="invoice-date"><?php print $invoice_date; ?></div>

  <?php if ($billing_info): ?>
  <div class="bill-to">
    <b><?php print $first_name; ?> <?php print $last_name; ?></b><br />
    <?php print $address1; ?><br />
    <?php if ($address2): ?>
      <?php print $address2; ?><br />
    <?php endif; ?>
    <?php print $city; ?>, <?php print $state; ?> <?php print $zip; ?><br />
    <?php print $country; ?>
  </div>
  <?php endif; ?>
  <div class="invoice-line-items clearfix">
    <h2><?php print t('Services'); ?></h2>
    <table class="line-items grid">
      <thead>
        <tr>
          <th scope="col"><?php print t('Date'); ?></th>
          <th scope="col"><?php print t('Description'); ?></th>
          <th class="right" scope="col"><?php print t('Subtotal'); ?></th>
        </tr>
      </thead>
      <tbody>
        <?php foreach ($line_items as $line_item): ?>
        <tr>
          <td><?php print $line_item['start_date']; ?>
          <?php if ($line_item['end_date']): ?>- <?php print $line_item['end_date']; ?><?php endif; ?></td>
          <td><?php print $line_item['description']; ?></td>
          <td class="right"><?php print $line_item['amount']; ?></td>
        </tr>
        <?php endforeach; ?>
      </tbody>
    </table>
    <table class="invoice-calculations">
      <tbody>
        <tr class="invoice-subtotal">
          <th scope="row"><?php print t('Subtotal'); ?>:</th>
          <td class="right"><?php print $subtotal; ?></td>
        </tr>
        <tr class="invoice-total">
          <th scope="row"><?php print t('Total'); ?>:</th>
          <td class="right"><?php print $total; ?></td>
        </tr>
        <tr class="paid">
          <th scope="row"><?php print t('Paid'); ?></th>
          <td class="right"><?php print $paid; ?></td>
        </tr>
        <tr class="invoice-toal">
          <th scope="row"><?php print t('Total Due'); ?>:</th>
          <td class="right"><b><?php print $due; ?></b></td>
        </tr>
      </tbody>
    </table>
  </div>
  <div class="invoice-payments clearfix">
    <h2><?php print t('Payments'); ?></h2>
    <table class="payments grid">
      <tbody>
        <tr>
          <th class="item-date"><?php print t('Date'); ?></th>
          <th class="item-description"><?php print t('Payment Description'); ?></th>
          <th class="line-total"><?php print t('Amount'); ?></th>
        </tr>
        <?php foreach ($transactions as $transaction): ?>
        <tr>
          <td class="item-date"><?php print $transaction['date']; ?></td>
          <td class="item-description"><?php print $transaction['description']; ?></td>
          <td class="line-total"><?php print $transaction['amount']; ?></td>
        </tr>
        <?php endforeach; ?>
        <tr>
          <td>&nbsp;</td>
          <td class="item-description"><b><?php print t('Payment Total'); ?></b></td>
          <td class="line-total"><b><?php print $transactions_total; ?></b></td>
        </tr>
      </tbody>
    </table>
  </div>
</div>
