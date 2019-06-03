<?php

/**
 * @file
 * A Drush command that changes the database password for a site.
 */


/**
 * Implements hook_drush_command().
 */
function provision_change_site_password_drush_command() {
  $items['provision-change_site_password'] = array(
    'description' => 'Regenerates a site database password and rewrites the configuration accordingly.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  return $items;
}

/**
 * Implements the provision-change_site_password command.
 */
function drush_provision_change_site_password() {
  drush_errors_on();

  if (d()->type === 'site') {
    // There's a delay between password change and verification, so put the site
    // into maintenance mode first just to be safe.
    provision_backend_invoke(d()->name, 'vset maintenance_mode 1');
    drush_log(dt('Site has been put into maintenance mode.'));

    // Generate the new database password.
    $new_password = provision_password();

    // Retrieve the current database connection information.
    $database_config = d()->service('db')->fetch_site_credentials();
    $db_user = $database_config['db_user'];
    $db_name = $database_config['db_name'];


    // Revoke and grant database permissions for each server (same username and pw for each).
    // For single server setups, it's possible that this is only one server,
    // but it's built this way to support Pack and Cluster setups.
    foreach (d()->service('db')->grant_host_list() as $db_grant_host) {
      drush_log(dt("Revoking privileges of %user@%client from %database", array('%user' => $db_user, '%client' => $db_grant_host, '%database' => $db_name)));
      if (!d()->service('db')->revoke($db_name, $db_user, $db_grant_host)) {
        drush_log(dt("Failed to revoke user privileges"), 'warning');
      }

      drush_log(dt("Granting privileges to %user@%client on %database", array('%user' => $db_user, '%client' => $db_grant_host, '%database' => $db_name)));
      if (!d()->service('db')->grant($db_name, $db_user, $new_password, $db_grant_host)) {
        drush_set_error('PROVISION_CREATE_DB_FAILED', dt("Could not create database user @user", array('@user' => $db_user)));
      }
    }

    // save new password to our context, other code needs it to e.d. render the vhost config
    drush_set_option('db_passwd', $new_password, 'site');

    // Update the site context and trigger a verify to rewrite all related
    // Apache virtualhosts.
    $config = new Provision_Config_Drushrc_Site(d()->name);
    $config->db_passwd = $new_password;
    $config->write();

    drush_provision_verify();

    // Finally, take the site out of maintenance mode.
    provision_backend_invoke(d()->name, 'vset maintenance_mode 0');
    drush_log(dt('Site has been taken out of maintenance mode.'));
  }
}

