<?php
/**
 * @file The HostingFieldCleanup class.
 */

class HostingFieldCleanup {

  // The module for which we're handling cleanup.
  protected $module = FALSE;

  public function __construct($module) {
    $this->module = $module;
  }

  /**
   * Call this method when disabling a module.
   */
  public function disableModule() {
    return $this->deleteFieldInstances();
  }

  /**
   * Call this method when uninstalling a module.
   */
  public function uninstallModule() {
    return $this->deleteFieldData();
  }

  /**
   * Delete all fields instances defined in this module from their bundles.
   */
  protected function deleteFieldInstances() {
    if (module_load_include('inc', $this->module, $this->module . '.features.field_instance')) {
      $function = $this->module . '_field_default_field_instances';
      if (function_exists($function)) {
        foreach ($function() as $instance) {
          field_delete_instance($instance, FALSE);
        }
      }
    }
  }

  /**
   * Delete data for all fields defined in this module.
   */
  protected function deleteFieldData() {
    if (module_load_include('inc', $this->module, $this->module . '.features.field_base')) {
      $function = $this->module . '_field_default_field_bases';
      if (function_exists($function)) {
        $fields = array_keys($function());
        foreach ($fields as $field) {
          field_delete_field($field);
        }
      }
    }
  }

}
