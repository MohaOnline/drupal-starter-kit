This module adds SMTP functionality to Drupal. It attempts to have
only the minimum required functionality to do that.


Note that for most hosting environments you do not need this
module. Hosting environments like Pantheon require use of modules like
this though.


REQUIREMENTS
------------
* Access to an SMTP server
* phpmailer installed as library.
  Get it here: https://github.com/PHPMailer/PHPMailer
* Optional: To connect to an SMTP server using SSL, you need to have the
  openssl package installed on your server, and your webserver and PHP
  installation need to have additional components installed and configured.


INSTALLATION INSTRUCTIONS
-------------------------
1. "git clone" the latest PHPMailer under sites/all/libraries.
2. Install the libraries module.
3. Login as administrator and configure the module
   (/admin/config/system/just_smtp).
   You must turn it on explicitly.


DEVELOPER FRIENDLY
------------------
The module has been tested with the reroute_email module, and both
work correctly together.

The module can also be disabled via a $conf['just_smpt_on'] = 0 setting.



COMPARABLE MODULES
------------------
Drupal has an SMTP module, but unfortunately was broken for me,
#1044534. Fixing this appeared to be very hard as the SMTP module
contains a half-baked MIME parser which I didn't want to fix. It also
simply has way too many options for what its purpose is.
