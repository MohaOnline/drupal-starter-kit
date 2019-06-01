<?php

/**
 * Class WetKitMigrateBean
 */
class WetKitMigrateBeanSlideOut extends XMLMigration {
  /**
   * Constructor
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->description = t('Migrate Bean Entities into the Web Experience Toolkit');

    // Create a map object for tracking the relationships between source rows.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Bean Slideout ID',
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
    $items_url = $xml_folder . '/migrate_bean_slideout.xml';
    $item_xpath = '/bean/*/content';
    $item_id_xpath = 'source_id';

    $items_class = new MigrateItemsXML($items_url, $item_xpath, $item_id_xpath);
    $this->source = new MigrateSourceMultiItems($items_class, $fields);

    $language = 'en';
    $this->destination = new MigrateDestinationBean($this->getBundle(), array('language' => $language));

    $this->addFieldMapping('title', 'title')
         ->xpath('title');
    $this->addFieldMapping('title_field', 'title')
         ->xpath('title');
    $this->addFieldMapping('label', 'label')
         ->xpath('title');

    // Fields.
    $this->addFieldMapping('field_bean_wetkit_body', 'body')
         ->xpath('body');
    $this->addFieldMapping('field_bean_icon', 'icon_bundle')
         ->xpath('icon_bundle');
    $this->addFieldMapping('field_bean_icon:icon', 'icon_icon')
         ->xpath('icon_icon');

    // Bean SlideOut settings.
    $this->addFieldMapping('slideout_widget', 'slideout_widget')
         ->xpath('slideout_widget');
    $this->addFieldMapping('slideout_form', 'slideout_form')
         ->xpath('slideout_form');
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
      $entity->title_field[$language][0]['value'] = $row->title;
      $entity->title_field[$language][0]['format'] = 'plain_text';
      $entity->title_field[$language][0]['safe_value'] = $row->title;
    }

    // Set some source fields.
    $entity->field_bean_wetkit_body[$language][0]['value'] = $row->body;
    $entity->field_bean_wetkit_body[$language][0]['format'] = 'wetkit_wysiwyg_text';
    $entity->field_bean_wetkit_body[$language][0]['safe_value'] = $row->body;

    // Set some bean slideout fields.
    $entity->data['view_mode'] = 'default';
    $entity->data['slideout_settings']['configure_slideout_widget'] = $row->slideout_widget;
    $entity->data['slideout_settings']['configure_slideout_form'] = $row->slideout_form;

    $entity->slideout_settings['configure_slideout_widget'] = $row->slideout_widget;
    $entity->slideout_settings['configure_slideout_form'] = $row->slideout_form;

    // If the title is empty, don't import this row.
    if (strlen(trim($entity->title_field[$language][0]['value'])) == 0) {
      throw new MigrateException(t("No title detected for: @sourceid", array('@sourceid' => $row->sourceid)), Migration::MESSAGE_NOTICE, MigrateMap::STATUS_IGNORED);
    }

    return $entity;
  }

  /**
   * Implements Migration::complete();
   */
  public function complete($entity, stdClass $row) {

    $machine_readable = strtolower($row->label);
    $machine_readable = preg_replace('@[^a-z0-9_]+@','-',$machine_readable);

    // Place Bean block
    $block = array(
        'module' => 'bean',
        'delta' => $machine_readable,
        'theme' => 'wetkit_bootstrap',
        'status' => 1,
        'weight' => -33,
        'region' => 'content',
        'pages' => '',
        'visibility' => 0,
        'title' => '<none>',
        'cache' => DRUPAL_CACHE_GLOBAL,
    );

    db_merge('block')
    ->key(
      array(
        'module' => 'bean',
        'delta' => $machine_readable,
        'theme' => 'wetkit_bootstrap',
      )
    )
    ->fields(array_keys($block), $block)
    ->execute();
  }

  /**
   * Implements Migration::prepareRollback();
   */
  public function prepareRollback(array $ids) {
    $result = db_query('SELECT b.destid2
      FROM {migrate_map_wetkitmigratebeanslideout} b WHERE b.destid1 = :destid', array(':destid' => ($ids)));
    foreach ($result as $record) {
      $bean_id = $record->destid2;
      $result = db_query('SELECT b.delta
        FROM {bean} b WHERE b.vid = :vid', array(':vid' => $bean_id));
      foreach ($result as $result_row) {
        db_delete('block')
          ->condition('module', 'bean')
          ->condition('delta', $result_row->delta)
          ->execute();
      }
    }
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
    return 'wetkit_slideout';
  }
}
