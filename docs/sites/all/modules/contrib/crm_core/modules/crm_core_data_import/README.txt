CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Installation
 * Configuration

INTRODUCTION
------------
The CRM Core data import module provides and easy way to import data from a 
CSV document or a pre-existing CiviCRM installation into CRM Core.
Configuration is performed from the CRM Core administration user interface.
The importer supports creating nodes, contacts, activities and users and is
based on the robust migrate module import framework.

REQUIREMENTS
------------
To use the CRM Core data import tool, you will need to install a version of 
migrate module newer than the 2.5 release (the latest official release at this
time). This means you will need to use a development version of the module. 
You can get a copy by visiting https://www.drupal.org/project/migrate and 
downloading the 7.x-2.x-dev version.

Alternatively you can download using the drush command:
drush dl migrate-7.x-2.x-dev

INSTALLATION
------------
If you are importing names to "name" fields, which is a very common use case 
with CRM Core, then you will need to apply a patch to the name module from 
https://www.drupal.org/node/2221717. Without the patch, name fields will not 
map correctly. Here's the link to download the patch directly, 
https://www.drupal.org/files/issues/name-2221717-16.patch.

For more information about applying patches, see 
https://www.drupal.org/patch/apply.

CONFIGURATION
-------------
You can find the import administration at admin/structure/crm-core/data-import.

Click "New import" to configure an import.

Give your import a name and select the type of import you want to perform. E.g.
CSV. Select "Next".

Now choose an example CSV to import and select the type of delimiters used in 
the file.

The next page is for mapping fields. Choose and add the target entities you 
want to import from the source data. E.g. Contact of type Individual. Now you
need to define a primary field for the import - the unique identifier of the 
record you are importing in the source data. The primary field can vary, but
e-mail address is often a good example. Now proceed to map the other fields to
be imported to the source data. Select "Next" when you're finished.

The final step is advanced functionality, which can be used to create
relationships between entities. E.g. Individual contacts may be related to
Organization contacts. You can safely skip this configuration if you just want
to import a single entity type.

You are now ready to test your importer. From the list of importers at
admin/structure/crm-core/data-import, tick the checkbox next to your importer
and use the "With selected" menu to "Start import". This will start a batch
operation to import your data. A running status of the import is provided.

If you got this far, congratulations, you have run your first CRM Core data
import! A powerful feature of the migrate framework is the ability to rollback
an import. This can be done from the "With selected" menu and choosing
"Rollback". This means you can confidently try an import and rollback if
something unexpected happens. Change your import configuration and restart
the import.
