<?php

/**
 * @file
 * Sample hook definitions and invokes.
 *
 * The definitions would normally be in an api.php file and the invokes in
 * regular code, but this is just for testing.
 */

/**
 * Regular hook definition.
 */
function hook_foo() {
  $x = 'one';
}

/**
 * Field hook definition.
 */
function hook_field_boo() {
  $y = 'one';
}

/**
 * Field hook with same name.
 */
function hook_field_foo() {
  $y = 'one';
}

/**
 * Entity hook definition.
 */
function hook_entity_coo() {
  $z = 'one';
}

/**
 * Entity hook with same name.
 */
function hook_entity_foo() {
  $y = 'one';
}

/**
 * Node hook definition.
 */
function hook_node_doo() {
  $n = 'one';
}

/**
 * User hook definition.
 */
function hook_user_moo() {
  $m = 'one';
}

/**
 * User hook with same name.
 */
function hook_user_foo() {
  $y = 'one';
}

/**
 * Alter hook definition.
 */
function hook_noo_alter() {
  $q = 'one';
}

/**
 * Alter hook with same name.
 */
function hook_foo_alter() {
  $y = 'one';
}

/**
 * Regular hook invoke with bootstrap.
 */
function regular_invoke_bootstrap() {
  bootstrap_invoke_all('foo');
}

/**
 * Regular hook invoke with getImplementations.
 */
function regular_invoke_get_implementations() {
  $x = $moduleHandler->getImplementations('foo');
}

/**
 * Regular hook invoke with implementsHook.
 */
function regular_invoke_implements_hook() {
  $x = $moduleHandler->implementsHook('foo');
}

/**
 * Regular hook invoke with invoke.
 */
function regular_invoke_invoke() {
  $moduleHandler->invoke($module, 'foo');
}

/**
 * Regular hook invoke with invokeAll.
 */
function regular_invoke_invoke_all() {
  $moduleHandler->invokeAll('foo');
}

/**
 * Regular hook invoke with module_hook.
 */
function regular_invoke_module_hook() {
  module_hook($module, 'foo');
}

/**
 * Regular hook invoke with module_implements.
 */
function regular_invoke_module_implements() {
  $x = module_implements('foo');
}

/**
 * Regular hook invoke with module_invoke.
 */
function regular_invoke_module_invoke() {
  module_invoke($module, 'foo');
}

/**
 * Regular hook invoke with module_invoke_all.
 */
function regular_invoke_module_invoke_all() {
  module_invoke_all('foo');
}

/**
 * Regular hook invoke with node_invoke.
 */
function regular_invoke_node_invoke() {
  node_invoke('foo');
}

/**
 * Field hook invoke with first functions.
 */
function field_invoke_one() {
  _field_invoke('foo');
  _field_invoke_default('boo');
}

/**
 * Field hook invoke with second functions.
 */
function field_invoke_two() {
  _field_invoke_multiple('foo');
  _field_invoke_multiple_default('boo');
}

/**
 * Entity hook invoke.
 */
function entity_invoke() {
  $entityHandler->invokeHook('coo');
  $entityHandler->invokeHook('foo');
  $entityHandler->invokeHook('doo');
}

/**
 * User hook invoke.
 */
function user_invoke() {
  user_module_invoke('moo');
  user_module_invoke('foo');
}

/**
 * Alter hook invoke with first functions.
 */
function alter_invoke_one() {
  drupal_alter('noo');
  $module_handler->alter('foo');
}

/**
 * Alter hook invoke with second functions.
 */
function alter_invoke_two() {
  $module_handler->alterInfo('noo');

  // Use a service with a chained method.
  $this->get('container.trait')->callMethod();
}

/**
 * Test for function call detection in control structures.
 *
 * This function calls various of the functions above, in various spots in
 * various control structures, in order to verify that the function call
 * references are being detected properly. See issue
 * https://www.drupal.org/project/api/issues/3059981 for some examples of
 * where reference detection failed.
 *
 * Note that the code doesn't make sense. We don't call hook functions. But
 * this is just for testing the syntax parsing.
 */
function control_structures() {
  if (hook_foo() || user_invoke()) {
    hook_field_boo();
  }
  elseif (hook_field_foo() && alter_invoke_two()) {
    hook_entity_coo();
  }
  else {
    hook_entity_foo();
  }

  $foo = (hook_node_doo()) ? hook_user_moo() : hook_user_foo();

  while(!hook_noo_alter()) {
    hook_foo_alter();
  }

  do {
    regular_invoke_bootstrap();
  }
  while (regular_invoke_get_implementations());

  for (regular_invoke_implements_hook(); regular_invoke_invoke(); regular_invoke_invoke_all()) {
    regular_invoke_module_hook();
  }

  foreach (regular_invoke_module_implements() as $value) {
    regular_invoke_module_invoke();
  }

  switch (regular_invoke_module_invoke_all()) {
    case 0:
      regular_invoke_node_invoke();
      break;

    default:
      field_invoke_one();
  }

  $foo = field_invoke_two(alter_invoke_one());

  return entity_invoke();
}
