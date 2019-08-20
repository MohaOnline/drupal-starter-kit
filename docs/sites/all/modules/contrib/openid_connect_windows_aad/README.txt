OPENID CONNECT WINDOWS AAD
==========================

This small module is a CTools plugin for the great module OpenID Connect and
focuses on integration with Windows Azure AD.

Basically, Windows Azure AD connection can be achieved by using the Generic
client in OpenID Connect. Unfortunately, Windows Azure does not support the use
of the regular JWT access tokens. When we want to retrieve the UserInfo (email
address, name), we need to do a separate request and map the results on existing
fields for the user.

This module uses the access token to do this second request to Windows Azure
AD, resulting in the UserInfo data. It will also check if an email address is
part of the UserInfo data. In case no email is there, it will still create the
user, but use the username instead, providing a notice to prompt the user to
change it in his/her user settings.

Setup
-----

* Install this module.
* Visit the OpenID Connect config page: admin/config/services/openid-connect.
* Windows Azure AD will be available as a client.

Requirements
------------

* Drupal OpenID Connect module
* Windows Azure Active Directory endpoints from your registered application

External sources
----------------

* http://stackoverflow.com/questions/28631635/
* https://www.drupal.org/node/2682135
