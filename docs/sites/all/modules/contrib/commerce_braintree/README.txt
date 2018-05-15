Installation
============

Download, install, and enable the following to your sites modules directory.
See https://www.drupal.org/documentation/install/modules-themes/modules-7 for
more information on downloading and installing modules.

  1. Drupal Commerce
     a. Commerce Payment [https://www.drupal.org/project/commerce]
     b. Commerce Payment UI

  2) Libraries [https://www.drupal.org/project/libraries]

Download and install the braintree php library to your sites libraries directory.
See https://www.drupal.org/node/1440066 for more information on where to install
libraries if you are not familiar with this process.

  1. Download the braintree_php library.
     a. zip: https://github.com/braintree/braintree_php/archive/3.21.0.zip
     b. tar.gz: https://github.com/braintree/braintree_php/archive/3.21.0.tar.gz

  2. Copy the library to your sites libraries folder and name it "braintree_php".
     The path should be libraries/braintree_php/lib if you've extracted it
     correctly.

  3. This module is tested to work with the 3.21.0 release. A minimum of PHP 5.6
     is required along with a version of libcul that supports TLS 1.2. These
     dependencies, while stricter than Drupal 7, are in place because Braintree
     endpoints will not respond to requests using lower TLS/SSL protocols.

Download, install, and enable the Commerce Braintree module(s).

  1. The "Braintree" Drupal module includes an implementation of Braintree's
     basic JavaScript and PHP library implementation.
     https://developers.braintreepayments.com/javascript+php/

  2. The "Braintree (Drop-in UI)" module includes an implementation of Braintree's
     Drop-In UI (iFrame) solution. If Rick Manelius sent you here, this is the
     implementation you want! We recommend his http://drupalpcicompliance.org/
     if you have questions about PCI in Drupal.
     See https://www.braintreepayments.com/features/seamless-checkout > Drop-in UI
     for more information.

  2. The "Braintree Hosted Fields" module includes an implementation of Braintree's
     Hosted Fields solution where each credit card field is an embedded iFrame.
     See https://www.braintreepayments.com/features/seamless-checkout > Hosted Fields
     for more information.

Add your Braintree account credentials for the enabled implementation(s).

  1. Create or gather your Braintree website login credentials.
     https://www.braintreepayments.com

     a. Create a sandbox account for testing via
        https://www.braintreepayments.com/get-started
     b. Create a production account for going live via
        https://signups.braintreepayments.com/

  2. Log into Braintree using either the production or sandbox login options
     to get you API keys for this module.

     Gather the following credentials by visiting Account > My User > View API Keys.
     Click on the 1st (and likely only) public key link to view the key details.
     Note: The order they are displayed does not match the order they are entered
           in Drupal. Pay attention! :)

     * Merchant ID
     * Public key
     * Private key

     Next, you'll need the Merchant account ID (not the same as Merchant ID above).
     You can find this by going to  Settings > Processing and scrolling to the bottom
     of the page. Merchant account ID's are tied to currencies and should match the
     currencies you have enabled on your Drupal Commerce store. If you only have one
     currency then grab the only Merchant account ID listed in Braintree.
     Otherwise, make sure you match these up correctly.

  3. Visit Administration > Store > Configuration > Payment methods
     (admin/commerce/config/payment-methods)

  4. Click edit for either the "Braintree Drop-in UI" or "Braintree Transparent
     Redirect" payment rule. Note: We do not recommend using both at the same time.

  5. Fill out the settings for the implementation you wish to enable.

     a. Enter your payment settings using the API credentials gathered above
     b. Select "Sandbox" server if you've registered using a sandbox account
     c. Select "Production" server if you're using a real account.
        Note: These are seprate accounts and the setting you choose has to
              match your API credentials entered above.

  6. Save the payment settings and enable the payment method.

     a. After you save the settings, you'll be redirected back to the payment
        "rule" page. Expand the "Settings" field set and check the
        "Active" box.
     b. Or, click "Save changes" again to return to the primary payment method
        settings form. From there, click the "enable" link next to your
        configured implementation.

Testing
=======

To test your implementation, add a product to your cart, proceed thorough checkout
and enter a credit card. We recommend using a sandbox account for this before
attempting to go live. If you're using a sandbox account, you can use the following
credit card to test a transaction

Credit Card #: 4111 1111 1111 1111
Expiration Date: Any 4 digit expiration greater than today's date
CVC (if enabled): Any 3 digit code

Troubleshooting
===============

If you have issues with your implementation, view the Drupal.org issue queue for this
module https://www.drupal.org/project/issues/commerce_braintree?categories=All

If you cannot find another issue that matches yours, feel free to open a support request,
bug, or other issue type.
