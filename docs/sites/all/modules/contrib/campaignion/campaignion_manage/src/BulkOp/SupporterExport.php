<?php

namespace Drupal\campaignion_manage\BulkOp;

use Drupal\campaignion\ContactTypeManager;
use Drupal\campaignion_manage\BatchJob;

class SupporterExport implements BatchInterface {
  public function title() { return t('Export contact data'); }

  public function helpText() {
    return t('Export all currently selected supporters into a CSV file.');
  }

  private function getFields() {
    $exporter = ContactTypeManager::instance()->exporter('csv');
    return $exporter->columnOptions();
  }

  public function formElement(&$element, &$form_state) {
    $options = $this->getFields();
    $element['export'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Select one or more fields that you want to export.'),
      '#options' => $options,
      '#default_value' => array_keys($options),
    );
  }

  public function apply($resultset, $values) {
    $data['fields'] = array_filter($values['export']);
    $data['csv_name'] = tempnam(file_directory_temp(), 'CampaignionSupporterExport_' );
    $this->initBatch($data);
    $messages = array(
      'title'            => t('Export supporter data into CSV file ...'),
      'init_message'     => t('Start exporting supporter data...'),
      'progress_message' => t('Start exporting supporter data...'),
      'error_message'    => t('Encountered an error while exporting supporter data.'),
      'status_message'   => t('Exported @current out of @total supporters.'),
    );
    $job = new BatchJob($this, $resultset, $data, $messages);
    $job->set();
  }

  public function getBatch(&$data) {
    return new SupporterExportBatch($data);
  }

  /**
   * Create the temporary file and add the header lines.
   */
  protected function initBatch(&$data) {
    $exporter = ContactTypeManager::instance()->exporter('csv');
    $exporter->filterColumns($data['fields']);
    $handle = fopen($data['csv_name'], 'w');
    fputcsv($handle, $exporter->header(0));
    fputcsv($handle, $exporter->header(1));
    fclose($handle);
  }

  public function batchFinish(&$data, &$results) {
    $file_name = 'Campaignion_Supporter_Export_' . date('Y-m-d_H:i:s') . '.csv';
    drupal_add_http_header('Content-Type', 'text/csv; utf-8');
    drupal_add_http_header('Pragma', 'public');
    drupal_add_http_header('Cache-Control', 'max-age=0');
    drupal_add_http_header('Content-Disposition', "attachment; filename=$file_name");
    drupal_send_headers();
    // compress exports over 20KB
    if (filesize($data['csv_name']) > 20000) {
      if (isset($_SERVER['HTTP_ACCEPT_ENCODING']) && strpos($_SERVER['HTTP_ACCEPT_ENCODING'], 'gzip') !== FALSE) {
        if ($this->compressFile($data['csv_name'])) {
          unlink($data['csv_name']);
          $data['csv_name'] .= '.gz';
          $file_name .= '.gz';
          ini_set('zlib.output_compression', '0');
          header('Content-Encoding: gzip');
        }
        else {
          drupal_set_message(t('Error while compressing file for supporter export.'));
          return;
        }
      }
    }
    if (ob_get_level()) {
      ob_end_clean();
    }
    readfile($data['csv_name']);
    unlink($data['csv_name']);
    drupal_exit();
  }

  /**
   * GZIPs a file on disk (appending .gz to the name)
   *
   * From http://stackoverflow.com/questions/6073397/how-do-you-create-a-gz-file-using-php
   * Based on function by Kioob at:
   * http://www.php.net/manual/en/function.gzwrite.php#34955
   *
   * @param string $source Path to file that should be compressed
   * @param integer $level GZIP compression level (default: 9)
   * @return string New filename (with .gz appended) if success, or FALSE if operation fails
   */
  protected function compressFile($src_file_name, $dest_file_name = NULL, $level = 9){ 
    if ($dest_file_name == NULL) {
      $dest_file_name = $src_file_name . '.gz';
    }
    $mode = 'wb' . $level;
    $result = TRUE;
    if ($fp_out = gzopen($dest_file_name, $mode)) {
      if ($fp_in = fopen($src_file_name,'rb')) {
        while (!feof($fp_in)) {
          gzwrite($fp_out, fread($fp_in, 1024 * 512));
        }
        fclose($fp_in);
      }
      else {
        $result = FALSE; 
      }
      gzclose($fp_out); 
    } else {
      $result = FALSE;
    }

    return $result;
  }
}
