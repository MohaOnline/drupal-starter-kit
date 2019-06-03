<?php

namespace Drupal\campaignion_manage\Query;

class Content extends Base {
  protected $langs;
  protected $paged;

  public function __construct() {
    $query = db_select('node', 'n');
    $query->addExpression('IF(n.tnid=0, n.nid, n.tnid)', 'tset');
    $query->fields('n', array('nid', 'tnid', 'title', 'type', 'language', 'status', 'uid'));
    $query->innerJoin('users', 'u', 'u.uid=n.uid');
    $query->addField('u', 'name');
    $query->condition('n.type', 'thank_you_page', '!=')
      ->orderBy('n.changed', 'DESC');

    parent::__construct($query);

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

  protected function pagerQuery() {
    $query = db_select('node', 'n');
    $query->addExpression('IF(n.tnid=0, n.nid, n.tnid)', 'tset');
    $query->addExpression('MAX(n.changed)', 'changed');
    $query->addField('n', 'tnid');
    $query->condition('n.type', 'thank_you_page', '!=')
      ->groupBy('tset')
      ->orderBy('changed', 'DESC');
    return $query;
  }

  public function reset() {
    parent::reset();
    $this->paged = $this->pagerQuery();
  }

  public function filter($form) {
    parent::filter($form);
    // Paged is completely separate from other queries - so we need to apply
    // filters there too.
    $form->applyFilters($this->paged);
  }

  public function paged() {
    $paged = clone $this->paged;
    $paged = $paged->extend('PagerDefault')->limit($this->size);
    return $paged;
  }

  public function modifyResult(&$rows) {
    if (empty($rows)) {
      return;
    }
    $nids = array();
    foreach ($rows as $row) {
      $nids[$row->tset] = $row->tset;
    }
    $query = clone $this->filtered;
    $or = db_or();
    $or->condition('n.tnid', $nids, 'IN');
    $or->condition('n.nid', $nids, 'IN');
    $query->condition($or);
    $result = $query->execute();
    $tsets = array();
    foreach ($result as $row) {
      $tsets[$row->tset][$row->language] = $row;
    }
    foreach ($rows as $index => $row) {
      $tset = $row->tset;
      $rows[$index] = $this->buildTset($tsets[$tset]);
    }
  }

  protected function buildTset($nodes) {
    reset($nodes);
    $lc = key($nodes);
    foreach ($this->langs as $test_lc) {
      if (isset($nodes[$test_lc])) {
        $lc = $test_lc;
        break;
      }
    }
    $node = $nodes[$lc];
    unset($nodes[$lc]);
    $node->translations = $nodes;
    return $node;
  }
}
