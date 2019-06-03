<?php

/**
 * @file
 * Page callbacks for Encrypted Files
 */

/**
 * Page callback that gives user a go back link after an error.
 */
function encrypted_files_download_error() {
  return '<a href="javascript:history.go(-1)">' . t('Click here to go back') .
    '</a>';
}
