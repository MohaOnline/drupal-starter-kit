<?php

/**
 * Contains OgonePaymentMethodController.
 */

/**
 * An Ogone payment method.
 */
class OgonePaymentMethodController extends PaymentMethodController {

  /**
   * The production server.
   */
  const SERVER_PRODUCTION = 0;

  /**
   * The testing server.
   */
  const SERVER_TESTING = 1;

  /**
   * The production server URL.
   */
  const SERVER_PRODUCTION_URL = 'https://secure.ogone.com/ncol/prod/orderstandard_utf8.asp';

  /**
   * The testing server URL.
   */
  const SERVER_TESTING_URL = 'https://secure.ogone.com/ncol/test/orderstandard_utf8.asp';

  public $controller_data_defaults = array(
    'ogone_payment_method' => '',
    'ogone_payment_method_brand' => '',
    'passphrase_algorithm' => '',
    'passphrase_in' => '',
    'passphrase_out' => '',
    'pspid' => '',
    'ogone_dynamic_template' => '',
    'server' => self::SERVER_PRODUCTION,
  );

  public $payment_method_configuration_form_elements_callback = 'ogone_payment_method_configuration_form_elements';

  public $title = 'Ogone';

  function __construct() {
    $currency_codes = array('AED', 'ANG', 'ARS', 'AUD', 'AWG', 'BGN', 'BRL', 'BYR', 'CAD', 'CHF', 'CNY', 'CZK', 'DKK', 'EEK', 'EGP', 'EUR', 'GBP', 'GEL', 'HKD', 'HRK', 'HUF', 'ILS', 'ISK', 'JPY', 'KRW', 'LTL', 'LVL', 'MAD', 'MXN', 'NOK', 'NZD', 'PLN', 'RON', 'RUB', 'SEK', 'SGD', 'SKK', 'THB', 'TRY', 'UAH', 'USD', 'XAF', 'XOF', 'XPF', 'ZAR');
    $this->currencies = array_fill_keys($currency_codes, array());
  }

  /**
   * Implements PaymentMethodController::validate().
   */
  function validate(Payment $payment, PaymentMethod $payment_method, $strict) {
  }

  /**
   * Implements PaymentMethodController::execute().
   */
  function execute(Payment $payment) {
    entity_save('payment', $payment);
    $_SESSION['ogone_pid'] = $payment->pid;
    drupal_goto('ogone/redirect/' . $payment->pid);
  }

  /**
   * Returns the API server URL.
   *
   * @throws InvalidArgumentException
   *
   * @param int $server
   *   self::SERVER_PRODUCTION or self::SERVER_TESTING.
   *
   * @return string
   */
  function serverURL($server) {
    $urls = array(
      $this::SERVER_PRODUCTION => $this::SERVER_PRODUCTION_URL,
      $this::SERVER_TESTING => $this::SERVER_TESTING_URL,
    );
    if (array_key_exists($server, $urls)) {
      return url($urls[$server], array(
        'external' => TRUE,
      ));
    }
    else {
      throw new InvalidArgumentException(t('Server type does not exist.'));
    }
  }

  /**
   * Returns a map of Ogone statuses to Payment statuses.
   *
   * @return array
   *   Keys are Ogone statuses, values are Payment statuses.
   */
  function statusMap() {
    // @todo Find more detailed payment status descriptions than those in the
    // parameter cookbook and update the statuses accordingly, by possibly
    // creating Ogone-specific Payment payment statuses.
    return array(
      // "Incomplete or invalid".
      0 => PAYMENT_STATUS_FAILED,
      // "Cancelled by client".
      1 => PAYMENT_STATUS_CANCELLED,
      // "Authorisation refused".
      2 => PAYMENT_STATUS_AUTHORIZATION_FAILED,
      // "Order stored".
      4 => PAYMENT_STATUS_SUCCESS,
      // "Waiting client payment".
      41 => PAYMENT_STATUS_PENDING,
      // "Authorised".
      5 => PAYMENT_STATUS_PENDING,
      // "Authorisation waiting".
      51 => PAYMENT_STATUS_PENDING,
      // "Authorisation not known".
      52 => PAYMENT_STATUS_UNKNOWN,
      // "Author. to get manually".
      59 => PAYMENT_STATUS_PENDING,
      // "Authorised and cancelled".
      6 => PAYMENT_STATUS_FAILED,
      // "Author. deletion waiting".
      61 => PAYMENT_STATUS_AUTHORIZATION_FAILED,
      // "Author. deletion uncertain".
      62 => PAYMENT_STATUS_AUTHORIZATION_FAILED,
      // "Author. deletion refused".
      63 => PAYMENT_STATUS_UNKNOWN,
      // "Payment deleted".
      7 => PAYMENT_STATUS_UNKNOWN,
      // "Payment deletion pending".
      71 => PAYMENT_STATUS_UNKNOWN,
      // "Payment deletion uncertain".
      72 => PAYMENT_STATUS_UNKNOWN,
      // "Payment deletion refused".
      73 => PAYMENT_STATUS_UNKNOWN,
      // "Payment deleted (not accepted)".
      74 => PAYMENT_STATUS_UNKNOWN,
      // "Deletion processed by merchant".
      75 => PAYMENT_STATUS_UNKNOWN,
      // "Refund".
      8 => PAYMENT_STATUS_MONEY_NOT_TRANSFERRED,
      // "Refund pending".
      81 => PAYMENT_STATUS_MONEY_NOT_TRANSFERRED,
      // "Refund uncertain".
      82 => PAYMENT_STATUS_MONEY_NOT_TRANSFERRED,
      // "Refund refused".
      83 => PAYMENT_STATUS_SUCCESS,
      // "Payment declined by the acquirer (will be debited)".
      84 => PAYMENT_STATUS_FAILED,
      // "Refund processed by merchant".
      85 => PAYMENT_STATUS_MONEY_NOT_TRANSFERRED,
      // "Payment requested".
      9 => PAYMENT_STATUS_SUCCESS,
      // "Payment processing".
      91 => PAYMENT_STATUS_PENDING,
      // "Payment uncertain".
      92 => PAYMENT_STATUS_PENDING,
      // "Payment refused".
      93 => PAYMENT_STATUS_FAILED,
      // "Payment declined by the acquirer".
      94 => PAYMENT_STATUS_FAILED,
      // "Payment processed by merchant ".
      95 => PAYMENT_STATUS_SUCCESS,
    );
  }

  /**
   * Converts an Ogone status to a Payment status.
   *
   * @param string|int $ogone_status
   *
   * @return string
   */
  function convertStatus($ogone_status) {
    $status_map = $this->statusMap();
    // The first digit is the main/parent status.
    $statuses = array($ogone_status, substr($ogone_status, 0, 1));
    foreach ($statuses as $status) {
      if (isset($status_map[$status])) {
        return $status_map[$status];
      }
    }
    return PAYMENT_STATUS_UNKNOWN;
  }

  /**
   * Maps payment methods and brands.
   *
   * @return array
   *   Keys are payment method names. Values array arrays that contain brand names.
   */
  function brandMap() {
    static $map = NULL;

    if (is_null($map)) {
      $map = array(
        '3XCB' => array(
          '3XCB',
        ),
        'CreditCard' => array(
          'AIRPLUS',
          'American Express',
          'Aurora',
          'Aurore',
          'Billy',
          'CB',
          'Cofinoga',
          'Dankort',
          'Diners Club',
          'JCB',
          'Laser',
          'MaestroUK',
          'MasterCard',
          'Solo',
          'UATP',
          'VISA',
          'BCMC',
          'Maestro',
          'NetReserve',
          'PRIVILEGE',
        ),
        'UNEUROCOM' => array(
          'UNEUROCOM',
        ),
        'Amazon Checkout' => array(
          'Amazon Checkout',
        ),
        'Belfius Direct Net' => array(
          'Belfius Direct Net',
        ),
        'cashticket' => array(
          'cashticket',
        ),
        'CBC Online' => array(
          'CBC Online',
        ),
        'CENTEA Online' => array(
          'CENTEA Online',
        ),
        'DirectEbanking' => array(
          'Sofort Uberweisung',
        ),
        'DirectEbankingAT' => array(
          'DirectEbankingAT',
        ),
        'DirectEbankingBE' => array(
          'DirectEbankingBE',
        ),
        'DirectEbankingCH' => array(
          'DirectEbankingCH',
        ),
        'DirectEbankingDE' => array(
          'DirectEbankingDE',
        ),
        'DirectEbankingFR' => array(
          'DirectEbankingFR',
        ),
        'DirectEbankingGB' => array(
          'DirectEbankingGB',
        ),
        'DirectEbankingIT' => array(
          'DirectEbankingIT',
        ),
        'DirectEbankingNL' => array(
          'DirectEbankingNL',
        ),
        'EBS_AXIS' => array(
          'EBS_AXIS',
        ),
        'EBS_BC' => array(
          'EBS_BC',
        ),
        'EBS_CB' => array(
          'EBS_CB',
        ),
        'EBS_CORP' => array(
          'EBS_CORP',
        ),
        'EBS_DC' => array(
          'EBS_DC',
        ),
        'EBS_FED' => array(
          'EBS_FED',
        ),
        'EBS_HDFC' => array(
          'EBS_HDFC',
        ),
        'EBS_HYDERABAD' => array(
          'EBS_HYDERABAD',
        ),
        'EBS_IB' => array(
          'EBS_IB',
        ),
        'EBS_ICASH' => array(
          'EBS_ICASH',
        ),
        'EBS_ICICI' => array(
          'EBS_ICICI',
        ),
        'EBS_INDIA' => array(
          'EBS_INDIA',
        ),
        'EBS_ITZ' => array(
          'EBS_ITZ',
        ),
        'EBS_JK' => array(
          'EBS_JK',
        ),
        'EBS_KARNATAKA' => array(
          'EBS_KARNATAKA',
        ),
        'EBS_KOTAK' => array(
          'EBS_KOTAK',
        ),
        'EBS_MYSORE' => array(
          'EBS_MYSORE',
        ),
        'EBS_TRAVANCORE' => array(
          'EBS_TRAVANCORE',
        ),
        'eDankort' => array(
          'eDankort',
        ),
        'EPS' => array(
          'EPS',
        ),
        'FidorPay' => array(
          'FidorPay',
        ),
        'Fortis Pay Button' => array(
          'Fortis Pay Button',
        ),
        'giropay' => array(
          'giropay',
        ),
        'iDEAL' => array(
          'iDEAL',
        ),
        'ING HomePay' => array(
          'ING HomePay',
        ),
        'KBC Online' => array(
          'KBC Online',
        ),
        'MPASS' => array(
          'MPASS',
        ),
        'paysafecard' => array(
          'paysafecard',
        ),
        'PostFinance e-finance' => array(
          'PostFinance e-finance',
        ),
        'Direct Debits AT' => array(
          'Direct Debits AT',
        ),
        'Direct Debits DE' => array(
          'Direct Debits DE',
        ),
        'Direct Debits NL' => array(
          'Direct Debits NL',
        ),
        'Acceptgiro' => array(
          'Acceptgiro',
        ),
        'Bank transfer' => array(
          'Bank transfer',
        ),
        'Bank transfer BE' => array(
          'Bank transfer BE',
        ),
        'Bank transfer DE' => array(
          'Bank transfer DE',
        ),
        'Bank transfer FR' => array(
          'Bank transfer FR',
        ),
        'Bank transfer NL' => array(
          'Bank transfer NL',
        ),
        'Installments DE' => array(
          'Installments DE',
        ),
        'Installments DK' => array(
          'Installments DK',
        ),
        'Installments FI' => array(
          'Installments FI',
        ),
        'Installments NL' => array(
          'Installments NL',
        ),
        'Installments NO' => array(
          'Installments NO',
        ),
        'Installments SE' => array(
          'Installments SE',
        ),
        'Open Invoice AT' => array(
          'Open Invoice ',
        ),
        'Open Invoice CH' => array(
          'Open Invoice CH',
        ),
        'Open Invoice DE' => array(
          'Open Invoice DE',
        ),
        'Open Invoice DK' => array(
          'Open Invoice DK',
        ),
        'Open Invoice FI' => array(
          'Open Invoice FI',
        ),
        'Open Invoice NL' => array(
          'Open Invoice NL',
        ),
        'Open Invoice NO' => array(
          'Open Invoice NO',
        ),
        'Open Invoice SE' => array(
          'Open Invoice SE',
        ),
        'Payment on Delivery' => array(
          'Payment on Delivery',
        ),
        'InterSolve' => array(
          'InterSolve',
        ),
        'PingPing' => array(
          'PingPing',
        ),
        'TUNZ' => array(
          'TUNZ',
        ),
        'cashEX' => array(
          'cashEX',
        ),
        'cashU' => array(
          'cashU',
        ),
        'cashU Direct' => array(
          'cashU Direct',
        ),
        'PAYPAL' => array(
          'PAYPAL',
        ),
        'Wallie' => array(
          'Wallie',
        ),
      );
    }

    return $map;
  }

  /**
   * Returns Ogone payment method brand options.
   *
   * @return array
   *   Keys are payment method/brand names. Values array arrays that contain brand names.
   */
  function brandOptions() {
    $map = $this->brandMap();
    $options = array();
    foreach ($map as $ogone_payment_method => $ogone_payment_method_brands) {
      foreach ($ogone_payment_method_brands as $brand) {
        $options[$ogone_payment_method . ':' . $brand] = $brand;
      }
    }
    natcasesort($options);

    return $options;
  }

  /**
   * Sets up redirect POST data.
   *
   * @param Payment $payment
   *
   * @return array
   *   Keys are POST parameter names, values are values.
   */
  function redirectData(Payment $payment) {
    $return_url = url('ogone/return', array(
      'absolute' => TRUE,
    ));
    $currency = currency_load($payment->currency_code);
    $data = array(
      'AMOUNT' => bcmul($currency->roundAmount($payment->totalAmount(TRUE)),  pow(10, $currency->getDecimals())),
      'PSPID' => $payment->method->controller_data['pspid'],
      'ORDERID' => ogone_get_orderid_from_pid($payment->pid),
      'CURRENCY' => $payment->currency_code,
      'LANGUAGE' => $this->locale(),
      'EMAIL' => user_load($payment->uid)->mail,
      'TITLE' => $payment->description,
      'ACCEPTURL' => $return_url,
      'BACKURL' => $return_url,
      'DECLINEURL' => $return_url,
      'EXCEPTIONURL' => $return_url,
      'CANCELURL' => $return_url,
      'HOMEURL' => url('<front>', array(
        'absolute' => TRUE,
      )),
      'PM' => $payment->method->controller_data['ogone_payment_method'],
      'BRAND' => $payment->method->controller_data['ogone_payment_method_brand'],
      'TP' => $payment->method->controller_data['ogone_dynamic_template'],
    );
    foreach (array_values($payment->line_items) as $i => $line_item) {
      $i++;
      $data['ITEMID' . $i] = $payment->pid . 'X' . $i;
      $data['ITEMNAME' . $i] = t($line_item->description, $line_item->description_arguments);
      $data['ITEMPRICE' . $i] = $line_item->unitAmount(TRUE);
      $data['ITEMQUANT' . $i] = $line_item->quantity;
    }
    drupal_alter('ogone_redirect_data', $data, $payment);
    $data['SHASIGN'] = $this->signIncomingData($data, $payment->method);

    return $data;
  }

  /**
   * Signs POST message data.
   *
   * @param array $data
   *   Keys are POST parameter names, values are values.
   * @param array $signature_parameters
   *   The names of the parameters that are be used to generate the signature.
   * @param string $passphrase
   * @param string $algorithm
   *
   * @return string
   *   The signature.
   */
  function signData(array $data, array $signature_parameters, $passphrase, $algorithm) {
    // Filter parameters that are not needed for the signature.
    ksort($data);
    $signature_data_string = '';
    foreach ($signature_parameters as $signature_parameter) {
      $signature_parameter_pattern = '/^' . str_replace('*XX*', '\d+?', $signature_parameter) . '$/';
      foreach ($data as $data_parameter => $value) {
        if (strlen($value) && preg_match($signature_parameter_pattern, $data_parameter)) {
          $signature_data_string .= strtoupper($data_parameter) . '=' . $value . $passphrase;
        }
      }
    }

    return strtoupper(hash($algorithm, $signature_data_string));
  }

  /**
   * Signs incoming POST message data.
   *
   * @param array $data
   *   Keys are POST parameter names, values are values.
   * @param PaymentMethod $payment_method
   *
   * @return string
   *   The signature.
   */
  function signIncomingData(array $data, PaymentMethod $payment_method) {
    return $this->signData($data, $this->signatureIncomingParameters(), $payment_method->controller_data['passphrase_in'], $payment_method->controller_data['passphrase_algorithm']);
  }

  /**
   * Signs outgoing POST message data.
   *
   * @param array $data
   *   Keys are POST parameter names, values are values.
   * @param PaymentMethod $payment_method
   *
   * @return string
   *   The signature.
   */
  function signOutgoingData(array $data, PaymentMethod $payment_method) {
    return $this->signData($data, $this->signatureOutgoingParameters(), $payment_method->controller_data['passphrase_out'], $payment_method->controller_data['passphrase_algorithm']);
  }

  /**
   * Returns the names of to be encrypted incoming parameters.
   *
   * @see https://secure.ogone.com/ncol/Ogone_e-Com-ADV_EN.pdf
   *
   * @return array
   */
  function signatureIncomingParameters() {
    return array(
      'ACCEPTANCE',
      'ACCEPTURL',
      'ADDMATCH',
      'ADDRMATCH',
      'AIACTIONNUMBER',
      'AIAGIATA',
      'AIAIRNAME',
      'AIAIRTAX',
      'AIBOOKIND*XX*',
      'AICARRIER*XX*',
      'AICHDET',
      'AICLASS*XX*',
      'AICONJTI',
      'AIDEPTCODE',
      'AIDESTCITY*XX*',
      'AIDESTCITYL*XX*',
      'AIEXTRAPASNAME*XX*',
      'AIEYCD',
      'AIFLDATE*XX*',
      'AIFLNUM*XX*',
      'AIGLNUM',
      'AIINVOICE',
      'AIIRST',
      'AIORCITY*XX*',
      'AIORCITYL*XX*',
      'AIPASNAME',
      'AIPROJNUM',
      'AISTOPOV*XX*',
      'AITIDATE',
      'AITINUM',
      'AITINUML*XX*',
      'AITYPCH',
      'AIVATAMNT',
      'AIVATAPPL',
      'ALIAS',
      'ALIASOPERATION',
      'ALIASUSAGE',
      'ALLOWCORRECTION',
      'AMOUNT',
      'AMOUNT*XX*',
      'AMOUNTHTVA',
      'AMOUNTTVA',
      'BACKURL',
      'BATCHID',
      'BGCOLOR',
      'BLVERNUM',
      'BIN',
      'BRAND',
      'BRANDVISUAL',
      'BUTTONBGCOLOR',
      'BUTTONTXTCOLOR',
      'CANCELURL',
      'CARDNO',
      'CATALOGURL',
      'CAVV_3D',
      'CAVVALGORITHM_3D',
      'CERTID',
      'CHECK_AAV',
      'CIVILITY',
      'CN',
      'COM',
      'COMPLUS',
      'CONVCCY',
      'COSTCENTER',
      'COSTCODE',
      'CREDITCODE',
      'CUID',
      'CURRENCY',
      'CVC',
      'CVCFLAG',
      'DATA',
      'DATATYPE',
      'DATEIN',
      'DATEOUT',
      'DCC_COMMPERC',
      'DCC_CONVAMOUNT',
      'DCC_CONVCCY',
      'DCC_EXCHRATE',
      'DCC_EXCHRATETS',
      'DCC_INDICATOR',
      'DCC_MARGINPERC',
      'DCC_REF',
      'DCC_SOURCE',
      'DCC_VALID',
      'DECLINEURL',
      'DEVICE',
      'DISCOUNTRATE',
      'DISPLAYMODE',
      'ECI',
      'ECI_3D',
      'ECOM_BILLTO_POSTAL_CITY',
      'ECOM_BILLTO_POSTAL_COUNTRYCODE',
      'ECOM_BILLTO_POSTAL_NAME_FIRST',
      'ECOM_BILLTO_POSTAL_NAME_LAST',
      'ECOM_BILLTO_POSTAL_POSTALCODE',
      'ECOM_BILLTO_POSTAL_STREET_LINE1',
      'ECOM_BILLTO_POSTAL_STREET_LINE2',
      'ECOM_BILLTO_POSTAL_STREET_NUMBER',
      'ECOM_CONSUMERID',
      'ECOM_CONSUMER_GENDER',
      'ECOM_CONSUMEROGID',
      'ECOM_CONSUMERORDERID',
      'ECOM_CONSUMERUSERALIAS',
      'ECOM_CONSUMERUSERPWD',
      'ECOM_CONSUMERUSERID',
      'ECOM_PAYMENT_CARD_EXPDATE_MONTH',
      'ECOM_PAYMENT_CARD_EXPDATE_YEAR',
      'ECOM_PAYMENT_CARD_NAME',
      'ECOM_PAYMENT_CARD_VERIFICATION',
      'ECOM_SHIPTO_COMPANY',
      'ECOM_SHIPTO_DOB',
      'ECOM_SHIPTO_ONLINE_EMAIL',
      'ECOM_SHIPTO_POSTAL_CITY',
      'ECOM_SHIPTO_POSTAL_COUNTRYCODE',
      'ECOM_SHIPTO_POSTAL_NAME_FIRST',
      'ECOM_SHIPTO_POSTAL_NAME_LAST',
      'ECOM_SHIPTO_POSTAL_NAME_PREFIX',
      'ECOM_SHIPTO_POSTAL_POSTALCODE',
      'ECOM_SHIPTO_POSTAL_STREET_LINE1',
      'ECOM_SHIPTO_POSTAL_STREET_LINE2',
      'ECOM_SHIPTO_POSTAL_STREET_NUMBER',
      'ECOM_SHIPTO_TELECOM_FAX_NUMBER',
      'ECOM_SHIPTO_TELECOM_PHONE_NUMBER',
      'ECOM_SHIPTO_TVA',
      'ED',
      'EMAIL',
      'EXCEPTIONURL',
      'EXCLPMLIST',
      'EXECUTIONDATE*XX*',
      'FACEXCL*XX*',
      'FACTOTAL*XX*',
      'FIRSTCALL',
      'FLAG3D',
      'FONTTYPE',
      'FORCECODE1',
      'FORCECODE2',
      'FORCECODEHASH',
      'FORCEPROCESS',
      'FORCETP',
      'GENERIC_BL',
      'GIROPAY_ACCOUNT_NUMBER',
      'GIROPAY_BLZ',
      'GIROPAY_OWNER_NAME',
      'GLOBORDERID',
      'GUID',
      'HDFONTTYPE',
      'HDTBLBGCOLOR',
      'HDTBLTXTCOLOR',
      'HEIGHTFRAME',
      'HOMEURL',
      'HTTP_ACCEPT',
      'HTTP_USER_AGENT',
      'INCLUDE_BIN',
      'INCLUDE_COUNTRIES',
      'INVDATE',
      'INVDISCOUNT',
      'INVLEVEL',
      'INVORDERID',
      'ISSUERID',
      'IST_MOBILE',
      'ITEM_COUNT',
      'ITEMATTRIBUTES*XX*',
      'ITEMCATEGORY*XX*',
      'ITEMCOMMENTS*XX*',
      'ITEMDESC*XX*',
      'ITEMDISCOUNT*XX*',
      'ITEMID*XX*',
      'ITEMNAME*XX*',
      'ITEMPRICE*XX*',
      'ITEMQUANT*XX*',
      'ITEMQUANTORIG*XX*',
      'ITEMUNITOFMEASURE*XX*',
      'ITEMVAT*XX*',
      'ITEMVATCODE*XX*',
      'ITEMWEIGHT*XX*',
      'LANGUAGE',
      'LEVEL1AUTHCPC',
      'LIDEXCL*XX*',
      'LIMITC',
      'LIENTSC',
      'RIPTUSAGE',
      'LINE_REF',
      'LINE_REF1',
      'LINE_REF2',
      'LINE_REF3',
      'LINE_REF4',
      'LINE_REF5',
      'LINE_REF6',
      'LIST_BIN',
      'LIST_COUNTRIES',
      'LOGO',
      'MAXITEMQUANT*XX*',
      'MERCHANTID',
      'MODE',
      'MTIME',
      'MVER',
      'NETAMOUNT',
      'OPERATION',
      'ORDERID',
      'ORDERSHIPCOST',
      'ORDERSHIPMETH',
      'ORDERSHIPTAX',
      'ORDERSHIPTAXCODE',
      'ORIG',
      'OR_INVORDERID',
      'OR_ORDERID',
      'OWNERADDRESS',
      'OWNERADDRESS2',
      'OWNERCTY',
      'OWNERTELNO',
      'OWNERTELNO2',
      'OWNERTOWN',
      'OWNERZIP',
      'PAIDAMOUNT',
      'PARAMPLUS',
      'PARAMVAR',
      'PAYID',
      'PAYMETHOD',
      'PM',
      'PMLIST',
      'PMLISTPMLISTTYPE',
      'PMLISTTYPE',
      'PMLISTTYPEPMLIST',
      'PMTYPE',
      'POPUP',
      'POST',
      'PSPID',
      'PSWD',
      'REF',
      'REFER',
      'REFID',
      'REFKIND',
      'REF_CUSTOMERID',
      'REF_CUSTOMERREF',
      'REGISTRED',
      'REMOTE_ADDR',
      'REQGENFIELDS',
      'RTIMEOUT',
      'RTIMEOUTREQUESTEDTIMEOUT',
      'SCORINGCLIENT',
      'SETT_BATCH',
      'SID',
      'STATUS_3D',
      'SUBSCRIPTION_ID',
      'SUB_AM',
      'SUB_AMOUNT',
      'SUB_COM',
      'SUB_COMMENT',
      'SUB_CUR',
      'SUB_ENDDATE',
      'SUB_ORDERID',
      'SUB_PERIOD_MOMENT',
      'SUB_PERIOD_MOMENT_M',
      'SUB_PERIOD_MOMENT_WW',
      'SUB_PERIOD_NUMBER',
      'SUB_PERIOD_NUMBER_D',
      'SUB_PERIOD_NUMBER_M',
      'SUB_PERIOD_NUMBER_WW',
      'SUB_PERIOD_UNIT',
      'SUB_STARTDATE',
      'SUB_STATUS',
      'TAAL',
      'TAXINCLUDED*XX*',
      'TBLBGCOLOR',
      'TBLTXTCOLOR',
      'TID',
      'TITLE',
      'TOTALAMOUNT',
      'TP',
      'TRACK2',
      'TXTBADDR2',
      'TXTCOLOR',
      'TXTOKEN',
      'TXTOKENTXTOKENPAYPAL',
      'TYPE_COUNTRY',
      'UCAF_AUTHENTICATION_DATA',
      'UCAF_PAYMENT_CARD_CVC2',
      'UCAF_PAYMENT_CARD_EXPDATE_MONTH',
      'UCAF_PAYMENT_CARD_EXPDATE_YEAR',
      'UCAF_PAYMENT_CARD_NUMBER',
      'USERID',
      'USERTYPE',
      'VERSION',
      'WBTU_MSISDN',
      'WBTU_ORDERID',
      'WEIGHTUNIT',
      'WIN3DS',
      'WITHROOT',
    );
  }

  /**
   * Returns the names of to be encrypted outgoing parameters.
   *
   * @see https://secure.ogone.com/ncol/Ogone_e-Com-ADV_EN.pdf
   *
   * @return array
   */
  function signatureOutgoingParameters() {
    return array(
      'AAVADDRESS',
      'AAVCHECK',
      'AAVZIP',
      'ACCEPTANCE',
      'ALIAS',
      'AMOUNT',
      'BIN',
      'BRAND',
      'CARDNO',
      'CCCTY',
      'CN',
      'COMPLUS',
      'CREATION_STATUS',
      'CURRENCY',
      'CVCCHECK',
      'DCC_COMMPERCENTAGE',
      'DCC_CONVAMOUNT',
      'DCC_CONVCCY',
      'DCC_EXCHRATE',
      'DCC_EXCHRATESOURCE',
      'DCC_EXCHRATETS',
      'DCC_INDICATOR',
      'DCC_MARGINPERC',
      'ENTAGE',
      'DCC_VALIDHOURS',
      'DIGESTCARDNO',
      'ECI',
      'ED',
      'ENCCARDNO',
      'FXAMOUNT',
      'FXCURRENCY',
      'IP',
      'IPCTY',
      'NBREMAILUSAGE',
      'NBRIPUSAGE',
      'NBRIPUSAGE_ALLTX',
      'NBRUSAGE',
      'NCERROR',
      'NCERRORCARDNO',
      'NCERRORCN',
      'NCERRORCVC',
      'NCERRORED',
      'ORDERID',
      'PAYID',
      'PM',
      'SCO_CATEGORY',
      'SCORING',
      'STATUS',
      'SUBBRAND',
      'SUBSC',
      'RIPTION_ID',
      'TRXDATE',
      'VC',
    );
  }

  /**
   * Finds the locale to use for Ogone communication.
   *
   * @return string
   *   A locale identifier.
   */
  function locale() {
    global $language;

    $supported_locales = $this->supportedLocales();
    $country_codes = array(variable_get('site_default_country', ''), strtoupper($language->language));
    foreach ($country_codes as $country_code) {
      $locale = $language->language . '_' . $country_code;
      if (in_array($locale, $supported_locales)) {
        return $locale;
      }
    }
    return 'en_US';
  }

  /**
   * Returns an array of supported locale identifiers.
   *
   * @return array
   */
  function supportedLocales() {
    return array(
      'ar_AR',
      'cs_CZ',
      'zh_CN',
      'da_DK',
      'nl_BE',
      'nl_NL',
      'en_GB',
      'en_US',
      'fr_FR',
      'de_DE',
      'el_GR',
      'hu_HU',
      'it_IT',
      'ja_JP',
      'no_NO',
      'pl_PL',
      'pt_PT',
      'ru_RU',
      'sk_SK',
      'es_ES',
      'se_SE',
      'tr_TR',
    );
  }

  /**
   * Gets Ogone feedback data.
   *
   * @return array
   */
  function feedbackData() {
    $data_raw = array_merge($_GET, $_POST);
    unset($data_raw['q']);
    $data = array();
    // As opposed to what the Ogone e-Commerce Advanced technical documentation
    // says, not all parameter names are all-caps, even though they should be
    // to generate a correct signature.
    foreach ($data_raw as $parameter => $value) {
      $data[strtoupper($parameter)] = $value;
    }

    return $data;
  }

  /**
   * Processes Ogone feedback data.
   *
   * @return NULL
   */
  function processFeedback(array $data, Payment $payment) {
    $payment->setStatus(new PaymentStatusItem($this->convertStatus($data['STATUS'])));
    entity_save('payment', $payment);
    module_invoke_all('ogone_feedback', $data, $payment);
    $payment->finish();
  }
}
