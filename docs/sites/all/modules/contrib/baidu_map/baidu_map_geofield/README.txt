Baidu Map Geofield
------------------
It is sometimes said to be more accurate than the Google Maps in China, more
complete than Ali Maps, the Baidu Map module allows geographic and location
information to be displayed through the Baidu Map API
[http://developer.baidu.com/map/].

In short, this module allows to obtain geographic coordinates from textual
addresses in China (Geocode) and display any geographic information through
Baidu Maps.


Features:
---------
All of the features are actually provided by the Baidu Map Geofield sub-module
integrated with the Geocoder and Geofield modules APIs.

1 - Baidu Geocoder: (Currently supports Geocoding API v2.0)
[http://developer.baidu.com/map/webservice-geocoding.htm]
Integrates with Geofield (Geocode from field) through a Geocoder Plugin that
returns geo-coordinates from any address in China.
* Geocode address or location data from numerous types of fields: text,
  addressfield, location, computed, taxonomy_term_reference, etc...
* Improved handling of addressfield values to match with China's addresses
  format and support for China address field.
* Filter geocoding results based on precision level.

2 - Baidu Map Display Formatter: (Currently supports Map Javascript API v2.0)
[http://developer.baidu.com/map/reference/]
Provides a formatter for Geofield values to be displayed through Baidu Maps.
* Supports all Geometries provided by GeoPHP (Polygon, linestring,
  multilinestring, etc...) and Geofield Widgets (WKT, Bounds,
  Longitude/Latitude, GeoJSON and geocoding from field).
* Provides numerous Baidu Map display formatter settings such as size, default
  zoom, map control types and many other display properties.
* Overridding Marker Bubbles (InfoWindow) from Theme.
* Automatic adjustment of Map's center and zoom to display all available data
  in the same viewport.
* Supports display of multiple field values.
* Integration with Views and Variable API.


Implementation:
---------------
The integration with Geofield and Geocoder already provides all the necessary
logic related with Field and Storage concerns.
The Baidu Map Geofield module's code was greatly inspired and adapted from the
excellent Geocoder (7.x-1.x branch, see google.inc) and Geofield (7.x-2.x
branch, see geofield_map) modules and their integration with Google Maps.
Several features provided by Google Geocoder and Maps APIs had to be adjusted
since Baidu has its own (slightly different) ways, methods and API to provide a
similar set of features.

The Baidu Map Geofield module provides a Baidu Geocoder to geocode textual
addresses in China and a display formatter to display any Geofield values
through Baidu Maps.

Main differences with the Google Geocoder and Geofield implementations:
For the Geocoder, although Google certainly provides more advanced features,
Baidu is probably more accurate or would provide perhaps more data when
geocoding requests in Chinese for China.
In terms of features, the major difference is that Baidu would return at most a
single location result, no matter how generic the request could be (for
example, 火车站 / Train station), when Google Geocoder could return multiple
results with various precision/approximation/filtering parameters or
Geometries. With a single point returned at most, fewer supported properties,
unfortunately, Geometry Types, such as Bounds or Viewport and other filtering
parameters provided by Google don't seem to be currently supported by Baidu
Geocoder.

For the Baidu Map display formatter, the entire Geofield Map module could be
re-used almost "as is". In terms of JS Map API, Google still seems to provide
more advanced features than Baidu, which mostly resulted in fewer properties
for the field formatter settings form (customization of the display). Another
notable difference was found in the way Google Map API handles complex paths,
such as multiple polygons, as a single path, which is not the case for Baidu
Map API, requiring each different polygon to be treated as a specific
path/group/overlay (see GeoJSON to Google Maps for more information).

Otherwise, overall, GeoJSON to Google, formatter settings form, Views support,
Google Geocoder, geofield_map JS code, etc... had all to be slightly adapted to
match closer with Baidu's APIs, but in general, most of Google Maps features
could cross-over very nicely with a great amount of similarities (Some parts
could even be re-used almost exactly "as is", such as for the formatter form or
the views display style plugin).


Installation and configuration:
-------------------------------
0 - Prerequisites:
Requires Geocoder, Geofield and Baidu Map modules to be installed.
[https://drupal.org/project/geocoder]
[https://drupal.org/project/geofield]

1 - Download the module and copy it into your contributed modules folder:
[for example, your_drupal_path/sites/all/modules] and enable it from the
modules administration/management page.
More information at: Installing contributed modules (Drupal 7).
[https://drupal.org/documentation/install/modules-themes/modules-7]

2 - Configuration:
After successful installation, two new components should be made available for
configuration:

a. Baidu Geocoder:
For any existing or newly created Geofield field, with the widget type "Geocode
from another field", the Baidu Geocoder could be selected from the main field
configuration settings form, in the required dropdown field "Geocoder".
For example: the page content type:
Home » Administration » Structure » Content types » Page » Manage fields

b. Baidu Map display formatter:
Browse to the "Manage Display" settings page, for the entity (Node content
type, for example) with a Geofield field, to configure the formatter.
For example: the page content type:
Home » Administration » Structure » Content types » Page » Manage display


Useful Resources:
-----------------
For any questions and problems, you may find some help on the official Baidu
Map developer's site [http://developer.baidu.com/map/]:
* Baidu Geocoder API documentation
  [http://developer.baidu.com/map/webservice-geocoding.htm]
* Baidu Map JS API documentation [http://developer.baidu.com/map/reference/]
* Baidu Map Forum[http://bbs.lbsyun.baidu.com/]:
  For general discussions, inquiries, future developments, API changes, etc...
* Baidu Geocoder and Map API general service agreement(TOS)
  [http://developer.baidu.com/map/question.htm#qa0013]
* Make sure you read module's JS examples to extend the Baidu Map JS API in the
  file located at: baidu_map/baidu_map_geofield/js/examples.js
* For further readings and features extensions: Baidu Place API v2.0
  documentation [http://developer.baidu.com/map/webservice-placeapi.htm]


Future developments:
--------------------
*Module is subject to changes and restrictions from the Baidu Geocoder and Map
Javascript APIs.*

For more information about Baidu Map future plans and developments, please
check the project page or the issue queue.

Efforts on documentation (especially in Chinese), tutorials/presentations,
module's tranlations, more simpletests, more code snippets and of course the
usual bug fixes.


Contributions are welcome!!
---------------------------
Feel free to follow up in the issue queue for any contributions, bug reports,
feature requests. Tests, feedback or comments (and of course, patches) in
general are highly appreciated.


Credits:
--------
Hopefully, with the help of the community, for testing, reviewing and
reporting, new features, patches or tests added to this module might as well be
ported/adapted to be back with the Geocoder or Geofield Map modules.

This module was sponsored by DAVYIN[http://www.davyin.com/] | 上海戴文
[http://www.davyin.cn/].


Maintainers:
------------
xiukun.zhou [https://drupal.org/user/2239768]
DYdave [https://drupal.org/user/467284]
