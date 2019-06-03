INTRODUCTION
------------

The DvG Authentication Manager module adds functionality for external
authentication. For example when a form requires DigiD authentication before it
can be (pre-)filled with personal data and send.

The module also adds an auto logout bar by default, which gives users
information about the current session and provides the possibility to extend
their session or logout immediately.

### Supported Authentication methods (Authentication Providers)
The DvG authentication module currently supports the
following authentication methods:
 * **DigiD** (SAML) replacing the deprecated dvg_digid module from
   the DvG installation profile.
 * **eHerkenning** (SAML) replacing the deprecated dvg_digid module from DvG.
 * **eIDAS** (SAML) European authentication method for non-Dutch users.
 * **Dummy provider** the Dummy provider is used for testing/demo-ing the
   Authentication Manager module only and **_should never be enabled on
   production environments!_**

### Submodules
 * **DvG Authentication Privacy** Automatically anonymize and clean up
   privacy-sensitive user data after a certain amount of time.

**Note:**
This module does not provide a way to manage these externally authenticated
users within Drupal (e.g. blocking specific users). The users are expected to
be authenticated by the (Dutch) government. All managing of these users is
assumed to be done by the authentication providers.


REQUIREMENTS
------------

This project requires the [Drupal voor Gemeenten](https://www.drupal.org/project/dvg)
installation profile.

### Modules
This module requires the following modules:

 * [X Autoload](https://www.drupal.org/project/xautoload)
 * DvG Global (/profiles/dvg/modules/features/dvg_global)
 * [Context Callback](https://www.drupal.org/project/context_callback)
   (for auto logout)

### Libraries
This module requires the following libraries:
 * SimpleSAMLphp (https://simplesamlphp.org), included in the DvG
   Installation profile.


INSTALLATION
------------

### Pre-install
>**Important!**
_Make sure that you have the latest version of DvG (>=1.12), before you install
dvg_authentication and any of it's submodules!_


Run updates to make sure the **dvg_authentication_tmp_mapping** is enabled
(if not already).
```
drush updb -y
```
The tmp mapping module maps the deprecated function calls to dvg_digid and
dvg_eherkenning to the new dvg_authentication module.

### Automatic installation with _hook_update_n()_ \(preferred method\)
You can call ```dvg_authentication_tmp_mapping_enable_dvg_authentication()```
in `sitemod.install` to automatically uninstall the deprecated modules and
enable the new modules. If dvg_digid or dvg_eherkenning is enabled, the
corresponding new providers will be enabled and existing configuration is
migrated to the new settings when running this update hook.

Example usage:
```
/**
 * Enable dvg_authentication and remove deprecated authentication methods.
 */
function yoursitemod_update_N() {
    dvg_authentication_tmp_mapping_enable_dvg_authentication();
}
```

> Note: Running this update can only be done using `drush updb`, running updates
in your site will throw an exception!

## Manual installation \(using drush\)
After DvG and DvG StUF BG are patched, you should be able to disable and
uninstall the deprecated modules without a problem:
```
drush dis dvg_digid_autologout && drush pmu dvg_digid_autologout
drush dis dvg_digid && drush pmu dvg_digid
drush dis dvg_eherkenning && drush pmu dvg_eherkenning
```

Install the DvG Authentication module and the required authentication providers
for the site.
```
drush en dvg_authentication
drush en dvg_authentication_digid
drush en dvg_authentication_privacy
```


CONFIGURATION
-------------

All configuration related to the dvg_authentication (sub)module(s) are grouped
under `/admin/config/services/dvg-authentication`.
As mentioned in the requirements, you should have configured your SAML service
provider config if you are not planning on using _only_ the dummy authentication
methods. For configuring SimpleSAMLphp see [their documentation](https://simplesamlphp.org/docs/stable/simplesamlphp-sp).

 * Permissions are handled by the implementation of
   `hook_dvg_default_permissions()` which are managed by the dvg_global module
   from [DvG](https://www.drupal.org/project/dvg). These permissions should not
   need further tweaking, but when changing them you should do so using the
   `hook_dvg_default_permissions_alter()` provided by dvg_global.
   The permissions are designed to only allow users to prove their identity or
   to allow staff to service the users using only the minimum
   required permissions.
 * DigiD services (when enabling the dvg_authentication_digid module) can be
   configured at `/admin/config/services/dvg-authentication/digid`. Where you
   can provide the DigiD logo. Due to legal reasons this logo can't be shipped
   with this module. If you have SAML sources configured you can select the
   matching source here, if not you can select the `Dummy` source for each
   level you want available.
 * eHerkenning services (when enabling the dvg_authentication_eherkenning
   module) can be configured at
   `/admin/config/services/dvg-authentication/eherkenning`.
   Here you can specify which eHerkenning version you need
   (1.7, 1.9 or 1.11 currently supported). Each level you want available should
   have a source selected. Again provide a logo for eHerkenning as well as
   providing a logo for each level. These are required by the eHerkenning
   specification, but aren't allowed to be packaged with the module.
 * eIDAS service (when enabling the dvg_authentication_eidas module) can be
   configured at `/admin/config/services/dvg-authentication/eidas`.
   Only version 1.11 is currently supported. Each level you want available
   should have a source selected.
 * Dummy settings can be configured at
   `/admin/config/services/dvg-authentication/dummy`.
   These settings allow you to provide prefill data for tokens (when using
   dvg_stuf_bg_tokens from [dvg_stuf_bg](https://drupal.org/project/dvg_stuf_bg),
   also included in DvG).
 * The dvg_authentication_privacy module allows configuring when user sensitive
   data should be anonymized or removed. The configuration can be found at
   `/admin/config/services/dvg-authentication/privacy`.
 * The dvg_authentication_service_catalog module does not provide configuration,
   but does have an administration page at
   `/admin/config/services/dvg-authentication/service-catalog`.
   This module aides in generating a service catalog as required for a proper
   SAML setup for eHerkenning and eIDAS.
 * The dvg_authentication_servicedesk module does not have any configuration.
   When a user has the `servicedesk` role they have access to the
   servicedesk features.
