API Module
Generates and displays API documentation pages.

CONTENTS OF THIS README:

* Overview
* Installation and setup
* Views integration
* Search integration (Core and Solr search)
* Information for contributors

OVERVIEW

This is an implementation of a subset of the Doxygen documentation generator
specification, tuned to produce output that best benefits the Drupal code base.
It is designed to assume the code it documents follows Drupal coding
conventions, and supports Doxygen constructs as documented on
https://drupal.org/node/1354.

In addition to standard Doxygen syntax requirements, the following restrictions
are made on the code format. These are all Drupal coding conventions (see
https://drupal.org/node/1354 for more details and suggestions).

1. All documentation blocks must use the syntax:

/**
 * Documentation here.
 */

The leading spaces are required.

2. When documenting a function, constant, class, etc., the documentation block
   must immediately precede the item it documents, with no intervening blank
   lines.

3. There may be no intervening spaces between a function name and the left
   parenthesis that follows it.

Besides the Doxygen features that are supported, this module also provides the
following features:

1. Functions may be in multiple groups (Doxygen ignores all but the first
   group). This allows, for example, theme_menu_tree() to be marked as both
   "themeable" and part of the "menu system".

2. Function calls to PHP library functions are linked to the PHP manual.

3. Function calls have tooltips briefly describing the called function.

4. Documentation pages have non-volatile, predictable URLs, so links to
   individual functions will not be invalidated when the number of functions in
   a document changes.


INSTALLATION AND SETUP

See https://drupal.org/node/1516558 for information on how to install and set up
this module.


VIEWS INTEGRATION

The API documentation is integrated with Views. Actually, most of the
listing pages for the API module use Views, and you should be able to clone
and modify these pages to make your own views if you want something different.


SEARCH INTEGRATION

If you enable the included "API Search Integration" module (machine name:
api_search), as well as either the Drupal Core "Search" module or the
contributed Apache Solr search module, you can perform full-text searches
on API documentation, just like your regular site content.


Configuring Core Search

1. Enable the Core Search module and the included "API Search Integration"
module.

2. Visit the API search configuration page, and choose which Core
Compatibilities to index (on admin/config/development/api/search).

3. If you already had core Search and the API module installed and running on a
site, and then enabled the "API Search Integration" module later, you will
either need to run a reparse of the API documentation, or a reindex of Search,
in order to get the full text of the API documentation into the search index.
But if you already had Search but not API, or API and not Search, and then
enable the other one plus the search integration module at the same time, the
API documentation will automatically be indexed for you over the next cron
runs.

In either case, once the indexing is complete, you'll be able to do full-text
searches of API documentation under the "Content" search tab (along with other
node content on your site).


Configuring Solr Search

This is a little more involved. Assuming you already have the Apache Solr
module configured and working, and optionally the Facet API module,
here are the steps:

1. Install/enable the included "API Search Integration" module.

2. On your Solr configuration page (admin/config/search/apachesolr), at the
bottom under Configuration, check "API reference entries" under Node, to
make Solr index API reference information.

3. Visit the API configuration page, and choose which Core
Compatibilities to index (on admin/config/development/api/search).

4. Run cron until the index is complete.

5. If you want to do faceted search, on
  admin/config/search/apachesolr/settings/solr/facets
set up the Branch, Project, Core Compatibility, and Object Type facets (or
some subset). If your search is mixing API items with other nodes on the
site, you may also want the "Content type" facet enabled. API nodes will
show up as content type "API Documentation".

6. On the Blocks page, turn on blocks for your search box and facets. If you
also have the API module's "Search" block enabled, you may want to change
the Search block title to distinguish between full-text full-site search and the
search for exact object names in the API module.

7. You will probably also want to do the following, on
admin/config/search/settings :
- Make Apache Solr search the only search module
- Make Apache Solr search the default search module

8. You may need to set up permission "Use search" for appropriate roles.


INFORMATION FOR CONTRIBUTORS

Here is a somewhat conversational overview of the architecture of the API module
itself (how it actually works), for people interested in contributing to the
development of the API module (revised from an IRC chat log with a potential
contributor):

During cron runs, the module parses the code and comments in PHP files (and some
other files), and saves information in the database. Then when someone visits
api.drupal.org or another site using the API module, they get a parsed view of
the API documentation. In PHP code, any comment that starts with /** rather than
just // or /* is parsed by the API module, and this turns into the documentation
pages on the API site.

For instance, take a look at this code from Drupal Core:
  https://cgit.drupalcode.org/drupal/tree/core/modules/node/node.module#n189
And here is what this node_title_list() function looks like on api.drupal.org:
  https://api.drupal.org/api/drupal/core!modules!node!node.module/function/node_title_list/8
So the @param documentation in the Drupal Core code comment is shown in the
Parameters section on the api.drupal.org page, and so on.

Also the module parses the code itself. For one thing, you can see in the Code
section on that page that it has turned a bunch of stuff into links. For
instance \Drupal is a link that takes you to that class, and its method turns
into a link too. There are also reverse links made from parsing the code: there
is a section that says "2 calls to node_title_list()" on that page that shows
you other functions that call this one -- that comes from parsing the code.

Another detail about the API module is that it uses standard Drupal nodes, in
kind of a fake way (they have no real title/body fields). The reason is that The
api.drupal.org site supports comments, and in Drupal 7 and prior versions,
comments can only be on Nodes.  So the API module creates a fake "node" for each
page that can be commented on -- that means each function, class, interface,
method on a class, constant, etc. -- everything has a page and can be commented
on.

Another detail: There are concepts of "project" and "branch" in the API module.
Drupal Core is a project, and you could also define a project for Views etc.
Within each one, you might have a 7.x branch or a 7.3.x or whatever. You can
have more than one project defined on an API module site, and multiple branches
within each project. For instance, go look at api.drupal.org. It has separate
sections for Drupal 7.x and 6.x and 8.x -- those are the branches. You do not
want to mix them together.

So, back to the cron runs and parsing... The API module uses the Drupal queue
system.  The code can "add" jobs to the queue, and then the Drupal queue system
will periodically (during cron or independently if you tell it to via Drush) use
"worker" functions to process the queue jobs.

The API module defines 3 queues in api_cron_queue_info() in api.module, and that
function also tells what the "worker" functions are that process them. For
instance, the worker for the "update branches" queue is
api_queue_update_branch(). The queue/cron architecture:
- cron: See if any branches need updating, and add them to the update
  branch queue. This is function api_cron() in api.module. It calls
  api_update_all_branches(), which is in the include file api.update_branch.inc.
- update branch queue: See if any files have changed, and add them to the
  parse queue. Also if any files have been deleted, add entries to the node
  delete queue for everything that file used to contain.
- parse queue: Parse a file and update/create entries for everything found
  in the file. Also add entries to the node delete queue if something that used
  to be in the file is no longer defined in that file. This is the hard part.
- node delete queue: Delete nodes that have been identified as obsolete.

A note about node deletes: During both "update branch" and "parse file" queue
operations, sometimes code has been removed and the class/function/etc. does not
exist (either its file has gone away or it no longer exists in the file). In
that case, the fake node associated with that has to be deleted. Node deletion
is time consuming in Drupal (lots of hooks), so rather than deleting the node
directly (especially if there are a bunch of them to delete), they are added to
the node_delete queue.

Aside from the parsing, there is another side to the code, mostly in the
api.pages.inc file, which generates the actual pages on the site from the stuff
that the parse functionality saves in the database. If you have done some Drupal
development, then you can probably figure out how the pages are generated by
looking at hook_menu() and seeing what the page generating functions are, theme
templates, etc.

The API module also has a comprehensive set of automated tests (using the
Simpletest framework in Drupal). This helps us be sure that when we make changes
to the module, we don't break anything.
