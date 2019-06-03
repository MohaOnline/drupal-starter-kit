Hosting Sync
============

Lets a user sync database and files from one site to another.


Custom Alias
------------

You can also specify a custom alias, one not managed by Aegir, for a sync task.


A minimal example could be:
```php
<?php

$aliases['live.example.com'] = array(
  'remote-host' => 'server.example.com',
  'remote-user' => 'www-admin',
  'root' => '/other/path/to/drupal',
  'uri' => 'http://www.example.com',
);
```

Only the last two lines are added for compatibility with hosting_sync.
See the Drush documentation for more options.
