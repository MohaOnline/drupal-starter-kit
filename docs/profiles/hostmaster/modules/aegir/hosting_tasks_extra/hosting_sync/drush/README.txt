
Provision Sync
==============

Allows you to sync content, configuration and files between Drupal
sites hosted by Aegir.

This module provides the backend commands needed by Drush.

Installation
------------

To install provision_sync, simply use drush:

  $ drush dl provision_sync

Or, you can download the source code to any available drush commands
directory, such as your ~/.drush/commands or the system-wide
/usr/drush/share/commands directory.

Commands
--------

  $ drush @project_NAME provision-sync SOURCE_ENVIRONMENT DESTINATION_ENVIRONMENT

This task makes it easy to syncronize the database and filesdown from other
environments within the project.

WARNING: This will DESTROY the destination site's database!

This task:
  - Drops the @destination database.
  - Creates an SQL dump from @source.
  - Copies the SQL dump to the local system (if @source is a remote).
  - Imports the SQL dump into @destination database.
  - (optionally) Runs update.php.
  - (optionally) Runs features-revert-all.
  - (optionally) Clears all caches.

