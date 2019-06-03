## Installation
Install the module as per the standard instructions on http://drupal.org/getting-started/install-contrib/modules

1. You will need a funnelback account. This is an offline process that requires contacting Funnelback.
2. Place the Funnelback search block in region, the search result page URL is /funnelback/search
3. To use facet block,you will need to
  * configure facet service in Funnelback admin dashboard to make sure you have facet data in your response
  * place the Funnelback facet block in region
4. To use view mode in your search results page, you will need to
  * edit metamap.cfg file in Funnelback admin dashboard
  * add 'nodeId,0,nodeId' to a new line in the file

## FAQs

### How can I use view mode for the search results page?
1. Visit /admin/config/search/funnelback
2. In Result display settings section, tick the 'Use display mode to render results' checkbox
3. Select the view mode from the drop down list below
4. Make sure you have the selected view mode configured for content types in your index
5. Default Funnelback result template will be used if the result is not a node

### What are the basic settings for Funnelback module
1. A base URL that Funnelback provides
2. A collection name that Funnelback provides
3. A profile name from Funnelback you want to search in, '_default' is used by default

### How can I override Funnelback templates
The available templates are:
* `funnelback-breadcrumb.tpl.php` - for selected filters
* `funnelback-contextual-nav-block.tpl.php` - for contextual navigation block
* `funnelback-curator.tpl.php` - for Best bet/curator block
* `funnelback-facets-block.tpl.php` - for facets blocks
* `funnelback-pager.tpl.php` - for pager
* `funnelback-result.tpl.php` - for single result record
* `funnelback-results.tpl.php` - for results content region
* `funnelback-spell.tpl.php` - for spell suggestion
* `funnelback-summary.tpl.php` - for search result summary block

These templates can be found in module funnelback/templates folder, copy either of them into your theme folder to override the markup.
Preprocess function can be used to alter the data in the templates, for example:
* `THEME_preprocess_funnelback_breadcrumb(&$variables)`

### How can I have specific field returned from Funnelback
To have Funnelback index specific field as metadata, you need to:
* Have the field value output in the page as in metatag. Use `drupal_add_html_head()` function to add specific value in your preprocess function, see example in funnelback.module line 199
* Add the custom metatag to metamap.cfg file in Funnelback dashboard: I.E `contentType,0,contentTpe`. The metatag name need to match the name in previous step
* Update the index via the Funnelback dashboard

### How can I edit facet blocks
1. Click `Customise Faceted Navigation` tab in Funnelback dashboard
2. Click `Add new`
3. Follow the wizard to add a new facet service
4. Update the collection index
   
### How can I use custom template for my response JSON
1. Contact your Funnelback account manager to create a custom template
2. Submit the custom template file name to configuration `Custom template name` field

Note: this feature is only for advanced user. The custom template needs to remain the same structure as the default JSON payload. Missing field will lead to the search failure.