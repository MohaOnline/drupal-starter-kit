Hosting site Git
================

This module extends Aegir with Git integration in a site and platform directory.

Supported commands are:

- git clone
- git pull
- git checkout


Installation
============

1. Install as any other Drupal module into your hostmaster site.
2. Enable by going to /admin/hosting.

WARNING: If you make code modifications on the server, don't forget to commit them. They will be lost otherwise.


Webhooks
=======

Provides a way for environments to stay up to date with the git repository.

Each environments can configure to Pull on Queue or Pull on URL Callback.

Pull on Queue will trigger Pull Code tasks on a regular basis using Hosting
Queues.  Pull on URL Callback provides a URL that you can add to your git host
to ping on receiving a commit.


GitHub Setup
------------

1. Visit your repos page: http://github.com/YOURNAME/YOURREPO
2. Click "Settings".
3. Click "Service Hooks".
4. Click "WebHook URLs"
5. Copy and paste your project's Git Pull Trigger URL into the URL field of the
   WebHook URLs page.
6. Click "Test Hook" to run a test, then check your hostmaster site to ensure a
   Pull Code task was triggered.


Scripting
=========

Besides via the Frontend you can also add e.g. a platform via the commandline.

      drush provision-save --context_type=platform --deploy_from_git=true --repo_url=http://git.drupal.org/project/drupal.git --git_ref=7.x --client_name=admin --db_server=@server_master --web_server=@server_master --root=/var/aegir/platforms/platform_core7x @platform_core_7x
      drush @platform_core7x provision-verify
      drush @hostmaster hosting-import @platform_core7x

Note the three Git related options in the provision-save command above.


CAVEATS
=======

In the ip addess access control list IPv6 addresses are NOT supported with a CIDR suffix.
