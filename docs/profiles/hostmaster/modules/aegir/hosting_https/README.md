# Aegir HTTPS

This module enables HTTPS support for sites within the [Aegir Hosting System](http://www.aegirproject.org/) using certificate management services such as [Let's Encrypt](https://letsencrypt.org/), whose support is included.

It provides a cleaner, more sustainable and more extensible implementation that what's currently offered in Aegir SSL within Aegir core, and doesn't require workarounds such as [hosting_le](https://github.com/omega8cc/hosting_le).

## Requirements

1. Aegir 3.9+ or the patch from [Remove 'node_access' check from default hosting_get_servers() calls](https://www.drupal.org/node/2824329#comment-11772591).  See [hosting_certificate_prevent_orphaned_services() causing recursive/loop cache rebuild](https://gitlab.com/aegir/hosting_https/issues/7) for details.
2. If you're running the Nginx Web server and would like to use Let's Encrypt certificates, be sure to prevent Nginx's default configuration from running.  Otherwise, it will prevent this server configuration from allowing access to the challenge directory.
    * `sudo rm /etc/nginx/sites-enabled/default`
3. By using the LetsEncrypt submodule you accept the terms of service from [LetsEncrypt](https://acme-v01.api.letsencrypt.org/terms)
4. To get a LetsEncrypt certificate all your site's aliases need to be resolvable in the global DNS.

## Architecture

This module is build up of several sub-modules that let the user choose between Apache and Nginx, and between certificate services.
Certificates are generated on the hostmaster server and pushed out to (cluster) slaves.
For the LetsEncrypt submodule also the well-known/acme-challenge files are synced out to the slave server for validation.


## Installation

1. Cleanup old SSL usage.
    * Check that the hostmaster site is not set to Encryption: Required. (e.g. on /hosting/c/hostmaster) to avoid locking yourself out.
    * Edit the server nodes(e.g. /hosting/c/server_master) to not use an SSL service.
    * Disable any of the SSL modules (including hosting_le) you may have already enabled.
2. Surf to Administration » Hosting » Optional » Aegir HTTPS.
3. Enable at least one certificate service (e.g. Let's Encrypt or Self-signed).
4. Enable at least one Web serrver service (e.g. Apache HTTPS or Nginx HTTPS).
5. Save the configuration.

## Server Set-Up

1. Surf to the Servers tab.
2. Click on the Web server where you'd like HTTPS enabled.
3. Click on the Edit tab.
4. Under Certificate, choose your desired certificate service (and set any of its additional configuration).
5. Under Web, choose the HTTPS option for your Web server (and set any of its additional configuration).
6. Hit the Save button.

## Site Set-Up

1. Ensure that there's a DNS entry for the site that you'd like HTTPS enabled (unless already handled by a wildcard entry pointing to your Aegir server).
2. Verify the site if this hasn't been done since the server was set up with the above steps.  This ensures that the site can respond to the certificate authority's challenge.
3. Edit the site.
4. In the HTTPS Settings section, choose either Enabled or Required.
5. Save the form.
6. Repeat these steps for any other sites for which you'd like to enable HTTPS.

## Upgrading from site specific installation

Since 3.14.0 hosting_https is included in the main Aegir distribution.
If you installed it earlier in e.g. the sites/example.com/modules then you can get errors when you just remove that old version.
To avoid these you have to manually fix the paths in /var/aegir/.drush/drushrc.php to point to the profile version of this module. After that you can verify the hostmaster site.

## Certificate Renewals

For the Let's Encrypt certificate service, this should get done automatically via the Let's Encrypt queue. It will run a Verify task on each site every week as site verification is where certificates get renewed if needed. The seven-day default was chosen to match the CA's [rate limits](https://letsencrypt.org/docs/rate-limits/).

## Forcing Certificate Regeneration

If you'd like to force a site's certificate regeneration, perhaps because you just changed the Web server configuration from Staging to Production, you can use this Drush command:

`drush @site1.example.com letsencrypt-force-key-regenerate`

If the new certificate isn't immediately available afterwards, you'll also have to Verify the site.  This is generally required for Hostmaster, the Aegir site itself.

## Known Issues

See [the issue queue](https://www.drupal.org/project/issues/hosting_https).

## Troubleshooting

If you notice that the certificate generation fails you can check the Aegir 'Verify' task logs for details.
See also http://docs.aegirproject.org for more generic help.

### Test the challenge directory

Create a file e.g. called `index.html` in `/var/aegir/config/letsencrypt.d/well-known/acme-challenge/` and test if you can access it over http via http://www.example.com/.well-known/acme-challenge/index.html

If your request is redirected to a *https* url then that could pose a problem when the certificate there is either invalid or expired. Try to remove the redirects.
