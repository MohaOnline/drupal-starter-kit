<?php
/**
 * Handle a WSField filtering.
 *
 * @ingroup views_filter_handlers
 */

class wsfields_views_handler_filter extends views_handler_filter_string {
  function query() {
    // Override query function so nothing happens.
  }

  function post_execute(&$values) {
    $field = substr($this->field, 0, -1 * strlen('_filter'));
    $entity_type = $this->table;
    $entity_info = entity_get_info($entity_type);
    $key = $entity_info['entity keys']['id'];

    $map = array();
    $ids = array();
    foreach ($values as $location => $value) {
      $ids[] = $value->$key;
      $map[$value->$key] = $location;
    }

    $entities = entity_load($entity_type, $ids);
    $index = array();
    foreach ($entities as $id => $entity) {
      $lang = current($entity->$field);
      $delta = current($lang);
      if ($this->filter_out($delta['value'])) {
        unset($values[$map[$id]]);
      }
    }
  }

  function filter_out($value) {
    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      return $this->{$info[$this->operator]['method']}($value);
    }
    return FALSE;
  }

  function op_equal($value) {
    return $this->value == $value;
  }

  function op_contains($value) {
    return stripos($value, $this->value);
  }

  function op_word($value) {
    return preg_match("/\$this->value\b/i", $value);
  }

  function op_starts($field) {
    return stripos($value, $this->value) === 0;
  }

  function op_not_starts($value) {
    return stripos($value, $this->value) !== 0;
  }

  function op_ends($value) {
    if (strlen($value) > strlen($this->value)) {
      return FALSE;
    }
    $end = substr($value, strlen($this->value));
    return $this->value == $end;
  }

  function op_not_ends($value) {
    return !$this->op_ends($value);
  }

  function op_not($value) {
    return !$this->op_contains($value);
  }

  function op_shorter($value) {
    if (strlen($value) < strlen($this->value)) {
      return TRUE;
    }
    return FALSE;
  }

  function op_longer($value) {
    return !$this->op_contains($value);
  }

  function op_regex($value) {
    return preg_match($this->value, $value);
  }

  function op_empty($value) {
    if ($this->operator == 'empty') {
      return $value == "";
    }
    else {
      return $value != "";
    }
  }
}
