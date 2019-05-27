# File System Switch

File System Switch enables you to switch file fields between private and public.

Maintaining the security of your files can be really good in Drupal, however, sometimes you may wish to move between public to private for a file field (or a bunch of file field). This module manages that process, including moving the files and updating the file references.

## Install

* Enable the module admin/modules or drush en file_system_switch.
* Ensure that the permissions are enabled.
* Navigate to admin/config/media/file-system-switch.
* Using the module

## Using the module

* On the configure page (admin/config/media/file-system-switch), you can choose the content type you wish to use.
* Make sure that the file systems are set correctly (admin/config/media/file-system). As you are moving between public to private (or visa versa) you will need to make sure that both are configured.
* Under the operations of each file field inside that content type you will be able to move between public and private file systems:
Backup Original Table (Backup the existing file system (for if something goes wrong))
* Switch Field File System (Move from public => private or private => public).
* Update File Paths. Update the file field (this is where you need Filefield Paths to retroactively update the tables).
* You will be transported to the file field that you are using. If you wish to change to Public or Private Upload Destination then select that from the Field Settings.
* Delete Backup. Remove the backup database tables that we created at the start.

## Dependencies

* https://drupal.org/project/filefield_paths

## Contributors

* Will Hall (https://drupal.org/u/willhallonline)
* Claudiu Paulet (https://drupal.org/u/klausp)
