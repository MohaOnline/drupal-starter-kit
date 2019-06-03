<?php
/**
 * @file The HostingFieldable trait.
 */

trait HostingFieldable {

  /**
   * Return the target of an entityreference field.
   */
  public function getEntityReference($field) {
    return $this->getFieldValueType($field, 'target_id');
  }

  public function getFieldValueType($field, $type) {
    if (isset($this->node->{$field}) && $this->node->{$field}) {
      if (isset($this->node->{$field}[$this->node->language])) {
        return $this->node->{$field}[$this->node->language][0][$type];
      }
      else {
        return $this->node->{$field}[0][$type];
      }
    }
  }

  /**
   * Return the target of an entityreference field.
   */
  public function setEntityReference($field, $value) {
    return $this->node->{$field}[$this->node->language][0] = array(
      'target_id' => $value,
      'target_type' => 'node',
    );
  }

  /**
   * Return the value of a field.
   */
  public function getFieldValue($field) {
    return $this->getFieldValueType($field, 'value');
  }

  /**
   * Set the value of a field.
   */
  public function setFieldValue($field, $value) {
    return $this->node->{$field}[$this->node->language][0]['value'] = $value;
  }

  /**
   * Set the value of a property.
   */
  public function setProperty($property, $value) {
    return $this->node->$property = $value;
  }



}
