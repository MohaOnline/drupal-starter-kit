<?php

namespace Drupal\campaignion_manage\Filter;

class ContentNodeReference extends Base implements FilterInterface {
  protected $query;
  protected $referenceField;
  protected $referenceColumn;
  protected $langs;

  public function __construct(\SelectQueryInterface $query, $reference_field, $reference_column) {
    $this->query           = $query;
    $this->referenceField  = $reference_field;
    $this->referenceColumn = $reference_column;
    // build the language preference list:
    // 1.) get the actual page language
    $this->langs[] = $GLOBALS['language']->language;
    // 2.) if the user is logged in, get the users preferred language
    if (!empty($GLOBALS['user']->language)) {
      $this->langs[] = $GLOBALS['user']->language;
    }
    // 3.) get the site default language
    $this->langs[] = language_default()->language;
  }

  protected function getOptions() {
    $query = clone $this->query;
    $fields =& $query->getFields();
    $fields = array();
    // join the field table that has the nid for the referenced node
    $query->innerJoin('field_data_' . $this->referenceField, 'ref', 'ref.entity_id = n.nid OR ref.entity_id = n.tnid');
    // join the node table for the referenced node
    $query->innerJoin('node', 'cn', 'ref.' . $this->referenceColumn . ' = cn.nid');
    // join the node table to build a complete translation set for the referenced node
    $query->innerJoin('node', 'tn', 'tn.nid=cn.nid OR (cn.tnid<>0 AND tn.tnid=cn.tnid)');
    $query->addExpression('IF(tn.tnid = 0, tn.nid, tn.tnid)', 'tset_ref');
    $fields = array(
      'nid' => array(
        'field' => 'nid',
        'table' => 'tn',
        'alias' => 'nid',
      ),
      'title' => array(
        'field' => 'title',
        'table' => 'tn',
        'alias' => 'title',
      ),
      'language' => array(
        'field' => 'language',
        'table' => 'tn',
        'alias' => 'language',
      ),
    );
    $query->groupBy('tn.nid');
    $tset_result = array();
    // build result as a translation set structure with
    // array[orig_nid][language][nid, title, language, tset_ref]
    foreach ($query->execute()->fetchAll() as $set) {
      $tset_result[$set->tset_ref][$set->language] = $set;
    }
    $result = array();
    // for each matching translation set, get the entry
    // that matches the language preference best
    foreach ($tset_result as $orig_nid => $set) {
      $node = NULL;
      foreach ($this->langs as $langcode) {
        if (isset($set[$langcode])) {
          $node = $set[$langcode];
          break;
        }
      }
      // the translation set has no matching entry for one of the
      // preferred languages; so we just get the next best entry
      if (!$node) {
        $node = array_shift($set);
      }
      $result[$node->nid] = $node->title;
    }
    return $result;
  }

  public function formElement(array &$form, array &$form_state, array &$values) {
    $form['nid'] = array(
      '#type'          => 'select',
      '#title'         => t('Node Reference'),
      '#options'       => $this->getOptions(),
      '#default_value' => isset($values) ? $values : NULL,
    );
  }
  public function title() { return t('Node Reference'); }
  public function apply($query, array $values) {
    // the user selected a referenced node, we get the nid of the node
    // via $values['nid']; we now build a list of nids for the complete translation set
    // for this nid
    $ref_nids = db_query(
      'SELECT tr.nid ' .
      '  FROM {node} n ' .
      '  INNER JOIN {node} tr ON IF(tr.tnid=0, tr.nid, tr.tnid) = IF(n.tnid=0, n.nid, n.tnid) ' .
      '    WHERE n.nid = :ref_nid ' ,
      array(':ref_nid' => $values['nid'])
    )->fetchCol();
    $alias = $query->innerJoin('field_data_' . $this->referenceField, 'ref', 'ref.entity_id = n.nid');
    // we filter for all nodes where the node reference has nid matching the previously build
    // set of translation set nids
    $query->condition($alias . '.' . $this->referenceColumn, $ref_nids, 'IN');
  }
  public function isApplicable($current) {
    return empty($current) && count($this->getOptions()) > 0;
  }
  public function defaults() {
    $options = $this->getOptions();
    return array('nid' => key($options));
  }
}

