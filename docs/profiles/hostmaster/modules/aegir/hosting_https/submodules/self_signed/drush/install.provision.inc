<?php

/**
 * @file
 *   Invoke Drush API hooks for the 'provision-install' command.
 */

/**
 * Implements drush_hook_provision_install().
 *
 * Create the site specific configuration for this service.
 *
 * In our selfsigned implementation we do not have an implementation
 * of this API method, because it wasn't necessary.
 *
 * If the service implemented doesn't define this method, or
 * no implementation has been selected, nothing will happen.
 */
function drush_self_signed_provision_install() {
  // only run this code on site objects.
  if (d()->type == 'site') {

    /**
     * Calling service methods.
     *
     * All the provision context objects can register which
     * servers handle specific services for them.
     *
     * To call the correct methods, you just need to use the
     * the 'service' method with the service type you want
     * to call as its only argument.
     */
    d()->service('Certificate')->create_config('site');
  }
}

/**
 * Implements drush_hook_provision_install_rollback().
 */
function drush_self_signed_provision_install_rollback() {
  d()->service('Certificate')->delete_config('site');
}

/**
 * Implements drush_hook_post_provision_install().
 *
 * We can now perform such operations as restarting the server,
 * by calling the public API of the service we have created.
 */
function drush_self_signed_post_provision_install() {
  d()->service('Certificate')->parse_configs();
}
