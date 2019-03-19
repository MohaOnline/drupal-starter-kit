IMCE Private Files
==================

Contents
--------

 * Introduction
 * Problem Statement
 * Requirements
 * Installation
 * Configuration
 * Troubleshooting
 * FAQ
 
Introduction
------------

IMCE (https://drupal.org/project/imce), which provides a file uploader and 
browser that can be used alone or
within a WYSIWYG editor, is one of Drupal's most popular contributed modules.
IMCE handles download permissions by showing or hiding the "browser" applet, but
does not robustly handle inline or direct download links to files if Drupal's
Private Files system is enabled.

IMCE Private Files extends IMCE by providing additional integration with 
Drupal's private file system. You can use it in one of two modes:

1. **Simple Mode:** All users with a specific role (defaults to "Authorized 
User") attempting to download an IMCE file via an inline link rather than the
IMCE browser will be allowed to do so.
2. **Pass-Through Mode:** Users who attempt to download an IMCE file via an
inline link rather than the IMCE browser will only be allowed do do so if IMCE
would allow them to browse the directory that contains the file.

Problem Statement
-----------------

With Drupal's default public file system, content creators can attach a file to
 a node, and that file is instantly available to
any anonymous user who has the direct link. The file lives at a URL like
`https://your.site/sites/default/files/MySensitiveDocument.pdf`, and anyone with
that direct URL can access the file, regardless of the access permissions of the
node to which the file was attached.

When using Drupal's private file system **without** IMCE, content creators can 
attach a file to a node, and the file will inherit the permissions of the node 
to which it is attached. I the content creater were to then add a link to that 
file inline (e.g. by entering 
`<a href="system/files/private/path/to/file.pdf">My File</a>` into a Body 
field),
either in that node or even in a different node, that link wouldn't work for
unauthorized users.

When using IMCE (with or without Drupal's private file system),
IMCE includes an access control mechanism to manage permissions within its file
browser. If you grant a user (via Roles and IMCE Profiles) the right to "browse"
a directory, then that user can download files within that directory 
through IMCE. By default, if a user has the Drupal private file system URL
to the file (e.g. `"https://your.site/system/files/private/path/to/file.pdf"`),
that user can download that file whether or not their IMCE Role/Profile 
combination would allow it. IMCE will let you shut direct downloads off
completely, or allow them from everyone.

In other words, IMCE only supports two security models when using Private Files:

1. EITHER refuse file downloads directly from the URL, and force 
everyone to use the file browser at all times,
2. OR allow all file downloads directly from the URL, regardless of what the 
 permissions on that file should be.

If you want to be able to include direct links to files inline within content or
in email blasts or some other case, but you don't want those files to become 
available anonymously, then you need this module.


Requirements
------------
This module requires:

 * IMCE

Additionally, if Drupal's Private Files are not enabled, this module has no
effect.

Installation
------------

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

Configuration
-------------

 * There are no new user permissions for IMCE Private Files. The "administer
 imce" permissions is used for configuration permission.
 
 * Choose a mode in Administration >> Configuration >> Media >> IMCE >> IMCE 
 Private Files.
 
   - **Simple Mode** will allow direct downloads to users with the selected 
  role. The role defaults to "Authenticated user"; when you choose Simple Mode,
  a selector will appear for you to choose a different role.
 
   - **Pass-Through Mode** piggy-backs on IMCE's built-in Role/Profile
  assignments. A user whose role and profile allows them to "browse" the 
  directory in which a file is contained will also be allowed to download that
  file.
  
 * **Debug mode** will write extra log messages to the watchdog table whenever
  a file is downloaded. Not recommended for production.
  
Troubleshooting
---------------

 * In Pass-through mode, if access is denied when you think it should be 
 granted, double-check the IMCE Role/Profiles Weight. Each user may only have
 one profile, even if their roles would grant them several. Use the weight to
 make sure the user gets the appropriate IMCE Profile.

FAQ
---

### Do I need this module? ###

* If you have only public files, you do not need this module.
* If you have a mixture of public and private files (or only private files),
but you force users to access the files through IMCE's browser, then you do 
not need this module - you can simply "Disable serving of private files" 
instead, on the IMCE configuration page.
* If on the other hand you have a mixture of public and private files (or only 
private files), you want files to be available by direct link, but you want 
access control over those links, then you need this module.

### How do I handle Users with Multiple Roles? ###

IMCE requires that each user have only one IMCE Profile, regardless of how many 
Roles they have. Please be aware of the "Weight" feature on the IMCE config
screen, which allows you to rank which Profile will be assigned to a user based
on the role with the highest rank (lightest weight). The IMCE Private Files 
module respects IMCE's assignments.
