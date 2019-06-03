<?php

/**
 * Implements hook_cron
 */
function opigno_moxtra_app_cron() {
  $organization = variable_get('opigno_moxtra_app_org_id', NULL);
  $status = variable_get('opigno_moxtra_app_status', 0);
  if (!empty($organization)) {
    $info = opigno_moxtra_app_api_opigno_get_organization_info();
    // Let the Opigno instance know that it is active or not
    if (!isset($info->opigno_error_message)) {
      if ($info->active) {
        variable_set('opigno_moxtra_app_status', 1);
      }
      else {
        variable_set('opigno_moxtra_app_status', 0);
      }
    }
  }
}
