<?php

/**
 * @file
 * Integrates Entity properties with synonyms "search" behavior.
 */

if (module_exists('synonyms_search')) {
  /**
   * Definition of SearchPropertySynonymsBehavior class.
   */
  class SearchPropertySynonymsBehavior extends PropertySynonymsBehavior implements SearchSynonymsBehavior {
  }
}
