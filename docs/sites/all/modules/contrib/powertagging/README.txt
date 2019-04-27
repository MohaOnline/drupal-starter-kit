
-- SUMMARY --

The PowerTagging module is able to analyse content from Drupal nodes including
file attachments. PowerTagging annotates content automatically with concepts
from a thesaurus or taxonomy by using all their names (incl. synonyms). Users
can curate all suggested tags or can also index collections of Drupal content
nodes automatically resulting in a semantic index. This makes search more
comfortable than ever before.

-- REQUIREMENTS --

- PPT & PPX (PoolParty Enterprise Edition) are required as an extraction source
of concepts used for tagging.
- cURL needs to be installed on the web server your Drupal-instance runs on.
- The "Semantic Connector"-module (https://drupal.org/project/semantic_connector)
needs to be installed and enabled.
- The "Sliderfield"-module (https://drupal.org/project/sliderfield) needs to be
installed and enabled.
- If you plan to use multilingual tagging, the "Internationalization"-module
(https://drupal.org/project/i18n) and its sub-module "Taxonomy Translation" are
also required.

-- INSTALLATION --

- Enable first the modules from the "Requirements"-list above and then the
PowerTagging module. See
https://drupal.org/documentation/install/modules-themes/modules-7 for further
information.

-- USAGE --

- Configure a PowerTagging configuration at admin/config/semantic-drupal/powertagging.
- Add the now available "PowerTagging Tags" field to all the entity bundles you
want. Currently supported entity types are nodes, users and taxonomy terms.
- After completely configuring the new field, tagging is available inside the
edit-area of entities of your configured bundle.
- For batch operations go to the "Batch jobs" tab in the configuration of a
specific PowerTagging configuration. (admin/config/semantic-drupal/powertagging/XXX)
