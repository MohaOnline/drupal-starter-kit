<?php

/**
 * @file
 * Documentation of hooks invoked by this module.
 *
 * There is no actually executed code in this file.
 */

/**
 * Gather information about defined filters.
 *
 * @return string[][]
 *   An array of an array of filter class names. The array-keys are:
 *   - type of filter: either 'supporter' or 'content'.
 *   - machine name: A unique machine name for this filter.
 */
function hook_campaignion_manage_filter_info() {
  return $filter['supporter']['country'] = SupporterCountry::class;
}
