# Introduction

Provides custom webform components for DvG.

* BSN (Burgerservicenummer)
Provides a field with BSN validation. Can be prefilled using tokens.

* address / postal code field (with prefill/validation)
Provides a default address field or PO box field (choice is configurable).
In addition: various validation methods are available:
- postcode.nl API
- Stuf-BG AOA
Also: city field is optionally configurable as a pre-defined key-value list.

* readonly
This provides a field with prefill capabilities but without being an input
element.

* stopform
When this element is shown (using conditionals) the form is stopped; no further
submits are possible.
Possible usecase is a scenario when you have multiple options and conditionally
want the visitor only to be able to complete the
form if certain conditions (choices) are met.

# Dependencies:

* webform
* dvg_stuf_bg (optional, for prefill/valdiation)

# Install

This functionality is currently available in the DvG distribution as a Beta.

# Configuration

After enabling, the new components will be available in your webforms.

* postcode.nl API
Note: if you want to use postcode.nl API, you need to manually set the key and
secret variables.
The recommended way to do this is using drush:

- drush vset dvg_postcodenl_key #######
- drush vset dvg_postcodenl_secret ######

* Stuf-BG

If you want to use Stuf-BG prefill/valdiation (based on postal code + number)
you need to enable and configure the dvg_stuf_bg module.

# Under consideration:

* iban field
* multifile module
