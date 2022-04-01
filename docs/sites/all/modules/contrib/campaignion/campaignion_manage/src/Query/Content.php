<?php

namespace Drupal\campaignion_manage\Query;

class Content extends Base {
  protected $langs;
  protected $paged;

  public function __construct() {
    $query = db_select('node', 'n');
    $query->addExpression('IF(n.tnid=0, n.nid, n.tnid)', 'tset');
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
    $self = $this;
    if (empty($rows)) {
      return;
    }
    // The query gives us a list of transalationsets. Get all translations
    // for each of them.
    $nids = array();
    foreach ($rows as $row) {
      $nids[$row->tset] = $row->tset;
    }
    $query = db_select('node', 'n');
    $query->addField('n', 'nid');
    $or = db_or();
    $or->condition('n.tnid', $nids, 'IN');
    $or->condition('n.nid', $nids, 'IN');
    $query->condition($or);
    $nids = $query->execute()->fetchCol();
    $nodes = entity_load('node', $nids);

    // Group nodes into translationsets.
    $tsets = [];
    foreach ($nodes as $node) {
      $tsets[$node->tnid ?: $node->nid][$node->language] = $node;
    }
    $rows = array_map(function ($row) use ($self, $tsets) {
      return $this->buildTset($tsets[$row->tset]);
    }, $rows);
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
