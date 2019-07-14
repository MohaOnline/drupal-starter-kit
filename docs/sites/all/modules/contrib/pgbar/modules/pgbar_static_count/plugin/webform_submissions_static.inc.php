<?php
/**
 * @file
 * Define the webform submission static source plugin.
 */

$plugin = array(
  'label'   => t('Webform Submissions Static'),
  'handler' => array('class' => 'PgbarSourceWebformSubmissionsStatic'),
);

class PgbarSourceWebformSubmissionsStatic {
  /**
   * Constructor: save entity, field and field_instance.
   */
  public function __construct($entity, $field, $instance) {
    $this->entity   = $entity;
    $this->field    = $field;
    $this->instance = $instance;
  }

  /**
   * Get the value for the given item.
   *
   * @return int
   *   The number of static (remembering deleted submissions) webform submissions
   *   for the node referrenced in $this->entity and all it's translations.
   */
  public function getValue($item) {
    $node = $this->entity;

    return db_query(
      'SELECT SUM(wss.count) ' .
      '  FROM {pgbar_webform_submissions_static} wss' .
      '  INNER JOIN {node} n USING(nid) ' .
      '    WHERE n.nid = :nid ' .
      '    OR ((n.nid = :tnid OR n.tnid = :tnid) AND :tnid > 0) ' ,
      array(':nid' => $node->nid, ':tnid' => $node->tnid)
    )->fetchField();
  }

  /**
   * No extra configuration for the widget needed.
   */
  public function widgetForm($item) {
    return NULL;
  }
}