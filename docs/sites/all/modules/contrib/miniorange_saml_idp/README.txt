CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration

INTRODUCTION
------------
Drupal SAML 2.0 IDP provides the ability to turn your Drupal site to an Identity Provider. SSO into any SP using your Srupal site as an IDP. We support all known Service Providers. If you need detailed instructions on setting up these SPs, we can give you step by step instructions.

REQUIREMENTS
------------
Libraries module is required for this Module to work.

INSTALLATION
------------

Download Libraries project from https://www.drupal.org/project/libraries. Install and enable this module.

Download xmlseclibs from https://github.com/simplesamlphp/xmlseclibs. Extract the archive and place it under sites/all/libraries.

Install as you would normally install a contributed Drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7
for further information.

CONFIGURATION
-------------
 * Configure user permissions in Administration » People » miniOrange SAML IDP Configuration:


   - Setup Customer account with miniOrange


     Create/Login with miniOrange by entering email address, phone number and
     password.


   - Identity Provider Setup.


     Make note of the Service Provider information. This will be required to configure your IdP.
	 Configure the Drupal site to act as a Identity Provider(IDP). Information such as SP Entity ID, ACS Url are taken from SP and stored here.


   - Service Provider Setup


     Make note of the Identity Provider information. This will be required to configure your SP.

