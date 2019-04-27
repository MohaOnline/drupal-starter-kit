
-- SUMMARY --

This module manages connections to external semantic services (PoolParty server,
GraphSearch server, SPARQL endpoints), offers APIs to work with them and
makes interconnection between these modules possible.

-- REQUIREMENTS --

- If you want to work with SPARQL-endpoints, the EasyRDF library needs to be
installed: Download the EasyRDF-library (http://www.easyrdf.org/downloads) and
add EasyRdf.php and all the other files and folders to "sites/all/libraries/easyrdf".

-- INSTALLATION --

Install as usual:
see https://drupal.org/documentation/install/modules-themes/modules-7 for
further information.

-- USAGE --

- Activate the module.
- Configure the modules, that require this module.
- Semantic Connector additionally offers an overview over all the created
connections to semantic services at "admin/config/semantic-drupal/semantic-connector"
(Configuration -> Semantic Drupal -> Semantic Connector).
- As soon as more than one Drupal module using the Semantic Connector is
installed, you can configure the interconnections at
"admin/config/semantic-drupal/semantic-connector/config" (Configuration ->
Semantic Drupal -> Semantic Connector -> Configuration).
