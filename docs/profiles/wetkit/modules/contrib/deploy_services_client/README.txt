INTRODUCTION
------------

This module provides a Services client which communicates with Deployment
endpoints (see http://drupal.org/project/services and
http://drupal.org/project/deploy if you are unfamiliar with those terms).

It is intended for developers using Deployment and Services to push content
between sites who would like to easily perform other operations on the content
which the Deployment module does not directly support.

As an example, suppose you have pushed content to a site and would later like
to delete it.  Services supports deleting content (even though Deployment
doesn't), and you could theoretically write custom code to contact the site and
delete it, but in doing so you'll find yourself writing a lot of complex code
to duplicate some of the basic mechanics that you already have wrapped up in
your Deployment configuration (for example, finding the correct URL on the
target site to contact and figuring out how to authenticate with the target
site).

With this module, you can simply load up the Deployment endpoint and call a
single API function; the mechanics will be taken care of behind the scenes, and
your content on the target site will be deleted.

EXAMPLES
--------

The module provides several procedural wrappers for performing common tasks:

1. deploy_services_client_delete_entity_from_endpoint()
   Given a Deployment endpoint and an entity, delete the entity from the
   endpoint.

2. deploy_services_client_delete_entity_from_plan_endpoints()
   Same as above, but performs the action on all endpoints associated with a
   given Deployment plan.

3. deploy_services_client_unpublish_entity_from_endpoint()
   Given a Deployment endpoint and an entity, unpublish the entity on the
   endpoint.

4. deploy_services_client_unpublish_entity_from_plan_endpoints()
   Same as above, but performs the action on all endpoints associated with a
   given Deployment plan.

In addition, the module provides a DeployServicesClient class which can be used
to perform an arbitrary Services request on a given Deployment endpoint.
Example:

<?php
  $endpoint = deploy_endpoint_load('your_endpoint_name');
  $client = new DeployServicesClient($endpoint);
  $client->login();
  $response = $client->request('some/services/path/relative/to/the/endpoint/url', 'POST', drupal_json_encode($data_you_want_to_send));
  //
  // ... Do something with the $response ...
  //
  $client->logout();
?>

Note that the procedural wrappers assume that the UUID Services module is
installed on the target site (which is presumably the case if you are deploying
content to that site via Deployment in the first place) and, naturally, the
delete methods require that the target site is actually configured to allow the
Deployment user to delete entities via Services.

LIMITATIONS
-----------

The module currently makes a couple of hardcoded assumptions about the
Deployment endpoint:

1. It assumes a REST-based JSON communication method.
2. It assumes a session-based authentication method.

Also note that this is an API module only; there is no user interface, and
therefore no reason to install this module unless you are using it in code you
are writing or another module's installation instructions tell you to install
it.

CREDITS
-------

This project was sponsored by Advomatic (http://advomatic.com).
