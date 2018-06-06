Description
-----------
This module provides API integration with Recurly.com, a hosted subscription
management service. For more information about the project, see the project page
at http://drupal.org/project/recurly.

Requirements
------------
Drupal 7.x
Libraries API

Installation
------------

- Download the Recurly PHP library from
  https://github.com/recurly/recurly-client-php and place it in
  sites/all/libraries so that "recurly.php" is located at
  sites/all/libraries/recurly/lib/recurly.php.

- If you want to enable billing forms on your site, enable the recurlyjs module.

  IMPORTANT: This version of the Recurly module is only compatible with
  Recurly.js API v4 and the Recurly v2 API.

- Log into your Recurly.com account and visit the API Credentials page.
  (developer/api_access) Copy the value for the Private API Key and subdomain
  (found in the address bar or configuration/edit) & paste them into the module
  configuration in Drupal under admin/config/services/recurly. If you are using
  recurlyjs, you will also need the Public Key.

- While on the Recurly settings page in Drupal, copy the full URL from the
  description of  the "Listener URL key" field. It should be something like:
  "https://example.com/recurly/listener/QVDtH2CR". Use an HTTPS URL if
  available. Take this full URL and paste it into the Recurly.com settings for
  "Webhook URL" - Developers > Webhooks (configuration/notifications/configure).

- After you have set up the desired plans in Recurly, on your Drupal site visit
  admin/config/services/recurly/subscription-plans and enable the desired plans.

By default, the Recurly module will provide built-in pages for managing
subscriptions on a one-subscription-per-user basis. You may also enable the
Recurly.js module *OR* the Recurly Managed Pages module to allow users to update
their billing information securely. If you're using a more complicated
subscription system, you should look into the Commerce Recurly module (not
included as part of the main Recurly package).

While this module provides the ability to create, update and cancel
subscriptions, it does *not* provide any built-in way to perform actions when
a subscription is purchased (such as granting a user a role) or perform any
actions when a subscription expires (such as blocking a user account). Currently
performing a response to an action requires custom coding through
hook_recurly_process_push_notification(). See the recurly.api.php file for more
information on that hook.

Account Codes and Recurly E-mails
---------------------------------
When creating new accounts in Recurly.com, the Drupal Recurly module will use
a pattern of [entity_type]-[entity_id] to generate Recurly account codes. That
is a Drupal user account with UID #1 would be assigned the Recurly account code
"user-1". However this mapping between the Recurly "account_code" and Drupal's
entities can be arbitrary. Any mapping desired can be imported into the
"recurly_account" database table to bind any account_code to a Drupal ID.

If using the built-in pages for managing subscriptions, a user's typical
subscription page will be located at user/[uid]/subscription. However within
Recurly.com, you only have the "account_code" to work with when configuring
e-mails. To redirect to a users subscription page on your Drupal site, you
would use a URL such as http://example.com/manage-subscription/{{account_code}}.
The Recurly module will redirect the account from the account code to
http://example.com/user/[uid]/subscription.

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/recurly?categories=All

WE DO NOT PROVIDE SUPPORT ON CUSTOM CODING. Please use the issue queue for
issues that are believed to be bugs or for requesting features!
