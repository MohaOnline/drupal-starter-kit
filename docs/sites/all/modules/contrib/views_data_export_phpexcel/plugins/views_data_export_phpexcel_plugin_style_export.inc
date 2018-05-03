<?php

/**
 * @file
 * PHPExcel style plugin.
 */

class views_data_export_phpexcel_plugin_style_export extends views_data_export_plugin_style_export {
  /**
   * Render the display in this style.
   */
  function render() {
    if ($this->uses_row_plugin() && empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }

    $output = '';
    $output .= $this->render_body();
    return $output;
  }
}
