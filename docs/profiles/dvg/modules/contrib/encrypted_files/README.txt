
DESCRIPTION
===========

Say you want users of your site to be able to upload and download documents,
but you also want these documents to be protected in case someone breaks into
your server. This module allows Drupal to encrypt files that users upload and
decrypt files for download, keeping the unencrypted versions of files from
being stored on disk. It does this by creating a custom file stream wrapper that
Drupal can read from and write to and a new download method that sits alongside
the regular public and private methods. So you can make Encrypted Files the
default download method, or only use it as the download method for specific
file-type fields.

Dependencies:
  - Encrypt (http://drupal.org/project/encrypt)

Note:
Though Encrypted Files encrypts your files for storage, it does not provide
any access checking for file downloads. Rather, it simply gives each encrypted
file the same access as the node it is attached to. This allows you to leverage
the Node Access System, permissions, and other access techniques available to
Drupal to control access to encrypted files by restricting viewing access to
their nodes.

Version 2 of this module is sponsored by Townsend Security
(http://townsendsecurity.com).

INSTALLATION
============

First, download and enable the Encrypt module. Create an encryption
configuration in Encrypt's administrative interface, using whatever
encryption method and key provider you prefer.

Then download and enable Encrypted Files like any other module. For
more info on installing modules, see:
http://drupal.org/documentation/install/modules-themes/modules-7

USAGE
=====

When creating a file field, choose "Encrypted files" as the upload
destination. Select the encryption configuration to use.

When a file is added, it will be encrypted before
saving. When displaying the file, it will be decrypted.

CONTACT
=======

Current maintainers:
  - rlhawk (http://www.drupal.org/user/352283/)
  - Cellar Door (http://drupal.org/user/658076)
