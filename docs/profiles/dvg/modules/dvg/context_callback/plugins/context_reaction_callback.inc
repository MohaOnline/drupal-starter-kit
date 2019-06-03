<?php

/**
 * Expose callback as context reactions.
 */
class context_reaction_callback extends context_reaction {

  protected $callback_info;

  public function __construct($plugin, $info) {
    parent::__construct($plugin, $info);

    $this->callbacks_info = module_invoke_all('context_callback_info');
  }

  function options_form($context) {
    return array(
      '#title' => $this->title,
      '#description' => $this->description,
      '#options' => $this->reaction_callbacks(),
      '#type' => 'checkboxes',
      '#default_value' => $this->fetch_from_context($context),
    );
  }

  /**
   * Retrieves all registered reaction-callbacks.
   */
  function reaction_callbacks() {
    $values = array();
    if (empty($this->callbacks_info['reactions'])) {
      return $values;
    }
    foreach ($this->callbacks_info['reactions'] as $callback_id => $callback_info) {
      $values[$callback_id] = check_plain($callback_info['label']);
    }
    return $values;
  }

  /**
   * Execute the condition. This triggers all callbacks.
   */
  function execute(&$variables = NULL) {
    foreach ($this->get_active_callbacks() as $callbacks) {
      foreach ($callbacks as $callback_id) {
        if (!isset($this->callbacks_info['reactions'][$callback_id])) {
          continue;
        }
        $callback = $this->callbacks_info['reactions'][$callback_id]['callback'];
        if (function_exists($callback)) {
          $callback_arguments = isset($this->callbacks_info['reactions'][$callback_id]['callback arguments']) ? $this->callbacks_info['reactions'][$callback_id]['callback arguments'] : array();
          if (empty($callback_arguments)) {
            $callback();
          }
          else {
            call_user_func_array($callback, $callback_arguments);
          }
        }
      }
    }
  }

  /**
   * Retrieves all reaction-callbacks of the active context.
   */
  function get_active_callbacks() {
    $active_callbacks = array();
    foreach ($this->get_contexts() as $context) {
      if (isset($context->reactions[$this->plugin])) {
        $active_callbacks[] = $context->reactions[$this->plugin];
      }
    }
    return $active_callbacks;
  }
}
