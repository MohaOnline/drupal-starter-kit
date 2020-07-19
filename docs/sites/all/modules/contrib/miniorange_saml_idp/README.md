CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements/dependencies
 * Installation
 * Configuration
 * Setup Guides


INTRODUCTION
------------
Drupal SAML 2.0 IDP provides the ability to turn your Drupal site to an Identity Provider. SSO into any SP using your Drupal site as an IDP. We support all known Service Providers. If you need detailed instructions on setting up these SPs, we can give you step by step instructions.


REQUIREMENTS/DEPENDENCIES
-------------------------
NONE


INSTALLATION
------------
Follow the steps mentioned on https://www.drupal.org/project/miniorange_saml_idp to install the module.


CONFIGURATION
-------------
 * Configure user permissions in Configuration » People » miniOrange SAML IDP Configuration:

   - Setup Customer account with miniOrange (Optional)
     Login/Create account with miniOrange by entering email address, phone number and password.

   - Service Provider Setup.
     Make note of the Identity Provider information from IDP Metadata tab. This will be required to configure your SP.

   - Identity Provider Setup
     Configure the Drupal site to act as a Identity Provider(SP). Information such as SP Entity ID, ACS Url are taken from SP and stored here.


SETUP GUIDES
------------
We provide details step by step setup guide for various SPs
Please visit - https://plugins.miniorange.com/guide-enable-miniorange-drupal-saml-idp/
Note: If you dont find guide for your desired SP, please contact us at drupalsupport@xecurify.com