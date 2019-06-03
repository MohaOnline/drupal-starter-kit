-- SUMMARY --

@todo: add summary

-- CONFIGURATION --

* Configure custom error page on SimpleSAML error:
    -   Add the following setting to the SimpleSAML config file
        (/profiles/dvg/libraries/simplesamlphp/config/confg.PROJECT.php):

        'errors.show_function' => array('DvgDigidErrorhandler', 'show'), // Use the Digid error show function.

    -   The SimpleSAML library runs standalone, so the custom error handler class needs to be available
        independently of drupal and therefore needs to be added to the end of the project config file:

    /**
     * Class DvgDigidErrorhandler.
     */
    class DvgDigidErrorhandler {

      /**
       * Handle SimpleSAML errors.
       *
       * Sends an error report to the administrator with the
       * SimpleSAML error information and redirect the user to a nice error page.
       *
       * @param \SimpleSAML_Configuration $config
       *   SimpleSAML config settings.
       * @param array $data
       *   All error message data.
       */
      public static function show(SimpleSAML_Configuration $config, array $data) {

        // Concat the error reference data into one string.
        $errorCode = $data['errorCode'] . '_' . $data['error']['reportId'] . '_' . $data['error']['trackId'];
        $redirect = 'https://' . $_SERVER['HTTP_HOST'] . '/digid-error-callback?error=' . $errorCode;

        // Redirect to the page where the error occured.
        header('location: ' . $redirect);
        exit();
      }
    }

    -   Create a Functional content Node to show as error page.
