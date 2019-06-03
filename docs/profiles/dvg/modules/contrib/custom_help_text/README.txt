DESCRIPTION
----------------------------
This module integrates into the Help module (using the hook_help() hook) to add 
help messages based on the URL. There is an administration form that allows 
users to set the messages and where they appear on the site. Multiple help 
messages can be added to the same page at once, the administration form can be 
ordered to sort the order that these messages appear.

In order to make the help messages appear you need to make sure that the 
$messages variable is printed on your template layer. Most themes will do this.

You can also allow certain roles to be able to view the help items by selecting 
them on the add/create form.

FEATURES SUPPORT
----------------------------
Help texts are exportable with the module Features.

INTERNATIONALIZATION SUPPORT
----------------------------
The module has translatable help texts and configuration options
via the use of i18n_strings and i18n_variables.

BLOCK SUPPORT
----------------------------
Help texts can be displayed on every block. To use this feature,
activate the sub-module custom_help_text_block.

SIMPLETESTING
----------------------------
This module comes with a few simpletests that cover all of the existing
functionality of the module.

CREDITS
----------------------------
Initial authored by Philip Norton <philipnorton42@gmail.com>
Maintained by Tessa Bakker <tessa@tessabakker.com>
Thanks for digital006, klausi and beanluc for their help in getting this module
finished.
Thanks for ezCompany for sponsoring time to add Internationalization support and
Block support.
