Views Data Export PHPExcel
==========================

Description
-----------
This module extends Views Data Export module to allow XLSX files export.
Possibly it will support more formats in the future.

Implementation is based on https://www.drupal.org/node/1269994#comment-9090229.
Many thanks to Johann Wagner <https://www.drupal.org/u/johann-wagner>!

Requirements
------------
1. Views Data Export
2. PHPExcel <https://phpexcel.codeplex.com/> library version >= 1.8.0
3. Libraries

Installation
------------
1. Place PHPExcel library under sites/all/libraries folder, so the resulting
structure should look like:
- sites/all/libraries/PHPExcel
- sites/all/libraries/PHPExcel/Classes
[...]
2. Enable the module.

Usage
-----
Follow the instructions from README.txt of the Views Data Export module.

Credits
-------
Dmitriy Novikov <https://www.drupal.org/u/d.novikov> from SmartWolverine.net
