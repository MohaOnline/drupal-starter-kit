Description
-----------

  Bartik Admin - Super admin theme as a Batrik theme.

  During the development We don't want to mess up with other themes. It's a developer friendly only for development purpose. Always we have to set the superadmin as bartik. If you want you can override with the configuration settings. It will be available only on the superadmin user (Superadmin uid as '1').

  Super administration theme will be override the admin pages as batrik. Drupal allows you to define a different theme for administration pages.

Installation
------------

1) Copy the bartik_theme module folder to the sites/all/modules directory of your Drupal 7. 
2) Enable the Bartik admin module in Drupal Modules page.(administer -> modules).

Configuration
-------------

An option provided to override the other enabled themes.
If you want to overwrite the superadmin theme goto profile page (user/<uid>/bartik-admin)
only enabled themes are shown in the settings page.

If Bartik theme is disabled means, it will take the default ADMINISTRATION THEME (/admin/appearance) as a superadmin user.