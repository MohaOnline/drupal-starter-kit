<?php
/**
 * @file
 * migrate_documentation.inc
 */


/**
 * Abstract Migration class for HTML Content for WET-BOEW.
 */
abstract class WetKitDocMigration extends Migration {

  /**
   * ProcessImport for Migration.
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
 * Migration class for WET-BOEW
 */
class WetKitMigrateDocumentation extends WetKitDocMigration {

  /**
   * Implements the migration constructor.
   */
  public function __construct($arguments) {

    parent::__construct($arguments);

    $this->description = t('Import html files for WET-BOEW');

    // Based on the current theme choose the correct
    // documentation to migrate.
    $wxt_active = variable_get('wetkit_wetboew_theme', 'theme-wet-boew');
    $library_path = libraries_get_path($wxt_active, TRUE);

    $directories = array(
      $library_path . '/demos',
    );
    $base_dir = $library_path . '/demos';
    $file_mask = '/(.*en\.htm$|.*en\.html|.*fr\.htm$|.*fr\.html$)/i';

    $options = array(
      'nomask' => '/(\.\.?|CVS)$/',
      'callback' => 0,
      'recurse' => FALSE,
      'key' => 'uri',
      'min_depth' => 0,
    );

    $list = new MigrateListFiles($directories, $base_dir, $file_mask, $options);
    $item = new MigrateItemFile($base_dir);

    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'sourceid' => array(
            'type' => 'varchar',
            'length' => 255,
            'not null' => TRUE,
            'description' => 'Documentation ID',
          ),
        ),
        MigrateDestinationNode::getKeySchema()
    );

    $fields = array(
      'title' => t('Title'),
      'title_field' => t('Title Field'),
      'body' => t('Body'),
      'nid' => t('nid'),
      'field_legacy_id' => t('Legacy ID'),
    );
    $this->source = new MigrateSourceList($list, $item, $fields);

    $language = 'en';
    $text_format = 'wetkit_wysiwyg_text';
    $node_options = MigrateDestinationNode::options($language, $text_format);
    $this->destination = new MigrateDestinationNode('wetkit_documentation', $node_options);

    $this->addFieldMapping('title_field', 'title_field')
      ->description(t('Title Field for Content Link'));
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('uid', 'uid')
      ->defaultValue('1');
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('language', 'language');
    $this->addFieldMapping('path', 'path');
    $this->addFieldMapping('nid', 'nid');
    $this->addFieldMapping('status', 'status')
      ->defaultValue('1');
    $this->addFieldMapping('promote', 'promotion');
    $this->addFieldMapping('created', 'datestamp');
    $this->addFieldMapping('changed', 'datestamp');

    // Indicate which fields are explictly ignored.
    $this->addUnmigratedDestinations(
      array(
        'sticky',
        'revision',
        'log',
        'revision_uid',
        'is_new',
        'body:summary',
        'body:format',
        'body:language',
      ));
  }

  /**
   * Prepare and modify the source rows.
   *
   * @param array $row
   *   Row to import for the migration.
   */
  public function prepareRow($row) {
    // Set the Language.
    $withoutExt = preg_replace("/\\.[^.\\s]{3,4}$/", "", $row->sourceid);
    $lang = (substr($withoutExt, -2) == 'fr') ? 'fr' : 'en';
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

    // Leverage the power of QueryPath.
    if (isset($this->elements_to_remove)) {
      $htmlpage = new WetKitMigrationhtmlpage($row, $this->elements_to_remove);
    }
    else {
      $htmlpage = new WetKitMigrationhtmlpage($row);
    }

    // If the title is empty, don't import this row.
    if (strlen(trim($htmlpage->title)) == 0) {
      throw new MigrateException(t("No title (title) detected for: @sourceid", array('@sourceid' => $row->sourceid)), Migration::MESSAGE_NOTICE, MigrateMap::STATUS_IGNORED);
    }

    // If the body is empty, don't import this row.
    if (strlen(trim($htmlpage->body)) == 0) {
      $htmlpage->body = $htmlpage->getHTML('.colLayout .center');
      if (strlen(trim($htmlpage->body)) == 0) {
        throw new MigrateException(t("No body (#wb-main-in || .colLayout .center) detected for: @sourceid", array('@sourceid' => $row->sourceid)), Migration::MESSAGE_NOTICE, MigrateMap::STATUS_IGNORED);
      }
    }

    // Title Module support.
    if (module_exists('title')) {
      $entity->title = $htmlpage->title;
      $entity->title_field[$language][0]['value'] = $htmlpage->title;
      $entity->title_field[$language][0]['format'] = 'plain_text';
      $entity->title_field[$language][0]['safe_value'] = $htmlpage->title;
    }

    // Links Parser.
    $basepath = '/' . variable_get('file_public_path', conf_path() . '/files');
    $linksParser = new LinksParser($htmlpage->body, $basepath, 'migrated');
    $htmlpage->body = $linksParser->process();

    // Body field.
    $entity->body[$language][0]['value'] = $htmlpage->body;
    $entity->body[$language][0]['format'] = 'wetkit_wysiwyg_text';
    $entity->body[$language][0]['safe_value'] = $htmlpage->body;

    // Map the French Content.
    if ($language == 'fr') {

      // Join Handling.
      $sourceid = preg_replace('#-fr#', '-en', $row->sourceid);
      $entity->nid = db_query('SELECT n.destid1 FROM {migrate_map_wetkitmigratedocumentation} n WHERE n.sourceid1 = :sourceid', array(':sourceid' => $sourceid))->fetchfield();
      $entity->vid = db_query('SELECT n.vid FROM {node} n WHERE n.nid = :nodeid', array(':nodeid' => $entity->nid))->fetchfield();

      // Set title with English title for defaults.
      $nodes = node_load($entity->nid, $entity->vid);
      $entity->title = $nodes->title;

      // Entity Translation Handler.
      $entity->translations = (object) array(
        'original' => 'en',
        'data' => array(
          'en' => array(
            'entity_type' => 'node',
            'entity_id' => $entity->nid,
            'language' => 'en',
            'source' => '',
            'uid' => '0',
            'status' => '1',
            'translate' => '0',
          ),
          'fr' => array(
            'entity_type' => 'node',
            'entity_id' => $entity->nid,
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
