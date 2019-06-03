Hosting tasks extra
===================

This module extends Aegir's front-end with some additional tasks.

Supported tasks/commands are:

- Flush all caches (drush cache-clear -d)
- Rebuild registry (drush registry-rebuild -d)
- Run cron (drush core-cron -d)
- Run updates (drush updatedb -d)
- Flush the Drush cache (drush clear-cache drush -d)

Extra bundled modules:
- HTTP Basic Authentication (hosting_http_basic_auth)
- Sync (hosting_sync)

INSTALL
-------

Starting the 3.x branch it's included in the distribution by default. So in most cases you can just enable it.

In addition, for 'Rebuild registry', you will need to install an additional
Drush extension:

  http://drupal.org/project/registry_rebuild

  drush dl registry_rebuild-7 --select

To use the extra tasks as Views Bulk Operations you have to manually edit the particular view to add them.
See https://www.drupal.org/node/2715945
