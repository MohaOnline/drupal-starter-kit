<?php

/**
 * Class WetKitMigrateBean
 */
class WetKitMigrateBeanSlideShow extends XMLMigration {
  /**
   * Constructor
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->description = t('Migrate Bean Slideshow Entities into the Web Experience Toolkit');

    // Create a map object for tracking the relationships between source rows.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Bean Slideshow ID',
        ),
      ),
      MigrateDestinationBean::getKeySchema('bean')
    );

    // Source fields available in the XML file.
    $fields = array(
      'id' => t('Source id'),
      'title' => t('Title'),
      'body' => t('Description'),
    );

    $xml_folder = DRUPAL_ROOT . '/' . drupal_get_path('module', 'wetkit_migrate') . '/migrations/bean';
    $items_url = $xml_folder . '/migrate_bean_slideshow.xml';
    $item_xpath = '/bean/*/content';
    $item_id_xpath = 'source_id';

    $items_class = new MigrateItemsXML($items_url, $item_xpath, $item_id_xpath);
    $this->source = new MigrateSourceMultiItems($items_class, $fields);

    $language = 'en';
    $this->destination = new MigrateDestinationBean($this->getBundle(), array('language' => $language));

    $this->addFieldMapping('title', 'title')
         ->defaultValue('');
    $this->addFieldMapping('title_field', 'title')
         ->defaultValue('');
    $this->addFieldMapping('label', 'label')
         ->xpath('title');

    // Bean SlideOut settings.
    $this->addFieldMapping('image_style', 'image_style');
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
   * Prepare some of the rows which require extra logic before can be mapped.
   */
  public function prepare($entity, stdClass $row) {
    // Default Language.
    $entity->language = 'en';

    // Language of Migrated content.
    $language = $row->language;

    // Title Module support.
    if (module_exists('title')) {
      $entity->title_field[$language][0]['value'] = '';
      $entity->title_field[$language][0]['format'] = 'plain_text';
      $entity->title_field[$language][0]['safe_value'] = '';
    }

    // Set some bean slideshow fields.
    $entity->data['image_style'] = '<none>';
    $entity->data['plugin_settings'] = array(
      'animate_effect' => 'fade',
      'autoplay' => 1,
      'rotate_speed' => 'slow',
      'carousel' => 'carousel-s2'
    );
    $entity->data['slideshow_plugin'] = 'wetkit_slide:tabbedinterface';

    $entity->image_style = '<none>';
    $entity->plugin_settings = array(
      'animate_effect' => 'fade',
      'autoplay' => 1,
      'rotate_speed' => 'slow',
      'carousel' => 'carousel-s2'
    );
    $entity->slideshow_plugin = 'wetkit_slide:tabbedinterface';

    // Map the French Content.
    if ($language == 'fr') {

      // Join Handling.
      $sourceid = preg_replace('#_fra_#', '_eng_', $row->sourceid);
      $entity->bid = db_query('SELECT b.destid1 FROM {migrate_map_wetkitmigratebeanslideshow} b WHERE b.sourceid1 = :sourceid', array(':sourceid' => $sourceid))->fetchfield();
      $entity->vid = db_query('SELECT b.vid FROM {bean} b WHERE b.bid = :beanid', array(':beanid' => $entity->bid))->fetchfield();

      // Set title with English title for defaults.
      $beans = bean_load($entity->bid, $entity->vid);
      $entity->title = $beans->title;
      $entity->label = $beans->label;
      $entity->delta = $beans->delta;

      // Hack for Bean Entities as only saving the bean once and not through
      // admin which doesn't handle saving additional entities to same id.
      $entity->is_new = FALSE;
      db_insert('entity_translation')
        ->fields(array(
          'entity_type' => 'bean',
          'entity_id' => $entity->bid,
          'revision_id' => '1',
          'language' => $language,
          'source' => 'en',
          'uid' => '1',
          'status' => '1',
          'translate' => '0',
          'created' => $beans->created,
          'changed' => $beans->changed,
        ))
        ->execute();

      // Entity Translation Handler.
      $entity->translations = (object) array(
        'original' => 'en',
        'data' => array(
          'en' => array(
            'entity_type' => 'bean',
            'entity_id' => $entity->bid,
            'language' => 'en',
            'source' => '',
            'uid' => '1',
            'status' => '1',
            'translate' => '0',
          ),
          'fr' => array(
            'entity_type' => 'bean',
            'entity_id' => $entity->bid,
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

  /**
   * Get entity type.
   */
  protected function getEntityType() {
    return 'bean';
  }

  /**
   * Get node bundle.
   */
  protected function getBundle() {
    return 'wetkit_slide';
  }
}
