CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Terminology
 * Instalation
 * Credits
 
INTRODUCTION
------------
http://www.idangero.us/sliders/swiper

Swiper - is the free and ultra lightweight mobile touch slider with hardware 
accelerated transitions (where supported) and amazing native behavior.
It is intended to use in mobile websites, mobile web apps, and mobile native
apps. Designed mostly for iOS, but also works great on Android, Windows Phone 8
and modern Desktop browsers. Swiper is created by iDangero.us

TERMINOLOGY
------------
This module provides a specific content type (Swiper Gallery). For each node
of this content type there is a Block with their respective swiper gallery 
content that you can place on your pages.

The gallery implemented by this module has a basic markup structure that you
will find in the file swiper/theme/block--swiper.tpl.php. If you want to change 
this implementation, simply copy and change this file template and put then
into your current theme folder.

You can also add, change and remove, any of API options of the Swipers, 
just you need to implement a hook:
hook_swiper_options_alter($node, $plugin_options) {}

This way the module will handle pass these options to the script that 
instantiates the swiper Plugin.

A complete list of options is available on the project website:
http://www.idangero.us/sliders/swiper/api.php

INSTALATION
-----------
1 - First, install the swiper module (standard Drupal way).
2 - Adds plugin library
  2.1 - Download the library: https://github.com/nolimits4web/Swiper
  2.2 - Unzip the library file in the Drupal path "/sites/all/libraries/Swiper"

Only that. 

To create your first swiper gallery, add a new node of type 'Swiper Gallery',
access the Drupal blocks page, locate the newly created block by the module,
(search by the node title) and adds this block in the pages that you want.

CREDITS
-------
The first credit goes to the creators of this amazing Swiper Plugin:
http://www.idangero.us

This project has been sponsored by:
CI&T
At Ci&T, we range from building a private set of applications to engineering 
an entire IT portfolio. Services include the entire application development 
cycle, including service and project management, business analysis, 
defining requirements, design, code, testing, implementation and support.
  
