govCMS DLM
-------------------

This module adds the option for a user to set a Dissemination Limiting Marker
(DLM) appended to the end of the subject for all outgoing emails sent using
drupal_mail() on your site.


Requirements
------------

* Drupal 7


Installation and configuration
------------------------------

1. Install govCMS DLM module as you install a contributed Drupal module.
   See https://drupal.org/documentation/install/modules/themes/modules-7

3. Go to /admin/config/system/dlm to configure the module.

   Here you can set the DLM to anything from UNCLASSIFIED up to PROTECTED

Your site will now append the selected DLM to the end of all email subjects sent
using drupal_mail(). Please note that any module that sends emails not using
drupal_mail will not append the DLM to the email subject.


Further reading
---------------

* https://govcms.gov.au
* https://drupal.org/project/govcms
