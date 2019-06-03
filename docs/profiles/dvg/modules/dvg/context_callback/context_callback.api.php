<?php

/**
 * @file
 * Hooks provided by Context Callback.
 */

/**
 * Registry of the callbacks.
 *
 * This hook is used to register your callbacks. Each one will be listed as
 * Callback conditions in the context module.
 *
 * @return array
 *   Returns an array of callbacks with names.
 */
function hook_context_callback_info() {
  return array(
    'conditions' => array(
      'mymodule_condition_callback_id' => array(
        'label' => 'MyModule Callback Example',
        'callback' => 'mymodule_condition_callback_example',
        // Optional.
        'callback arguments' => array(
          'argument 1',
          'argument 2',
        ),
      ),
    ),
    'reactions' => array(
      'mymodule_reaction_callback_id' => array(
        'label' => 'MyModule Callback Example',
        'callback' => 'mymodule_reaction_callback_example',
        // Optional.
        'callback arguments' => array(
          'argument 1',
          'argument 2',
        ),
      ),
    ),
  );
}
