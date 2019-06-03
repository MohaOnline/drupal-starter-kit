<?php

/**
 * @file
 * The HostingPlatformComposerGitNode class.
 */

class HostingPlatformComposerGitNode extends HostingNode {

  protected $info_fields = [
    'field_composer_git_docroot',
    'field_composer_git_project_url',
    'field_composer_git_path',
    'field_composer_git_version',
  ];

  public function nodeInsert() {
    // Fields are cached empty when platform nodes are initially created. So,
    // we clear the cache for this specific node right after it's inserted, so
    // that its fields are available to pass to the back-end.
    cache_clear_all('field:node:' . $this->getNid(), 'cache_field', TRUE);
  }

}
