Functional Content
------------------

A small module to help with a common problem; a view of which you want to make
the path, title, text above it and perhaps other elements configurable without
changing the view. For example, a view of news nodes. Above this view, you
might want a little text describing the nature of the news nodes. Your
content-editors might not know how to edit a view, or you have the view stored
in a feature. Changing the url, title or the text above the view would be hard
then.
That's when this module comes in! Create a view block, create a node where you
want the view to appear, enable the functional content for this view, and enter
the node id (or select the node) in the created Functional Content item. (See
'Usage' for the steps to follow.) If you now view the node you created, you'll
see the view right there! And the best thing is that your content-editors can
edit the node without breaking the view! They also can't delete the node,
because it's used by a Functional Content item.


Installation
------------
Functional Content can be installed like any other Drupal module: place it in
the modules directory for your site and enable it on the `admin/modules` page.


Hooks
-----
See `functional_content.api.php` for the available hooks.


Usage
-----
1. Create a view with a block display
2. Create a node and remember its node id
3. Go to the functional content settings
4. Enable the functional content for the view block you just created
5. Optionally have the module generate a context; enter the context name, tag
   and select the region for the view block
6. Save the settings and go to the functional content nodes
7. Enter the node id from the node you created step 3
8. If you did generate the context, you're ready to go! Just browse to the node
   you created in step 3 and see the marvelous view appear in the region you
   selected!
   If you did not choose to automatically generate the context, you can create
   your own; just select the correct Functional Content-callback in the
   Callback conditions, etc.


Advanced Usage
--------------
You can also create your own functional content items. See
`functional_content.api.php` for the available hooks. Creating your own
Functional Content items would be used for any node that could be seen as
'configuration'. Several good examples are in the dvg distribution:
https://www.drupal.org/project/dvg.

For example the dvg_ct_block feature. This feature shows the contact block on
the front page and several other blocks. A few blocks are defined in the code,
for each block a Functional Content item is created. For each block, a node is
created and its node id configured in the correct Functional Content item. The
titles and contents of these blocks are managed by content editors through
nodes.
