<?php

/**
 * @file
 * Provides special token for commerce funds.
 *
 * @see httx://drupal.org/node/1308488
 */

/**
 * Provide information about our custom placeholder/token.
 *
 * @see httx://api.drupal.org/api/drupal/modules--system--system.api.php/function/hook_token_info/7
 * @see httx://api.lullabot.com/token_example_token_info/7
 *
 * @return array
 *   An associative array of available tokens and token types.
 */
function commerce_funds_token_info() {
  $info['tokens']['transaction']['date'] = array(
    'name' => t('Date'),
    'description' => t('Date when the transaction was issued.'),
  );
  $info['tokens']['transaction']['brut-amount'] = array(
    'name' => t('Brut amount'),
    'description' => t('Brut amount of the transaction.'),
  );
  $info['tokens']['transaction']['commission'] = array(
    'name' => t('Commission fees'),
    'description' => t('The commission taken on the transaction.'),
  );
  $info['tokens']['transaction']['net-amount'] = array(
    'name' => t('Net amount'),
    'description' => t('Net amount of the transaction.'),
  );
  $info['tokens']['transaction']['type'] = array(
    'name' => t('Type'),
    'description' => t('The type of the transaction.'),
  );
  $info['tokens']['transaction']['status'] = array(
    'name' => t('Status'),
    'description' => t('The status of the transaction.'),
  );
  $info['tokens']['transaction']['notes'] = array(
    'name' => t('Notes'),
    'description' => t('The notes left by the issuer.'),
  );
  $info['tokens']['withdrawal_request']['date'] = array(
    'name' => t('Date'),
    'description' => t('Date when the withdrawal was issued.'),
  );
  $info['tokens']['withdrawal_request']['method'] = array(
    'name' => t('Method'),
    'description' => t('Payment method requested by the issuer.'),
  );
  $info['tokens']['withdrawal_request']['brut-amount'] = array(
    'name' => t('Brut amount'),
    'description' => t('Brut amount of the withdrawal.'),
  );
  $info['tokens']['withdrawal_request']['commission'] = array(
    'name' => t('Commission fees'),
    'description' => t('The commission taken on the withdrawal.'),
  );
  $info['tokens']['withdrawal_request']['net-amount'] = array(
    'name' => t('Net amount'),
    'description' => t('Net amount of the withdrawal.'),
  );
  $info['tokens']['withdrawal_request']['status'] = array(
    'name' => t('Status'),
    'description' => t('The status of the withdrawal.'),
  );
  $info['tokens']['withdrawal_request']['reason'] = array(
    'name' => t('Declined reason'),
    'description' => t('The reason why the withdrawal has been declined.'),
  );
  return $info;
}

/**
 * Provide replacement values for placeholder tokens.
 *
 * @param string $type
 *   The machine-readable name of the type (group) of token being replaced, such
 *   as 'node', 'user', or another type defined by a hook_token_info()
 *   implementation.
 * @param array $tokens
 *   An array of tokens to be replaced. The keys are the machine-readable token
 *   names, and the values are the raw [type:token] strings that appeared in the
 *   original text.
 * @param array $data
 *   An associative array of data objects
 *   to be used when generating replacement values,
 *   as supplied in the $data parameter to token_replace().
 *   (optional)
 * @param array $options
 *   An associative array of options for token replacement; see token_replace()
 *   for possible values.
 *   (optional)
 *
 * @see httx://api.drupal.org/api/drupal/modules--system--system.api.php/function/hook_tokens/7
 * @see httx://api.lullabot.com/token_example_tokens/7
 *
 * @return array
 *   An associative array of replacement values, keyed by the raw [type:token]
 *   strings from the original text.
 */
function commerce_funds_tokens($type, array $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  if ($type == 'transaction' && !empty($data['transaction'])) {
    $transaction = $data['transaction'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'date':
          $replacements[$original] = str_replace(
           $original,
           format_date($transaction->created, $type = 'medium'),
           $original
             );
          break;

        case 'brut-amount':
          $replacements[$original] = str_replace(
           $original,
           commerce_currency_format($transaction->brut_amount, commerce_default_currency()),
           $original
             );
          break;

        case 'commission':
          $replacements[$original] = str_replace(
           $original,
           commerce_currency_format($transaction->commission, commerce_default_currency()),
           $original
             );
          break;

        case 'net-amount':
          $replacements[$original] = str_replace(
           $original,
           commerce_currency_format($transaction->net_amount, commerce_default_currency()),
           $original
             );
          break;

        case 'type':
          $replacements[$original] = str_replace(
           $original,
           check_plain($transaction->type),
           $original
             );
          break;

        case 'status':
          $replacements[$original] = str_replace(
           $original,
           check_plain($transaction->status),
           $original
             );
          break;

        case 'notes':
          $replacements[$original] = str_replace(
           $original,
           check_plain($transaction->notes),
           $original
             );
          break;
      }
    }
  }

  if ($type == 'withdrawal_request' && !empty($data['withdrawal_request'])) {
    $request = $data['withdrawal_request'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'date':
          $replacements[$original] = str_replace(
           $original,
           format_date($request->created, $type = 'medium'),
           $original
             );
          break;

        case 'brut-amount':
          $replacements[$original] = str_replace(
           $original,
           commerce_currency_format($request->brut_amount, commerce_default_currency()),
           $original
             );
          break;

        case 'commission':
          $replacements[$original] = str_replace(
           $original,
           commerce_currency_format($request->commission, commerce_default_currency()),
           $original
             );
          break;

        case 'net-amount':
          $replacements[$original] = str_replace(
           $original,
           commerce_currency_format($request->net_amount, commerce_default_currency()),
           $original
             );
          break;

        case 'method':
          $replacements[$original] = str_replace(
           $original,
           check_plain($request->method),
           $original
             );
          break;

        case 'status':
          $replacements[$original] = str_replace(
           $original,
           check_plain($request->status),
           $original
             );
          break;

        case 'reason':
          $replacements[$original] = str_replace(
           $original,
           check_plain($request->reason),
           $original
             );
          break;
      }
    }
  }

  return $replacements;
}
