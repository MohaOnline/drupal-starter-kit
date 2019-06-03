Context Callback


Installation
------------
Context Callback can be installed like any other Drupal module: place it in
the modules directory for your site and enable it on the `admin/modules` page.


Hooks
-----
See `context_callback.api.php` for the available hooks.


Usage
-----
Implement the `hook_context_callback_info` hook in your own module. With this
hook, you let the Context Callback know of your callback functions.
Each callback will be called upon page-view to see whether or not it should
trigger the context.


Example code
------------
  // mymodule.module
  /**
   * Implements hook_context_callback_info().
   */
  function mymodule_context_callback_info() {
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

  /**
   * Condition callback example function.
   */
  function mymodule_condition_callback_example($argument_1, $argument_2) {
    if (/* Do some checks to see if the context should trigger */) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Reaction callback example function.
   */
  function mymodule_reaction_callback_example($argument_1, $argument_2) {
    // Do some awesome stuff!
  }
