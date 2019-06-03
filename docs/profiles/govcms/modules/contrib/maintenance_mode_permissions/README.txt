This module enables more fine grained access to Drupal maintenance mode
permissions.

By default, Drupal code enables anyone with permission to Administer Site
Configuration to set the site maintenance mode. This permission includes such
settings as clean URLS, theme administration, etc. Sometimes you want to let
someone put the site in maintenance mode w/o being able to reconfigure all kinds
of other things.

This module provides a permission to Administer Maintenance Mode. The core
provided permission of Administer Site Configuration can still put the site into
maintenance mode. This new permission will allow someone to do the same we well.
But, this permission is only for maintenance mode and nothing else.

Warning: Make sure to pair the use of this new permission with the one for
"Use the site in maintenance mode". Otherwise the person who puts it in
maintenance mode will be logged out immediately unable to turn maintenance mode
off.