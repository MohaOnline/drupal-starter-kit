Taxonomy Single Tag adds another widget for taxonomy reference fields that
simplifies adding taxonomy that include commas using autocomplete fields by
limiting the field to a single term.

Why such a module might be necessary is explaned at http://drupal.org/node/559756:

   It possible to create a taxonomy-input with autocomplete, but without the
   possibility to select multiple terms? With "Free tagging" its possible to
   separate multiple terms with a comma, but i like to select only one term.
   Autocomplete is necessary because 1000+ terms are too much for a single
   dropdown...


Notes:
 - 6.x-1.0-alpha1 - initial release supported taxonomy_other 6.x-1.1
 - 6.x-1.0-beta1 - removed taxonomy_other support to keep this module simple
    and straightforward, instead of taking care about improving taxonomy_other's
    lack of error reporting; besides that module is growing in many directions
    that won't be tracked by this module, instead taxonomy_other should be aware
    of taxonomy_single_tag or another simplier module should carry the
    responsibility of providing "other" option.
 - 7.x-1.0 - Initial port. Major rewrite to change this from a vocab-based
    solution to a Fields API widget.


Authors:
 - 6.x: arhak (http://drupal.org/user/334209)
 - 7.x: mikeker (http://drupal.org/user/192273)