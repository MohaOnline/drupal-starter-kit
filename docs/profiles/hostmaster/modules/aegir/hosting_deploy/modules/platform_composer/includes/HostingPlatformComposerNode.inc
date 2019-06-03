<?php

/**
 * @file
 * The HostingPlatformComposerNode class.
 */

class HostingPlatformComposerNode extends HostingNode {

  protected $info_fields = [
    'field_composer_project_docroot',
    'field_composer_project_package',
    'field_composer_project_path',
    'field_composer_project_version',
  ];

  public function nodeInsert() {
    // Fields are cached empty when platform nodes are initially created. So,
    // we clear the cache for this specific node right after it's inserted, so
    // that its fields are available to pass to the back-end.
    cache_clear_all('field:node:' . $this->getNid(), 'cache_field', TRUE);
  }

}
