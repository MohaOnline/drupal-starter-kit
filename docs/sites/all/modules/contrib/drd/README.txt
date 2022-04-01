-- SUMMARY --

See http://drupal.org/project/drd

-- REQUIREMENTS --

* Drupal 7

-- INSTALLATION --

Install as usual, see http://drupal.org/node/70151 for further information.

-- CONFIGURATION --

See details and instructions on the project home at http://drupal.org/project/drd

-- CUSTOMIZATION --

None

-- SECURITY and TROUBLESHOOTING --

In some really rare cases there might be an issue that a domain wouldn't properly
install, then it's very likely that this is caused by the settings.php file of that
domain and we suggest that you clean up the respective file and try again. Cleaning
up means that you may want to remove all comments and commands that are not really
needed. What DRD is really interested in is to identify the $database array for the
domain so that it can write the security keys into the database for each hosted domain.

However, if that fails, there is also a manual alternative available introduced in
version 7.x-2.3.

To use that, let's explain security in and around DRD: each core in DRD has its
own set of security keys that are created when adding the core to the dashboard.
Those keys and settings have to be transferred to each domain of that core together
with the IP of the DRD core. Those keys are then used to encrypt all parameters
for each request from DRD to their cores and domains and the remote instance is
then using the same keys to decrypt them again. That serves two purposes:

1) Make sure, that DRD requests are only accepted by previously defined DRD instances.

2) Protect the requests and results from prying eyes. In other words, make sure that
no data is ever visible to anyone who should not be able to see them. This needs to
be done with our own encryption because we can't just use SSL which is most likely
not available on all your Drupal domains that you're managing with DRD.

Now, how do those keys get over to your remote domains? Here is the step-by-step
explanation:

- When adding a new core to DRD, new keys and settings are created for that core.

- The admin gets a message with an explanation to follow two steps:
-- Go to admin/settings/drd_settings on the remote domain of the core
-- Input the IP address of the DRD core (the correct IP address is displayed)
-- Click another link in DRD: this will then transfer the keys to that domain

- From that point on, that main remote domain accepts DRD requests from this core

- When a new domain is found on that remote core, DRD is using this first main
domain (which has been verified, see above), to push the same keys to all of the
other domains on that same core. A process in the main domain is writing the keys
into the database for each of the sibling domains and then the same DRD core is
verified as well and can talk to each of those domains directly.

- Sometime (i.e. on Aegir installations) the main remote domain has no chance
to find out details about the database settings of its sibling domains. In such
a case, pushing the keys as described automatically will fail. In that case you
have to enter the keys manually (see below)

- How to push security keys and settings manually:
-- Get the settings from the main domain (go to admin/settings/drd_settings)
-- Copy the values to each of the other domains by going to the same path on
each of them

Once all that is done, you're completely set and all your communication is
properly secured.

-- FAQ --

Yet to come

-- CONTACT --

Current maintainer:
* Jürgen Haas (jurgenhaas) - http://drupal.org/user/168924

Current supporter:
* Jons Slemmer (j.slemmer) - http://drupal.org/user/361180

This project has been sponsored by:
* PARAGON Executive Services GmbH: http://www.paragon-es.de
* Joy Group: http://www.joygroup.nl
