FIX OWNERSHIP
=============

This extension will help Aegir to fix file ownership for Drupal platforms and
sites.

A full discussion on proper file ownership and permissions in Drupal can be
found at: https://www.drupal.org/node/244924

## Install Script

On Debian and Ubuntu systems, run `sudo bash scripts/install.sh` in order to
deploy these scripts, and set up the proper sudoers entry. Other Unixes may
require slightly different steps to add the sudoers entry.

## Standalone Install Script for both Fix Ownership and Fix Permissions

There is a now an install script that can be run by itself (without the rest of the module code) to install all of the Fix Permissions and Fix Ownership scripts.

This is useful for automated installation such as in the [Dockerfiles](https://github.com/aegir-project/dockerfiles).

To use the standalone install script:

       wget http://cgit.drupalcode.org/hosting_tasks_extra/plain/fix_permissions/scripts/standalone-install-fix-permissions-ownership.sh
       sudo bash standalone-install-fix-permissions-ownership.sh
