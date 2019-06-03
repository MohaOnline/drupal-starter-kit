<?php

namespace Drupal\campaignion_email_to_target;

/**
 * A cache_bin that immediately forgets it's values.
 *
 * We use this to deactivate the token-cache so we can have dynamic tokens.
 */
class NullCacheBin implements \DrupalCacheInterface {
  public function get($cid) {
    return FALSE;
  }

  public function getMultiple(&$cids) {
    return [];
  }

  public function set($cid, $data, $expire = CACHE_PERMANENT) {}

  public function clear($cid = NULL, $wildcard = FALSE) {}

  public function isEmpty() { return TRUE; }
}
