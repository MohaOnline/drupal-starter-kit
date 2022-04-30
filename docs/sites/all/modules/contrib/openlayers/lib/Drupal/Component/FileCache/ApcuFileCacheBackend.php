<?php

namespace OpenlayersDrupal\Component\FileCache;

/**
 * APCu backend for the file cache.
 */
class ApcuFileCacheBackend implements FileCacheBackendInterface {

  /**
   * {@inheritdoc}
   */
  public function fetch(array $cids) {
    return apc_fetch($cids);
  }

  /**
   * {@inheritdoc}
   */
  public function store($cid, $data) {
    apc_store($cid, $data);
  }

  /**
   * {@inheritdoc}
   */
  public function delete($cid) {
    apc_delete($cid);
  }

}
