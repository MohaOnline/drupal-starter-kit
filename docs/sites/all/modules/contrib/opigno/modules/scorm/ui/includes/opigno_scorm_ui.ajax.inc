<?php

/**
 * @file
 * Provides the AJAX callbacks for the SCORM UI module.
 */

/**
 * AJAX callback: SCORM committed data.
 *
 * @param object $scorm
 */
function opigno_scorm_ui_ajax_commit($scorm, $sco_id = NULL) {
  $data = json_decode(file_get_contents('php://input'));

  if (!empty($_POST['data'])) {
    $data = json_decode($_POST['data']);
  }

  if (!empty($data)) {
    module_invoke_all('opigno_scorm_ui_commit', $scorm, $sco_id, $data);
    drupal_json_output(array('success' => 1));
  }
  else {
    drupal_json_output(array('error' => 1, 'message' => 'no data received'));
  }
}
