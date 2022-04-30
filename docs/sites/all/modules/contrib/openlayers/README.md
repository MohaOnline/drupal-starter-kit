[![Build Status](https://travis-ci.org/drupol/openlayers.svg?branch=7.x-3.x)](https://travis-ci.org/drupol/openlayers)
[![Coverage Status](https://coveralls.io/repos/drupol/openlayers/badge.png?branch=7.x-3.x)](https://coveralls.io/r/drupol/openlayers?branch=7.x-3.x)

# Openlayers
Openlayers module for Drupal 7.

Openlayers is a module that will help you displaying slippy maps on your site.

# History
Find a brief history and roadmap of this module at this page:
https://groups.drupal.org/node/440733

# Technology
Openlayers was built with performance, reliability and sustainability in mind.

To do so, the module has been split in sub-modules so you can enable only the
modules for you needs.

The module has also be written in a way that the code used in the plugins will
be the same when the Drupal 8 version will be out, hopefully very soon.

# Installation
The best way to install the module and its dependencies is through drush and a
simple command:

```
drush en openlayers -y
```

You may also install the module by just downloading each dependencies manually.

Once the module is enabled, you won't see any user interface. You have to
enable an extra module for it: Openlayers UI

```
drush en openlayers_ui -y
```

Disabling the UI module will not affect your maps, it's just the admin
interface, nothing else.

You can chose the method that you prefer to load the Openlayers library.
Either through a CDN, or locally.

By default, Openlayers is configured to use the local library. You may also
enable the contrib module [Libraries CDN API](https://www.drupal.org/project/libraries_cdn) to have the library automatically loaded without the need to download anything else.

If you want to install it locally, the easiest way to do so is with drush.

We've created a special command that will download the library and install
it for you.

Basically it just download a zipfile and unzip it at the right place for you.

To do it with drush:

```
drush dl-openlayers
```

If you do not use Drush, download the latest Openlayers library from the
official website: http://openlayers.org/ and unzip it at the place where your
libraries are.

Usually it's in /sites/all/libraries, but it can change depending on your
installation.

The directory containing the library must be named: 'openlayers3'.

Once installed, the standard "status report" page at admin/reports/status
will tell you if the library is installed correctly.

You may also use drush to do so:

```
drush libraries-list
```

Openlayers requires jQuery 1.10 or higher to function. The easiest way to
ensure that the correct version is used is through the jQuery Update module.
Do not forget to configure jQuery Update via
admin/config/development/jquery_update, and select jQuery 1.10 or higher.
Note that jQuery Update offers separate settings for the regular and
administration themes, so set both if you are using the Openlayers UI or need
openlayers in the administrative interface.

# Getting help
Has this module is a complete rewrite, it still needs documentation and we,
maintainers, still need to work on that.

To create your first map, you must first understand how Openlayers 3, the JS
library, works.

Each map you create is composed of a couple of basic objects: the controls,
the interactions, the layers.

Depending on the type of object, some of these are also composed of objects.

Layers are composed of: source and style.

There are 5 'core' openlayers components: Controls, Interactions, Layers,
Sources and Styles.

These components can be created within this module.

We've added a custom object: Component.

Those objects 'Components' are added at the end, when the map is fully created,
so you can custom extend the behavior of your maps.

To quickly create a map, here are the steps in this following order:
* Create your source, this is the source that will provide the data to display
  on your map.
* Create your layer, this will need a source and a style to be created.
* Create your map.

# Displaying maps

You can find all the available maps in the maps tabs in
**admin/structure/openlayers/maps**.

To render a map on your site, you have many options:
* Using the API, see [this example](https://gist.github.com/drupol/fee9d808b711f658d79b)
* In a block, enable submodule **Openlayers Block**, then tick the checkbox
 in the map settings, you'll have a new block in **admin/structure/block**,</li>
* In a field, enable submodule **Openlayers Field** and you'll have a new field
 type 'Openlayers map' available,</li>
* On any site, enable submodule **Openlayers Services**, then tick the checkbox
 in the map settings, you'll get an embed url that you can use on any site to
 display the map only.
* In a [Panels](https://www.drupal.org/project/panels), enable submodule **Openlayers Content Types**,</li>
* In a [Box](https://www.drupal.org/project/boxes), enable submodule **Openlayers Boxes**,</li>
* In a [Quicktabs](https://www.drupal.org/project/quicktabs), enable submodule **Openlayers Quicktabs**,</li>
* In a [Views](https://www.drupal.org/project/views), enable submodule **Openlayers Views**.</li>
* In any content using the appropriate input format and the contrib module [Openlayers filters](https://www.drupal.org/project/openlayers_filters).</li>

With Openlayers 3, it's up to you to decide how you want to integrate the map
in your content.

# Sub-modules
Find the documentation of each sub-modules in their README file.

# Extend Openlayers
You can extend or alter the module objects by creating a submodule. Have a look
at the submodule **openlayers_library** to get started, there's plenty of
examples in it.

# Included libraries
* Spin.js: http://fgnass.github.io/spin.js/
* OL3 Layerswitcher: https://github.com/walkermatt/ol3-layerswitcher

.
