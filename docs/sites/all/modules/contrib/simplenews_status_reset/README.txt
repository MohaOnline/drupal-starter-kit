Simplenews Status Reset module
------------------------------

DESCRIPTION
-----------

Simplenews Status Reset is a tiny module, that allows to reset the sent status
of an already sent Simplenews newsletter issue to "not sent". This allows to
resend a newsletter (e.g. to a different category) without duplicating the node
or changing the status value by hand in the database.


MOTIVATION
--------

Simplenews is a great tool to setup a newsletter on your website - it's
probably the best open-source newsletter software. You can even have different
categories, where every category has its own subscriber list. It suits
perfectly for the normal needs of a website newsletter. However, we experienced
that some customers would rather like to use it as an e-mail marketing software
rather than a plain newsletter system because they cannot or are not willing to
pay for a dedicated e-mail marketing software. The customers often have
thousands of e-mail addresses segmented to different lists (by region, branch,
etc).

Each list may be shaped into its own newsletter category, but the customers
than often want to setup a newsletter issue, and send it e.g. to list A, B and
E, the next issue to B, C and D,... Simplenews isn't designed for this kind of
use: after a newsletter issue has been sent out, it is locked and cannot be
sent another time. You could now either clone the node and send it to a
different category, or you can do database manipulation by hand - but both you
and your customer would not want, that it's up to you to change the status
every time they want to resend an newsletter.

This is the point, where this tiny module jumps in: it adds a button to the
"newsletter" tab on already sent Simplenews nodes to reset its status.
That's it!


REQUIREMENTS
------------
This module requires the following modules:
 * Simplenews (https://drupal.org/project/simplenews)


INSTALLATION
------------

Install as you would normally install a contributed drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7
for further information.


CREDITS
-------

The Simplenews Status Reset module was originally developed by
Mag. Andreas Mayr (www.agoradesign.at).
