#CUSTOM MENU TOOLBAR

This module allows you to use custom menus as administrative toolbars for different roles.

The main idea behind this module is to have complete control over administrative toolbar for non-technical roles that do not need to have the full access to Drupal backend.

There are two main issues with the standard solutions such as the core "Toolbar" or "Admin Menu" modules:

1. They automatically generate the links based on the admin paths.
2. These links cannot be customized only for specific user role.

This module solves both of these issues and gives you full freedom to name, order and create any links people working on your site need.

##Installation

1. Install the module.
2. Create a new menu that you want to use as a toolbar.
3. Navigate to `/admin/people/permissions` to grant necessary permissions.
4. Navigate to `/admin/config/administration/custom-menu-toolbar/menus` to pair the menu you just created and a user role.
5. Optionally you can customize the colors and some other minor settings of this module at `/admin/config/administration/custom-menu-toolbar/configure`.

That's it!
