<?php
/**
 * File for the ShareaholicContentSettings class.
 *
 * @package shareaholic
 */

/**
 * An interface to the Shareaholic Content Settings database table
 *
 * @package shareaholic
 */
class ShareaholicContentSettings {

  public static function schema() {
    $schema['shareaholic_content_settings'] = array(
      'description' => 'Stores shareaholic specific settings for nodes.',
      'fields' => array(
        'nid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'default' => 0,
          'description' => 'The {node}.nid to store settings.',
        ),
        'settings' => array(
          'type' => 'text',
          'size' => 'medium',
          'not null' => TRUE,
          'serialize' => TRUE,
          'description' => 'The settings object for a node',
        ),
      ),
      'primary key' => array('nid'),
      'foreign keys' => array(
        'dnv_node' => array(
          'table' => 'node',
          'columns' => array('nid' => 'nid'),
        ),
      ),
    );
    return $schema;
  }


  /**
   * Updates the content settings in the database
   * with values selected in the content type form
   *
   * @param Object $node the node object that has been updated
   */
  public static function update($node) {
    if (!db_table_exists('shareaholic_content_settings')) {
      return;
    }
    if (db_select('shareaholic_content_settings', 'settings')
        ->fields('settings')->condition('nid', $node->nid, '=')
        ->execute()->fetchAssoc()) {

      db_update('shareaholic_content_settings')
        ->fields(array(
          'settings' => self::serialize_settings($node),
        ))
        ->condition('nid', $node->nid)
        ->execute();
    }
    else {
      // If not found insert it as a new node
      self::insert($node);
    }
  }


  /**
   * Inserts the content settings for a node
   * in the database with values selected in the form
   *
   * @param Object $node the newly created node object
   */
  public static function insert($node) {
    if (!db_table_exists('shareaholic_content_settings')) {
      return;
    }
    if (isset($node->shareaholic_options)) {
      db_insert('shareaholic_content_settings')
        ->fields(array(
          'nid' => $node->nid,
          'settings' => self::serialize_settings($node),
        ))
        ->execute();
    }
  }



  /**
   * Load the content settings from the database
   * and attach to each node object
   *
   * @param Array $node The list of node objects
   * @param Array $types The list of content types from the available nodes
   */
  public static function load($nodes, $types) {
    if (!db_table_exists('shareaholic_content_settings')) {
      return;
    }

    $result = db_query('SELECT * FROM {shareaholic_content_settings} WHERE nid IN(:nids)', array(':nids' => array_keys($nodes)))->fetchAllAssoc('nid');

    foreach ($nodes as &$node) {
      if(isset($result[$node->nid]->settings)) {
        $settings = self::unserialize_settings($result[$node->nid]);
        $node->shareaholic_options['shareaholic_exclude_from_recommendations'] = isset($settings['exclude_from_recommendations']) && $settings['exclude_from_recommendations'];
        $node->shareaholic_options['shareaholic_hide_recommendations'] = isset($settings['hide_recommendations']) && $settings['hide_recommendations'];
        $node->shareaholic_options['shareaholic_hide_share_buttons'] = isset($settings['hide_share_buttons']) && $settings['hide_share_buttons'];
        $node->shareaholic_options['shareaholic_exclude_og_tags'] = isset($settings['exclude_og_tags']) && $settings['exclude_og_tags'];
      }
    }
  }

  /**
   * Implements hook_node_delete().
   *
   * Delete the content settings for a node
   * @param Object $node the node that will be deleted
   */
  public static function delete($node) {
    if (!db_table_exists('shareaholic_content_settings')) {
      return;
    }
    db_delete('shareaholic_content_settings')
      ->condition('nid', $node->nid)
      ->execute();
  }


  /**
   * Get a serialized version of the content settings
   *
   * @param Object $node The node with settings to serialize
   * @return String the string representation of the settings
   */
  private static function serialize_settings($node) {
    $settings = array(
      'exclude_from_recommendations' => $node->shareaholic_options['shareaholic_exclude_from_recommendations'],
      'hide_recommendations' => $node->shareaholic_options['shareaholic_hide_recommendations'],
      'hide_share_buttons' => $node->shareaholic_options['shareaholic_hide_share_buttons'],
      'exclude_og_tags' => $node->shareaholic_options['shareaholic_exclude_og_tags'],
    );

    return serialize($settings);
  }


  /**
   * Get an unserialized version of the content settings
   *
   * @param Object $node The node with settings to serialize
   * @return String the string representation of the settings
   */
  private static function unserialize_settings($node) {
    return unserialize($node->settings);
  }

}