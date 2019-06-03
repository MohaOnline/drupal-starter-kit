<?php

/**
 * Expose callback as a context condition.
 */
class context_condition_callback extends context_condition {

  protected $callback_info;

  public function __construct($plugin, $info) {
    parent::__construct($plugin, $info);

    $this->callbacks_info = module_invoke_all('context_callback_info');
  }

  function condition_values() {
    $values = array();
    if (empty($this->callbacks_info['conditions'])) {
      return $values;
    }
    foreach ($this->callbacks_info['conditions'] as $callback_id => $callback_info) {
      $values[$callback_id] = check_plain($callback_info['label']);
    }
    return $values;
  }

  /**
   * Execute the condition. This triggers all callbacks.
   */
  public function execute() {
    foreach ($this->get_contexts() as $context) {
      $callbacks = $this->fetch_from_context($context, 'values');
      foreach ($callbacks as $callback_id) {
        if (!isset($this->callbacks_info['conditions'][$callback_id])) {
          continue;
        }
        $callback = $this->callbacks_info['conditions'][$callback_id]['callback'];
        if (function_exists($callback)) {
          if (empty($this->callbacks_info['conditions'][$callback_id]['callback arguments'])) {
            $condition_met = $callback();
          }
          else {
            $callback_arguments = $this->callbacks_info['conditions'][$callback_id]['callback arguments'];
            $condition_met = call_user_func_array($callback, $callback_arguments);
          }

          if ($condition_met) {
            $this->condition_met($context);
          }
        }
      }
    }
  }
}
