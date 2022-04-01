<?php

function drd_migrate_d8() {
  $inventory = array();
  foreach (drd_domain_load_all() as $did => $name) {
    $domain = drd_domain_load($did);
    $token = base64_encode(drupal_get_token(((string) REQUEST_TIME) . $domain->url));

    $php = _drd_migrate_d8_php($token);
    drd_action_remote('drd.php', 'domain', $domain->did, $php);

    $inventory[$domain->sid][$domain->did] = array(
      'url' => $domain->url,
      'ssl' => $domain->usessl,
      'token' => $token,
    );
  }

  drupal_json_output($inventory);
  exit;
}

function _drd_migrate_d8_php($token) {
  return <<<EOF
<?php
  if (!module_exists('drd_remote')) {
    module_enable(array('drd_remote'));
    menu_rebuild();
  }
  variable_set('drd_remote_ott', array(
    'token' => '$token',
    'expires' => REQUEST_TIME + 1800,
  ));
?>
EOF;
}
