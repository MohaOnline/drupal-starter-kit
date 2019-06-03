<?php

/**
 * @file
 * Contains SearchApiNoindexAlterNoindexFilter.
 */

/**
 * Represents a data alteration that restricts entity indexes to some nodes.
 */
class SearchApiNoindexAlterNoindexFilter extends SearchApiAbstractAlterCallback {

  /**
   * Overrides SearchApiAbstractAlterCallback::supportsIndex().
   *
   * Returns TRUE only for indexes on node.
   */
  public function supportsIndex(SearchApiIndex $index) {
    return $index->getEntityType() === 'node';
  }

  /**
   * Overrides SearchApiAbstractAlterCallback::alterItems().
   *
   * Removes nodes with Noindex status.
   */
  public function alterItems(array &$items) {
    foreach ($items as $nid => $node) {
      if (!empty($node->search_api_noindex[$this->index->machine_name])) {
        unset($items[$nid]);
      }
    }
  }
}
