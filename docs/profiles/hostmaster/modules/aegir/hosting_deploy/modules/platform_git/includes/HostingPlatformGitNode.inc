<?php

/**
 * @file
 * The HostingPlatformGitNode class.
 */

class HostingPlatformGitNode extends HostingNode {

  protected $info_fields = [
    'field_git_repository_url',
    'field_git_repository_path',
    'field_git_reference',
    'field_git_docroot',
  ];

  public function nodeInsert() {
    // Fields are cached empty when platform nodes are initially created. So,
    // we clear the cache for this specific node right after it's inserted, so
    // that its fields are available to pass to the back-end.
    cache_clear_all('field:node:' . $this->getNid(), 'cache_field', TRUE);
  }

}
