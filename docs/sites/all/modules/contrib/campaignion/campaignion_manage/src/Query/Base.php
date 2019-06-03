<?php

namespace Drupal\campaignion_manage\Query;

abstract class Base {
  protected $query;
  protected $filtered;
  protected $size;

  public function __construct(\SelectQuery $query) {
    $this->query = $query;
    $this->reset();
  }

  public function execute() {
    $rows = $this->paged()->execute()->fetchAll();
    $this->modifyResult($rows);
    return $rows;
  }

  public function setPage($size) {
    $this->size = $size;
  }

  public function reset() {
    $this->filtered = clone $this->query;
  }

  public function modifyResult(&$rows) {
  }

  public function ensureTable($alias) {
  }

  public function query() {
    return clone $this->query;
  }

  public function filtered() {
    return clone $this->filtered;
  }

  public function paged() {
    $paged = clone $this->filtered;
    $paged = $paged->extend('PagerDefault')->limit($this->size);
    return $paged;
  }

  public function count() {
    return $this->filtered()->countQuery()->execute()->fetchField();
  }

  public function filter($form) {
    $this->reset();
    $form->applyFilters($this->filtered);
  }
}
