<?php

/**
 * @file
 * Defines a handler for a string argument for a file name with ! separators.
 */

/**
 * Handles an argument that contains a file name with ! separators.
 */
class api_views_handler_argument_filename extends views_handler_argument_string {

  /**
   * Builds the query.
   */
  public function query($group_by = FALSE) {
    $this->argument = str_replace('!', '/', $this->argument);
    return parent::query($group_by);
  }

}
