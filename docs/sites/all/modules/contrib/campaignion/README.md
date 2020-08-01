# Campaignion

This project contains the main functionality of campaignion a distrubution and SaaS for online campaigning and online fundraising.


## Browser compatibility

JavaScript in this project is transpiled for IE 11 and Edge 18 compatibility. It does however *not* include the necessary polyfills. Adding these is left as responsibility of the distribution.

We recommend using [polyfill.io](https://polyfill.io) or something similar, for example by adding this in your profile’s `hook_init()`-implementation:

```php
if ($polyfill_url = variable_get('polyfill_url', 'https://polyfill.io/v3/polyfill.min.js?flags=gated')) {
  drupal_add_js($polyfill_url, [
    'type' => 'external',
    'group' => 'library',
    'every_page' => TRUE,
    'weight' => -50,
  ]);
}
```
