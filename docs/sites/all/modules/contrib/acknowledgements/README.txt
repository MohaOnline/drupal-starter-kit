Sign for acknowledgement
------------------------
Date: 03/05/2016
Version: 2.17

WHAT THE MODULE DOES
This module is useful if you have to check which users have read
your drupal documents. When these documents are displayed,
a checkbox appears and the current user should be able to sign
for acknowledgement by clicking on it.
You can also specify the date by which the document must be signed.
Clicking on 'Acknowledgements' tab, administrators can view a list
of all users with specification of whether or not they have signed
the document.
Latest features:
- administrators and authors can select the roles to which the
  document is intended;
- roles selection can be predefined in global configuration.
- roles selection can show only items selected in global configuration.
- an alternative form can be used for specific needs
- a filter for users table has been implemented;
- the ability for the end user to insert a note was added.
- better users selection while editing a content
- now a notification email can be sent to selected users and roles
  while creating a new content.
- when a user or role is added to an old content, email is sent only
  to new users/roles
- views integration: user and date fields added.
- views integration: users other than current user are allowed. Filters improved.

REQUIREMENTS
Date module
Markup module

RECOMMENDED
Multiple Selects module
installed *before* installing or updating "Sign for acknowledgement"

INSTALLATION
install and activate this module as usual.

CONFIGURATION
To configure this module you must login as administrator, go
to modules list and click the "configure" link related to the
module itself. Here you can select the node types that will be
managed by the module task. Also you can:
- customize the messages most frequently used by the module;
- set the characteristics of the table that will be used to list
  acknowledgements (number of rows, columns to display, etc..);
- set the roles of the users who will have to sign for acknowledgement;
- set a few features of the acknowledgment form ("weight" of the form,
  prevent or not the acknowledgment when the signature period is
  expired, show or not the "submit" button, etc.);
- set the text labels that will be used if you want to use the 'alternate
  form'.

NOTE: by clicking the link "permissions", the administrator can
select the users who will access to configuration and to users table.

WEB: detailed infos are available at:
https://www.drupal.org/project/acknowledgements

Greetings
Many thanks to:
Nadia Caprotti
Roberto Bisceglia
Giuseppe Castelli
Piermichele De Agostini
Adam Clear
Gioacchino De  Lucia
"Porte aperte sul web" community
Drupal community
