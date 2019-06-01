<?php
/**
 * @file
 * Contains UUIDFieldCollectionItemEntity.
 */

/**
 * Overrides save functionality from FieldCollectionItemEntity.
 *
 * Avoid exceptions when deploying a field_collection entity before its host
 * entity exists.
 */
class UUIDFieldCollectionItemEntity extends FieldCollectionItemEntity {

  /**
   * Save the field collection item.
   */

  public function save($skip_host_save = FALSE) {
    if (isset($this->hostEntity)) {
      // If we have a host entity save normally.
      parent::save($skip_host_save);
    }
    else {
      // If there is no host entity, we may be receiving it from deploy.
      // so save the item directly.
      $this->revision = TRUE;
      entity_get_controller($this->entityType)->save($this);
    }
  }
}
