<?php

/**
 * @file
 * Theme and preprocess functions.
 */

/**
 * Theme function for XLSX file using PHPExcel.
 */
function theme_views_data_export_xlsx_body(&$vars) {
  _views_data_export_header_shared_preprocess($vars);
  _views_data_export_body_shared_preprocess($vars);
  $complete_array = array_merge(array($vars['header']), $vars['themed_rows']);
  // Decode HTML entities.
  foreach ($complete_array as &$row) {
    foreach ($row as &$value) {
      $value = decode_entities($value);
    }
  }

  // Load PHPExcel library.
  $library = libraries_load('PHPExcel');

  // Create PHPExcel spreadsheet and add rows to it.
  $objPHPExcel = new PHPExcel();
  $objPHPExcel->setActiveSheetIndex(0);
  $objPHPExcel->getActiveSheet()->fromArray($complete_array);

  $excel_type = 'excel2007';
  switch ($excel_type) {
    default:
    case 'excel2007':
      $objWriter = new PHPExcel_Writer_Excel2007($objPHPExcel);
      break;
  }

  // Catch the output of the spreadsheet.
  ob_start();
  $objWriter->save('php://output');
  $excelOutput = ob_get_clean();
  return $excelOutput;
}
