Purpose of this module
======================

webform_confirm_email is a simple addon module for the webform module.

If you define an email in webform (eg. mydomain.net/node/9999/webform/emails)
this email/these emails will be send immediately when the user clicked the
webform submit button.

This module allows you to let webform send a first email to the email address
the user specified in the webform asking him/her to click on a link (the second
webform email will not be send). When the user clicks the link from his/her
email the second webform email is send.

Using this module you can ensure that the email address the user entered is a
valid one.

Example 1: Letter writing campaign
==================================

The administrator creates a webform where a user can participate in a letter
campaign. The webform contains name and email fields for the user to enter.
Further, the administrator defines 2 emails, the first email will be send to
the email address the user provided in the webform, the 2nd will be send to the
letter writing email target.

When a user submits a webform, he/she receives a 1st email message containing a
link that should be followed to confirm the correctness of his/her email
address.

When the user uses the confirmation link, the 2nd email will be send to the
letterr writing email target.

Example 2: Online petition
==========================

The administrator creates a webform where a user can participate in an online
petition. The webform contains name and email fields for the user to enter.
Further, the administrator defines 2 emails, the first email will be send to
the email address the user provided in the webform, the 2nd will also be send
to the user with a short thank you for participating note.

When a user submits a webform, he/she receives a 1st email message containing a
link that should be followed to confirm the correctness of his/her email
address.

When the user uses the confirmation link, he/she will receive the 2n email with
the thank you note. Further his/her submission can be inserted into the list of
petition signers with valid email addresses.

Configuration
=============
You will only notice it is installed when visiting a webform emails
configuration tab. That is, if your webform is defined on a node with node ID
19, you'll find the settings by "http://mydomain.net/node/19/webform/emails".
With webform_confirm_email installed you'll see 3 email tables instead of 1,
one table for "standard emails", one for "confirmation request emails" and one
for "confirmation emails".

The "standard emails" behave just like normal webform emails, "confirmation
reques emails" are send to users asking them to click on a confirmation link
and "confirmation emails" are send only when the confirmation link was used.

The forms for changing the 3 different webform email settings (from address,
from name, to address, to name, ...) is the same as the webform email settings
form.  The only difference is in the 2nd email type, the "confirmation request
email", where you have an added entry in the "Token values" list, here you'll
find the [submission:confirm_url] token that should be used in confirmation
request emails.  This token will be expanded to the confirmation link. So as
an example the content of your "E-mail template" could look like this:

"Hello [submission:values:first_name] [submission:values:last_name],

Please visit the link below to confirm your submission:

 [submission:confirm_url]

Thank you!

Your petition team"

Installing
==========

Nothing special, if you're using drush that would be

drush dl webform_confirm_email

drush en webform_confirm_email -y

Updating
========
No updates are provided for all pre 7.x-1.0 versions.

From 7.x-1.0 onwards the will be a continues update path whith updates for
releases that change the database structure.

Todo/Plans
==========
* drupal 8 support
