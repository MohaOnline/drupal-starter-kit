Aegir Platform Git
==================

This module extends Aegir to allow platforms to be built from Git.

Installation
============

1. Install as any other Drupal module into your hostmaster site.
2. Enable by going to /admin/hosting.


Scripting
=========

Besides via the front-end you can also add a platform via the commandline.

    drush provision-save --context_type=platform --git_repository_url=http://git.drupal.org/project/drupal.git --git_reference=7.x --client_name=admin --db_server=@server_master --web_server=@server_master --root=/var/aegir/platforms/platform_example @platform_example
    drush @platform_example provision-verify
    drush @hostmaster hosting-import @platform_example

Note the Git related options in the provision-save command above.

