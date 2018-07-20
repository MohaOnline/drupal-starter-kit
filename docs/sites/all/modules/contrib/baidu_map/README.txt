Baidu Map
---------
It is sometimes said to be more accurate than the Google Maps in China, more
complete than Ali Maps, the Baidu Map module allows geographic and location
information to be displayed through the Baidu Map API
[http://developer.baidu.com/map/].

In short, this module allows to obtain geographic coordinates from textual
addresses in China (Geocode) and display any geographic information through
Baidu Maps.


Features:
---------
The Baidu Map module is an API wrapper which currently only stores a Baidu Map
API key (see Implementation, below for more information).
All of the features are actually provided by the Baidu Map Geofield sub-module
integrated with the Geocoder and Geofield modules APIs.


Implementation:
---------------
The idea behind the Baidu Map module is to provide an API wrapper that would
allow other modules or APIs to potentially integrate with Baidu Map. After
further investigation, it would seem the Baidu Map API key could potentially be
used by other modules that would extend its logic to be integrated with other
Drupal modules' APIs, that's why it was kept as an independent wrapper module.

Then, the integration with Geofield and Geocoder already provides all the
necessary logic related with Field and Storage concerns.
See the Baidu Map Geofield module for more information.


Installation and configuration:
-------------------------------
1 - Download the module and copy it into your contributed modules folder:
[for example, your_drupal_path/sites/all/modules] and enable it from the
modules administration/management page.
More information at: Installing contributed modules (Drupal 7).
[https://drupal.org/documentation/install/modules-themes/modules-7]

2 - Configuration:
a. After successful installation, browse to the Baidu Map Settings form
page under:
Home » Administration » Configuration » Web services » Baidu Map
Path: admin/config/services/baidu_map or use the "Configure" link displayed on
the Modules management page.

b. To start using Baidu Map fill in the Baidu Map API Key as described in
field's help text (first field of the admin settings form). If you don't
already have an API Key, feel free to apply for one on the Baidu API website
[http://lbsyun.baidu.com/apiconsole/key].


Contributions are welcome!!
---------------------------
Feel free to follow up in the issue queue for any contributions, bug reports,
feature requests. Tests, feedback or comments (and of course, patches) in
general are highly appreciated.


Credits:
--------
This module was sponsored by DAVYIN[http://www.davyin.com/] | 上海戴文
[http://www.davyin.cn/].


Maintainers:
------------
xiukun.zhou [https://drupal.org/user/2239768]
DYdave [https://drupal.org/user/467284]
