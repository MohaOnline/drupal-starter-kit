<?php

namespace Drupal\pgbar_webform\Source;

use Drupal\pgbar\Source\AddNids;
use Drupal\pgbar\Source\PluginInterface;

/**
 * Pgbar source plugin that counts webform submissions.
 */
class WebformSubmissionCount implements PluginInterface {
  protected $entity;

  /**
   * @var Drupal\pgbar\Source\AddNids
   */
  protected $addNids;

  public static function label() {
    return t('Webform submission count');
  }

  public static function forField($entity, $field, $instance) {
    return new static($entity);
  }

  /**
   * Constructor: save entity, field and field_instance.
   */
  public function __construct($entity) {
    $this->entity = $entity;
    $this->addNids = new AddNids($entity ? $entity->nid : NULL);
  }

  /**
   * Get the value for the given item.
   *
   * @return int
   *   The number of webform submissions in $this-entity,
   *   and on additional nodes provided via the field widget,
   *   and all their translations.
   */
  public function getValue($item) {
    $nids = $this->addNids->translationsQuery($item)->execute()->fetchCol();
    $q = db_select('webform_submissions', 'ws');
    $q->addExpression('COUNT(ws.nid)');
    $q->condition('ws.nid', $nids, 'IN');
    $q->condition('ws.is_draft', 0);
    return $q->execute()->fetchField();
  }

  /**
   * Build the configuration form for the field widget.
   */
  public function widgetForm($item) {
    return $this->addNids->widgetForm($item);
  }

}
