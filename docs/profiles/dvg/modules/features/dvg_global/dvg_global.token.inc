<?php

/**
 * Implements hook_token_info_alter().
 */
function dvg_global_token_info_alter(&$data) {
  $data['tokens']['current-page']['request-url'] = array(
    'name' => t('Request URL'),
    'description' => t('The current request URL.'),
    'type' => 'text',
  );
}

/**
 * Implements hook_tokens().
 */
function dvg_global_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'current-page') {
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'request-url':
          $scheme = isset($_SERVER['HTTPS']) ? 'https' : 'http';
          $replacements[$original] = check_url($scheme . '://' . $_SERVER['HTTP_HOST']) . request_uri();
          break;
      }
    }
  }

  return $replacements;
}
