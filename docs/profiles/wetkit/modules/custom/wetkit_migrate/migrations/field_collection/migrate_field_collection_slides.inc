<?php

/**
 * Class WetKitMigrateFieldCollectionSlides
 */
class WetKitMigrateFldCollectionSlides extends XMLMigration {
  /**
   * Constructor
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->description = t('Migrate Field Collection Slides into the Web Experience Toolkit');
    $this->dependencies = array('WetKitMigrateBeanSlideShow', 'WetKitMigrateFileBeanSlideShow');

    // Create a map object for tracking the relationships between source rows.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Field Collection ID',
        ),
      ),
      MigrateDestinationFieldCollection::getKeySchema()
    );

   // Source fields available in the XML file.
    $fields = array(
      'id' => t('Source id'),
      'host_entity_id' => t('Host Entity ID'),
      'slide_headline' => t('Slide Headline'),
      'slide_teaser' => t('Slide Teaser'),
    );

    $xml_folder = DRUPAL_ROOT . '/' . drupal_get_path('module', 'wetkit_migrate') . '/migrations/field_collection';
    $items_url = $xml_folder . '/migrate_field_collection_slides.xml';
    $item_xpath = '/bean/*/content';
    $item_id_xpath = 'source_id';

    $items_class = new MigrateItemsXML($items_url, $item_xpath, $item_id_xpath);
    $this->source = new MigrateSourceMultiItems($items_class, $fields);

    $this->destination = new MigrateDestinationFieldCollection(
      'field_bean_slide_collection',
      array('host_entity_type' => 'bean')
    );

    $this->addFieldMapping('host_entity_id', 'host_entity_id')
         ->xpath('host_entity_id')
         ->callbacks(array($this, '_wetkit_migrate_bean_slideshow_id'));
    $this->addFieldMapping('field_slide_image', 'id')
         ->xpath('image/media_field/key')
         ->sourceMigration('WetKitMigrateFileBeanSlideShow');
    $this->addFieldMapping('field_slide_image:file_class')
         ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_slide_headline', 'slide_headline')
         ->xpath('slide_headline');
    $this->addFieldMapping('field_slide_teaser', 'slide_teaser')
         ->xpath('slide_teaser');
    $this->addFieldMapping('field_slide_link:title', 'slide_title')
         ->xpath('link/link_title');
    $this->addFieldMapping('field_slide_link', 'slide_link')
         ->xpath('link/link_url');
  }

  /**
   * Prepare and modify the source rows.
   *
   * @param array $row
   *   Row to import for the migration.
   */
  public function prepareRow($row) {
    // Set the Language.
    $lang = (preg_match("/eng/i", $row->sourceid)) ? 'en' : 'fr';
    $row->language = $lang;
  }

  /**
   * Implements Migration::complete();
   */
  public function prepare($entity, stdClass $row) {

    // Set some source fields.
    $entity->field_slide_headline['und'][0]['value'] = $row->slide_headline;
    $entity->field_slide_headline['und'][0]['format'] = 'plain_text';
    $entity->field_slide_headline['und'][0]['safe_value'] = $row->slide_headline;

    $entity->field_slide_teaser['und'][0]['value'] = $row->slide_teaser;
    $entity->field_slide_teaser['und'][0]['format'] = 'wetkit_wysiwyg_text';
    $entity->field_slide_teaser['und'][0]['safe_value'] = $row->slide_teaser;

    return $entity;
  }

  public function _wetkit_migrate_bean_slideshow_id($id) {
    $result = db_query('SELECT b.destid1 FROM {migrate_map_wetkitmigratebeanslideshow} b WHERE b.sourceid1 = :sourceid1', array(':sourceid1' => $id));
    foreach ($result as $record) {
      return $record->destid1;
    }
  }
}
