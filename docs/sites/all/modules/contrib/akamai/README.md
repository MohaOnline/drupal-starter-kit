
# Akamai module for Drupal

## Installation

* Unpack the akamai folder and contents in the appropriate modules directory of your Drupal installation, normally `sites/all/modules/`.
* Enable the akamai module on the Modules admin page at `admin/build/modules`.
* You will also need a license to use Akamai's network, http://www.akamai.com/index.html.

## Setup

After installing the module, you may enter your Akamai API credentials at `admin/config/system/akamai`.

For instructions on obtaining API credentials, see https://developer.akamai.com/introduction/Prov_Creds.html

### Credential storage

Akamai API credentials may be stored in the database or in a `.edgerc` file. In either case, they are stored as plaintext. The decision depends on your risk tolerance. If you choose to store the API credentials in the database, they may be exposed through SQL injection vulnerabilities or lost database backups. Depending on your Drupal configuration, credentials stored in the database may also be cached in other systems such as memcached or redis.

By storing credentials in a file, you can mitigate some of those risks. For example, if providing a database backup to a developer, the credentials won't be lost a laptop containing the database backup is lost or stolen.

For more information on this topic, see https://docs.acquia.com/articles/storing-private-information-securely-drupal

For instructions on generating a `.edgerc` file, see https://developer.akamai.com/introduction/Conf_Client.html

## Usage

The module allows two forms of interaction for clearing the cache.
1. Block form:
  Enable this on the admin/build/block section of the admin UI and put the block where you want. This form will clear the path that is listed above the button, (likely the page you are currently viewing).
2. Refresh Tool tab:
  The Refresh Tool tab is found at, admin/settings/akamai/refresh. It is part of the admin interface for this module. From here you can list several paths to be cleared.

The Akamai module also offers integration with Context through the context_http_header module.

## Hooks

Occasionally when one path is cleared, others will need to be cleared
as well. To enable this, we have an alter hook you can implement.

```
function HOOK_akamai_paths_alter(&$paths, $node)
```

 * `$paths` An array of URL paths to be submitted to Akamai for clearing, this
          array can be modified.
 * `$node` If this is a node page you are viewing, this will be that node, otherwise
         it will be NULL.

## Development

To run unit tests with PHPUnit:
* Run `composer install` from the repository root.
* Run `phpunit` from the repository root.
** The PHPUnit configuration will be read from `phpunit.xml.dist`.
