<?php
/**
 * @file
 * migrate_default_content.inc
 */

/**
 * Migration class for media images.
 */
class WetKitMigrateFileBeanSlideShow extends XMLMigration {

  /**
   * Implements the migration constructor.
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->description = t('Migrate Media (File Entities) into the Web Experience Toolkit');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'filename' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'File Entity ID',
        )
      ),
      MigrateDestinationMedia::getKeySchema()
    );

    // Source fields available in the XML file.
    $fields = array(
      'source_id' => t('Source id'),
      'title' => t('Title'),
      'body' => t('Description'),
      'image' => t('Image filename, relative to the source directory'),
      'image_alt' => t('Alt text for the image'),
    );

    $xml_folder = DRUPAL_ROOT . '/' . drupal_get_path('module', 'wetkit_migrate') . '/migrations/field_collection';
    $items_url = $xml_folder . '/migrate_field_collection_slides.xml';
    $item_xpath = '/bean/*/content/image/*';
    $item_id_xpath = 'key';

    $items_class = new MigrateItemsXML($items_url, $item_xpath, $item_id_xpath);
    $this->source = new MigrateSourceMultiItems($items_class, $fields);

    // Media Destination Migration.
    $this->destination = new MigrateDestinationMedia('image');

    // The source images are in a local directory - specify the parent.
    $this->addFieldMapping('source_dir')
      ->defaultValue(drupal_get_path('module', 'wetkit_migrate') . '/migrations/field_collection/images');

    // The 'value' of the media destination is mapped to the source field
    // representing the media itself - in this case, a filename relative to
    // source_dir.
    $this->addFieldMapping('value', 'image')
         ->xpath('filename');
    // Fields on the entity can be mapped in the usual way.
    $this->addFieldMapping('field_file_image_alt_text', 'image_alt')
         ->xpath('alt');
    $this->addFieldMapping('field_file_image_alt_text:language', 'language_alt');
    $this->addFieldMapping('file_replace')
         ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('uid')
         ->defaultValue(1);

    // Unmapped destination fields.
    $this->addUnmigratedDestinations(array(
      'destination_dir',
      'destination_file',
      'preserve_files',
      'timestamp',
    ));

    // Remove path if set.
    if (module_exists('path')) {
      $this->addUnmigratedDestinations(array('path'));
    }
  }

  /**
   * Prepare and modify the source rows.
   *
   * @param array $row
   *   Row to import for the migration.
   */
  public function prepareRow($row) {
    // Prepare alt text for image.
    $xmlobj = $row->xml;
    $key = strip_tags($xmlobj->key->asXML());

    $row->key = $key;
    $row->language = (preg_match("/eng/i", $key)) ? 'en' : 'fr';
    $row->language_alt = $row->language;
  }

  /**
   * Prepare some of the rows which require extra logic before can be mapped.
   */
  public function prepare($entity, stdClass $row) {
    // Default Language.
    $entity->language = 'en';

    // Language of Migrated content.
    $language = $row->language;

    // Entity Translation name_field support.
    $entity->filename_field[$language][0]['value'] = $row->image;
    $entity->filename_field[$language][0]['format'] = 'plain_text';
    $entity->filename_field[$language][0]['safe_value'] = $row->image;

    // Entity Translation alt tag support.
    $entity->field_file_image_alt_text[$language][0]['value'] = $row->image_alt;
    $entity->field_file_image_alt_text[$language][0]['format'] = 'plain_text';
    $entity->field_file_image_alt_text[$language][0]['safe_value'] = $row->image_alt;

    // Map the French Content.
    if ($language == 'fr') {

      // Join Handling.
      $source_id = preg_replace('#_fra_#', '_eng_', $row->key);
      $entity->fid = db_query('SELECT n.destid1 FROM {migrate_map_wetkitmigratefilebeanslideshow} n WHERE n.sourceid1 = :sourceid', array(':sourceid' => $source_id))->fetchfield();

      // Entity Translation Handler.
      $entity->translations = (object) array(
        'original' => 'en',
        'data' => array(
          'en' => array(
            'entity_type' => 'file',
            'entity_id' => $entity->fid,
            'language' => 'en',
            'source' => '',
            'uid' => '0',
            'status' => '1',
            'translate' => '0',
          ),
          'fr' => array(
            'entity_type' => 'file',
            'entity_id' => $entity->fid,
            'language' => 'fr',
            'source' => 'en',
            'uid' => '1',
            'status' => '1',
            'translate' => '0',
          ),
        ),
      );
    }
    return $entity;
  }
}

