<?php
/**
 * @file
 * URL Processor for WetKit Solr.
 */

/**
 * Extension of FacetapiUrlProcessor.
 */
class WetKitSolrUrlProcessor extends FacetapiUrlProcessorStandard {
  /**
   * Overrides FacetapiUrlProcessor::fetchParams().
   *
   * Use custom path as the source for facet data.
   */
  public function fetchParams() {
    return $_GET;
  }
}
