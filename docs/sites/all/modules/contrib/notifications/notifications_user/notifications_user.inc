<?php
/**
 * @file
 * User subscriptions for Notifications
 */

/**
 * Generic content subscription Thread subscription.
 */
class Notifications_User_Content_Subscription extends Notifications_Content_Subscription {
  /**
   * Set all the fields we can from node.
   */
  public function set_node($node) {
    parent::set_node($node);
    $this->set_author($node->uid);
    return $this;
  }
  /**
   * Set author (user).
   */
  public function set_author($user) {
    $uid = is_object($user) ? $user->uid : $user;
    $this->get_field('node:uid')->set_value($uid);
    return $this;
  }
  /**
   * Get author (user field).
   */
  public function get_author() {
    return $this->get_field('node:uid');
  }
  /**
   * Get name.
   */
  function get_name() {
    if (isset($this->name)) {
      return $this->name;
    }
    else {
      $author_name = $this->get_author()->get_name();
      if ($type = $this->get_field('node:type')) {
        return t('@type posts by @author', array('@type' => $type->get_name(), '@author' => $author_name));
      }
      else {
        return t('All posts by @author', array('@author' => $author_name));
      }
    }
  }
}

/**
 * Class Notifications_Node_Author_Field.
 */
class Notifications_Node_Author_Field extends Notifications_Field_Default {
  public $type = 'user:uid';
  public $field = 'uid';
  public $object_type = 'node';

  /**
   * Get field name.
   */
  function get_title() {
    return t('Node: Author');
  }

  /**
   * Get description.
   */
  function get_description() {
    return t('Author of a node');
  }

  /**
   * Get object.
   */
  function get_object() {
    $node = node_load($this->value);
    $uid = isset($node->uid) ? $node->uid : NULL;
    return notifications_object('user', $uid);
  }

  /**
   * Get object value.
   */
  function object_value($object) {
    if ($object->type == $this->object_type) {
      $object = $object->get_object();
      if ($object) {
        return $object->uid;
      }
    }
  }
}
