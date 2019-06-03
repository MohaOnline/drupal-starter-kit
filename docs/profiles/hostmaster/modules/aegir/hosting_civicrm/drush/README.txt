Hosting CiviCRM
===============

This is the front-end Aegir module to manage CiviCRM [1] in Aegir [2].
It manages the CiviCRM crons (using the hosting_civicrm_cron submodule)
and the CiviCRM API site key.

Developed, maintained and supported by Coop SymbioTIC [3], Praxis Labs Coop [4]
and Ergon Logic Enterprises [5].

[1] https://civicrm.org
[2] http://www.aegirproject.org/
[3] https://www.symbiotic.coop
[4] http://praxis.coop
[5] http://ergonlogic.com

Installation instructions
-------------------------

Hosting CiviCRM is now included in the Aegir Hosting System distribution, as
part of the "Golden contrib" initiative. As such, it is available to enable by
default as of Aegir 3.2. For older versions, follow these steps:

- Copy the module to your /var/aegir/hostmaster/sites/aegir.example.org/modules/
- Enable the module: drush @hostmaster en hosting_civicrm -y
- In Aegir, give the 'configure site CiviCRM cron intervals' permission to admins.

If you are using the Debian package for Aegir and you would like to
automate the installation of the module on new Aegir installs, you
should use a custom makefile so that the module is not lost after an upgrade:
http://community.aegirproject.org/upgrading/debian#Custom_distributions

Debugging
=========

If you are having problems running the crons, try:

  drush '@hostmaster' hosting-civicrm_cron --items=5 --debug --strict=0

License
=======

(C) 2012-2015 Christopher Gervais <https://www.drupal.org/u/ergonlogic>
(C) 2012-2015 Mathieu Lutfy <mathieu@bidon.ca>
(C) 2012-2015 Coop SymbioTIC <info@symbiotic.coop>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
