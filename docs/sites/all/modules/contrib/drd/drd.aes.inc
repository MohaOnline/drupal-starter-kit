<?php

/**
 * Internal callback to generate an IV for AES encryption.
 *
 * @return string
 */
function drd_aes_get_iv() {
  if (!extension_loaded('mcrypt')) {
    return '';
  }
  if (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN') {
    $randgen = MCRYPT_RAND;
    srand();
  }
  else {
    $randgen = MCRYPT_DEV_URANDOM;
  }
  $td = mcrypt_module_open('rijndael-128', '', MCRYPT_MODE_CBC, '');
  $iv = mcrypt_create_iv(mcrypt_enc_get_iv_size($td), $randgen);
  mcrypt_module_close($td);
  return $iv;
}

function drd_aes_encrypt($string, $base64encode, $key, $cipher, $iv, $implementation) {
  if (empty($string)) {
    return FALSE;
  }

  # Decode the key
  $key = base64_decode($key);

  if ($implementation == 'phpseclib') {
    _drd_aes_load_phpsec();
    $phpsec = new Crypt_AES();
    $phpsec->setKey($key);
    if (!empty($iv)) {
      $phpsec->setIV($iv);
    }
    $encrypted = $phpsec->encrypt($string);
  }
  else if ($implementation == 'mcrypt') {
    $td = mcrypt_module_open($cipher, '', MCRYPT_MODE_CBC, '');
    $ks = mcrypt_enc_get_key_size($td);
    $key = substr(sha1($key), 0, $ks);
    mcrypt_generic_init($td, $key, $iv);
    $encrypted = mcrypt_generic($td, $string);
    mcrypt_generic_deinit($td);
    mcrypt_module_close($td);
  }
  else {
    return FALSE;
  }
  if ($base64encode) {
    return base64_encode($encrypted);
  }
  else {
    return $encrypted;
  }
}

function drd_aes_decrypt($string, $base64encoded, $key, $cipher, $iv, $implementation) {
  if ($base64encoded) {
    $string = base64_decode($string);
  }
  if (empty($string)) {
    return FALSE;
  }

  # Decode the key
  $key = base64_decode($key);

  if ($implementation == 'phpseclib') {
    _drd_aes_load_phpsec();
    $phpsec = new Crypt_AES();
    $phpsec->setKey($key);
    if (!empty($iv)) {
      $phpsec->setIV($iv);
    }
    $decrypted = $phpsec->decrypt($string);
  }
  else if ($implementation == 'mcrypt') {
    $td = mcrypt_module_open($cipher, '', MCRYPT_MODE_CBC, '');
    $ks = mcrypt_enc_get_key_size($td);
    $key = substr(sha1($key), 0, $ks);
    mcrypt_generic_init($td, $key, $iv);
    $decrypted = mdecrypt_generic($td, $string);
    mcrypt_generic_deinit($td);
    mcrypt_module_close($td);
  }
  else {
    return FALSE;
  }
  return trim($decrypted);
}

function _drd_aes_load_phpsec() {
  if (module_exists('libraries') && libraries_get_path('phpseclib')) {
    $phpsec_include_path = libraries_get_path('phpseclib');
  }
  else {
    $phpsec_include_path = dirname(__FILE__) . '/phpseclib';
  }
  if (file_exists($phpsec_include_path . '/Crypt/AES.php') === FALSE) {
    return -2;
  }
  if (is_readable($phpsec_include_path . '/Crypt/AES.php') === FALSE) {
    return -3;
  }
  if (function_exists('set_include_path') == FALSE) {
    return -1;
  }
  set_include_path(get_include_path() . PATH_SEPARATOR . $phpsec_include_path);
  include_once('Crypt/AES.php');
  return TRUE;
}
