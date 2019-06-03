dvg_authentication 1.0
-----------------------
- First public release!
- Moved dvg_authentication module to git root.
- **dvg_authentication** Fixed DigiD idp status permanently complaining even
  though it is working.
- Add module **dvg_authentication_tmp_mapping** to aid migrating to
  dvg_authentication.

DvGCoop 0.20.10
-----------------------
- **dvg_authentication** Trim H2 to prevent empty H2 tags on the page.

DvGCoop 0.20.9
-----------------------
- **dvg_authentication_servicedesk** Add the servicedesk role to
  the dvg_stuf_bg_allowed_prefill_roles.

DvGCoop 0.20.8
-----------------------
- **dvg_authentication_privacy** Remove ip address from webform submissions and
  watchdog logs.

DvGCoop 0.20.7
-----------------------
- **dvg_authentication** Add clean handling of SAML errors.

DvGCoop 0.20.6
-----------------------
- **dvg_authentication_digid** Correct translation for the DigiD error message
  on Saml errors.

DvGCoop 0.20.5
-----------------------
- **dvg_authentication** Expose provider getLogo() function for use in
  other places.

DvGCoop 0.20.4
-----------------------
- **dvg_authentication** Changed two dutch translations "je" to "u".

DvGCoop 0.20.3
-----------------------
- **dvg_authentication** Improve webform hidden component description and fix
  dvg status error levels.

DvGCoop 0.20.2
-----------------------
- **dvg_authentication_servicedesk** Give super editor permission to assign
  the servicedesk role.

DvGCoop 0.20.1
-----------------------
- **dvg_authentication_service_catalog** Bugfix for the access callback of
  the SAML metadata download route.

DvGCoop 0.20.0
-----------------------
- **dvg_authentication_service_catalog** Add quick download links for the SAML
  metadata files of te configured services.

DvGCoop 0.19.0
-----------------------
- **dvg_authentication_auto_logout** Provided a way to start a new browser tab
  when being logged out of the identity provider. This can be optionally
  build-in by the authentication provider.

DvGCoop 0.18.3
-----------------------
- **dvg_authentication** Fix allow saving webform settings without external
  authentication.

DvGCoop 0.18.2
-----------------------
- **dvg_authentication** Fix level logos not marked as permanent and when
  required not being enforced.

DvGCoop 0.18.1
-----------------------
- **dvg_authentication** Better check for status.
- **dvg_authentication_digid** Added $levels with the method getSimpleSaml in
  the digid authentication provider.

DvGCoop 0.18.0
-----------------------
- **dvg_authentication_service_catalog** Add RequestedAttributes and the url of
  the privacy statement page to the service catalog for eIDAS services.
- **dvg_authentication** Fix: only render one login button for system forms when
  authentication is required without a required level.
- **dvg_authentication_digid** Fix: typo in the translatable string for the
  button description.

DvGCoop 0.17.2
-----------------------
- **dvg_authentication_auto_logout** Fix: CSS add space-between flexbox styling.

DvGCoop 0.17.1
-----------------------
- **dvg_authentication_digid** Fix migrating the DigiD auth source on install.

DvGCoop 0.17.0
-----------------------
- **dvg_authentication** DvG Authentication is now compatible with domains.

**Important** _Add the following forms to the Domain-specific settins - Forms
in /admin/structure/domain/settings and recreate the **SITE_domain_settings**
feature:_
 - dvg_authentication_provider_configuration_form
 - dvg_authentication_auto_logout_settings_form


DvGCoop 0.16.2
-----------------------
- **dvg_authentication_auto_logout** Fix auto logout bar countdown timer.

DvGCoop 0.16.1
-----------------------
- **dvg_authentication_service_catalog** Fix service catalog headers.
- **dvg_authentication** Give login button stylesheet a unique name.

DvGCoop 0.16.0
-----------------------
- **dvg_authentication** Changed login authentication overview to make it more
  accessible and responsive.

DvGCoop 0.15.0
-----------------------
- **dvg_authentication_service_catalog** Added service certificate to catalog
  for eIDAS. Renamed module from catalogue to catalog.

DvGCoop 0.14.1
-----------------------
- **dvg_authentication_digid** Update translations. Make logo's optional and
  enabled on dummy and eHerkenning.

DvGCoop 0.14.0
-----------------------
- **dvg_authentication_digid** Add levels Basic, Middle and Substantial
  to DigiD.

DvGCoop 0.13.0
-----------------------
- **dvg_authentication_service_catalogue** Add service catalogue module for
  easy generation of the "Diensten catalogus".

DvGCoop 0.12.1
-----------------------
- **dvg_authentication_tokens** Rename _kvk_vestigings_number_ to
  _kvk_department_number_ for consistent use of the English language.
- **dvg_authentication_eherkenning** Add missing translations.

DvGCoop 0.12.0
-----------------------
- **dvg_authentication_auto_logout** Move autologout functionality to a
  seperate module with better (re)usability.
- **dvg_authentication_servicedesk** Add the servicedesk role which allows
  servicedesk employees to override authentication on forms.

DvGCoop 0.11.1
-----------------------
- **dvg_authentication_tokens** Fix translations and code style issues.

DvGCoop 0.11.0
-----------------------
- **dvg_authentication_tokens** Make webform components (_textfield_, _bsn_)
  readonly when prefilled with token values.

DvGCoop 0.10.1
-----------------------
- **dvg_authentication** Fix SAML level handling.

DvGCoop 0.10.0
-----------------------
- **dvg_authentication** Add README.md.
- Rename dvg_coop CHANGELOG.txt to CHANGELOG.md.

DvGCoop 0.9.0
-----------------------
- **dvg_authentication_privacy** Adds dvg_authentication_privacy to remove
  users an anonymize their data.

DvGCoop 0.8.0
-----------------------
- **dvg_authentication_eidas** Add eIDAS authentication provider.

DvGCoop 0.7.1
-----------------------
- **dvg_authentication_digid** Fix missing DIGID_SECTOR_BSN constant.
- **dvg_authentication** Fix dummy login redirect.
- **dvg_authentication**, dvg_authentication_eherkenning Fix for dummy mode
  for levels.

DvGCoop 0.7.0
-----------------------
- **dvg_authentication_eherkenning** Add eHerkenning as an authentication
  provider.
- **dvg_authentication** Now supports levels for authentication providers,
  e.g. for the eHerkenning provider.
- **dvg_authentication_digid** Updated the migration path and made ready for
  authentication levels.

DvGCoop 0.6.0
-----------------------
- **dvg_authentication** Add functions and hooks for integration of
  dvg_authentication in other modules.
  Also refactor and restucture the code.

DvGCoop 0.5.2
-----------------------
- **dvg_authentication_digid** Fix SimpleSAML error on the DvG Status page.

DvGCoop 0.5.1
-----------------------
- **dvg_authentication** Fix for manually setting the authentication provider
  on test accounts.

DvGCoop 0.5.0
-----------------------
- **dvg_authentication_digid** Add digid authentication provider.

DvGCoop 0.4.1
-----------------------
- **dvg_authentication** Fix for login without authentication provider.

DvGCoop 0.4.0
-----------------------
- **dvg_authentication_manager** renamed to dvg_authentication. Auto logout
  functionality included.

DvGCoop 0.3.0
-----------------------
- **dvg_authentication_manager** Allow login without authentication provider.

DvGCoop 0.2.1
-----------------------
- **dvg_authentication_manager** Fix added update hook to add new
  webform table field.

DvGCoop 0.2.0
-----------------------
- **dvg_authentication_manager** Added webform settings to restrict webforms to
  selected external authentication methods.

DvGCoop 0.1.0
-----------------------
- Initial release
- **dvg_authentication_manager** Added module basics for managing
  authentication.
- **dvg_authentication_dummy** Added module for a dummy authentication method.
