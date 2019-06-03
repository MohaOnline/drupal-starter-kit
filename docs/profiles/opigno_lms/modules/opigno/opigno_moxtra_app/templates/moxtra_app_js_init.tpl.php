<?php

drupal_add_html_head(
  array(
    '#tag' => 'script',
    '#type' => 'html_tag',
    '#attributes' => array(
      'src' => OPIGNO_MOXTRA_APP_MOXTRA_JS,
      'type' => 'text/javascript',
      'id' => 'moxtrajs'
    ),
    '#value' => '',
    '#weight' => 1000
  ),
  'moxtrajs'
);
?>

<script>
  Moxtra.init({
    mode: '<?php echo OPIGNO_MOXTRA_APP_MOXTRA_JS_ENV; ?>',
    client_id: '<?php echo variable_get('opigno_moxtra_app_client_id'); ?>',
    org_id: '<?php echo variable_get('opigno_moxtra_app_org_id'); ?>',
    access_token: '<?php echo opigno_moxtra_app_api_opigno_get_access_token() ?>',
    sdk_version: '5'
  });
</script>
