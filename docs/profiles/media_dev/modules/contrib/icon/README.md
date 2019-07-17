## Introduction


The Icon API module provides integration for icon bundles and icon providers
throughout Drupal.

Integrations - the Icon API provides the following submodules:

 * `icon_block` - icon support for blocks.
 * `icon_menu` - icon support for menu items.
 * `icon_field` - icon support on field-able entities.
 * `icon_filter` - icon support as a filter for text-area fields.

For a full description of the module visit:
<https://www.drupal.org/project/icon>

To submit bug reports and feature suggestions, or to track changes visit:
<https://www.drupal.org/project/issues/icon>


## Installation

Install the Icon API module as you would normally install a contributed
Drupal module. Visit https://www.drupal.org/node/895232 for further information.

## Configuration

1. Navigate to `Administration > Modules` and enable the module.
2. Navigate to `Administration > Configuration > Media > Icons` to enable the
   desired Icon bundle and select the `Configure` contextual link. Save the
   Bundle.

## Recommended Providers

By itself, this module does very little other than provide an API so that
other providers may interface with Drupal.

It does contain a single "default" icon bundle named:
[Lullacons](https://www.lullabot.com/articles/free-gpl-icons-lullacons-pack-1).

However, you may choose several other projects that may provide addition
icon bundles:

 * [IcoMoon](https://www.drupal.org/project/icomoon)
 * [Fontello](https://www.drupal.org/project/fontello)
 * [Bootstrap](https://www.drupal.org/project/bootstrap)
 * [Font Awesome](https://www.drupal.org/project/fontawesome)
 * [FAMFAMFAM](https://www.drupal.org/project/famfamfam)

## API/Documentation

Icon API comes with extensive documentation on how to integrate your module or
service provider. Checkout the `icon.api.php` file located in the root of this
module. If you need additional help, look at the previously mentioned providers
for examples.


## Usage in Code

All you need to know is the bundle's machine name and the icon machine name,
Icon API handles the rest!

```php
  $element[] = array(
    '#theme' => 'icon',
    '#bundle' => 'my_bundle',
    '#icon' => 'my_icon',
  );
  print drupal_render($element);
```
