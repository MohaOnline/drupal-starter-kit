<?php
/**
 * @file
 * taxonomy.inc
 */

/**
 * Abstract Migration class for Default Content for the Web Experience Toolkit.
 */
abstract class WetKitTaxMigration extends Migration {

  /**
   * ProcessImport for WetKit Migration.
   *
   * @param array $options
   *   Process the import array.
   */
  public function processImport(array $options = array()) {
    parent::processImport($options);
    // Do not force menu rebuilding. Otherwise pathauto will try to rebuild
    // in each node_insert invocation.
    variable_set('menu_rebuild_needed', FALSE);
  }
}

/**
 * Migration class for taxonomy.
 */
class WetKitMigrateTaxonomy extends WetKitTaxMigration {

  /**
   * Implements the migration constructor.
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->description = t('Migrate taxonomy for Drupal WxT.');

    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'sourceid' => array(
            'type' => 'int',
            'length' => 255,
            'not null' => TRUE,
            'description' => 'Taxonomy ID',
            ),
          ),
        MigrateDestinationTerm::getKeySchema()
      );

    // The source fields.
    $fields = array(
      'name_field' => t('Taxonomy Name'),
      'description' => t('Taxonomy Description'),
      'parent_name' => t('Taxonomy Parent Name'),
      'field_url' => t('Taxonomy URL'),
    );

    // Create a MigrateSource object.
    $this->source = new MigrateSourceCSV(drupal_get_path('module', 'wetkit_migrate') . '/migrations/taxonomy/taxonomy.csv', $this->csvcolumns(), array(), $fields);

    // Set the destination
    $this->destination = new MigrateDestinationTerm('wetkit_categories');

    // Assign mappings to destination fields from source fields.
    $this->addFieldMapping('name_field', 'name_field');
    $this->addFieldMapping('parent_name', 'parent_name');
    $this->addFieldMapping('pathauto')
         ->defaultValue(0);
  }

  /**
   * Process the file prior to the actual migration.
   */
  public function prepareRow($row) {
    // Set the Language.
    $lang = 'en';
    $row->language = $lang;
  }


  /**
   * Prepare some of the rows which require extra logic before can be mapped.
   */
  function prepare($entity, stdClass $row) {

    // Default Language.
    $entity->language = 'en';

    // Language of Migrated content.
    $language = $row->language;

    // Title Module support.
    if (module_exists('title')) {
      if (!empty($row->Taxonomy2)) {
      	$entity->name = $row->Taxonomy2;
        $entity->name_field[$language][0]['value'] = $row->Taxonomy2;
        $entity->name_field[$language][0]['format'] = 'plain_text';
        $entity->name_field[$language][0]['safe_value'] = $row->Taxonomy2;
      }
      else {
        $entity->name = $row->Taxonomy1;
        $entity->name_field[$language][0]['value'] = $row->Taxonomy1;
        $entity->name_field[$language][0]['format'] = 'plain_text';
        $entity->name_field[$language][0]['safe_value'] = $row->Taxonomy1;
      }
    }

    // Other Fields.
    if (!empty($row->Taxonomy2)) {
      // Look for the name in the same vocabulary.
      // Note that hierarchies may have multiples of the same name...
      $terms = taxonomy_term_load_multiple(array(), array('name' => trim($row->Taxonomy1)));
      $tids = array_keys($terms);
      $entity->parent = array($tids[0]);
    }

    // If the title is empty, don't import this row.
    if (strlen(trim($entity->name_field[$language][0]['value'])) == 0) {
      throw new MigrateException(t("No title detected for: @sourceid", array('@sourceid' => $row->sourceid)), Migration::MESSAGE_NOTICE, MigrateMap::STATUS_IGNORED);
    }

    return $entity;
  }

  /**
   * Process the csvcolumns.
   */
  function csvcolumns() {
    $columns[0] = array('sourceid', 'sourceid');
    $columns[1] = array('Taxonomy1', 'Taxonomy1');
    $columns[3] = array('Taxonomy2', 'Taxonomy2');
    return $columns;
  }
}
