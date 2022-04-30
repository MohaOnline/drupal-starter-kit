<?php

namespace OpenlayersDrupal\Core\Database;

use Database as BaseDatabase;

/**
 * FIX - insert comment here.
 */
class Database {

  /**
   * FIX - insert comment here.
   */
  final public static function startLog($logging_key, $key = 'default') {
    return BaseDatabase::startLog($logging_key, $key);
  }

  /**
   * FIX - insert comment here.
   */
  final public static function getLog($logging_key, $key = 'default') {
    return BaseDatabase::getLog($logging_key, $key);
  }

  /**
   * FIX - insert comment here.
   *
   * @return \OpenlayersDrupal\Core\Database\Connection
   *   FIX - insert comment here.
   */
  final public static function getConnection($target = 'default', $key = NULL) {
    return new Connection(BaseDatabase::getConnection($target, $key));
  }

  /**
   * FIX - insert comment here.
   */
  final public static function isActiveConnection() {
    return BaseDatabase::isActiveConnection();
  }

  /**
   * FIX - insert comment here.
   */
  final public static function setActiveConnection($key = 'default') {
    return BaseDatabase::setActiveConnection($key);
  }

  /**
   * FIX - insert comment here.
   */
  final public static function parseConnectionInfo(array $info) {
    BaseDatabase::parseConnectionInfo();
  }

  /**
   * FIX - insert comment here.
   */
  final public static function addConnectionInfo($key, $target, array $info) {
    BaseDatabase::addConnectionInfo($key, $target, $info);
  }

  /**
   * FIX - insert comment here.
   */
  final public static function getConnectionInfo($key = 'default') {
    return BaseDatabase::getConnectionInfo($key);
  }

  /**
   * FIX - insert comment here.
   */
  final public static function getAllConnectionInfo() {
    throw new \Exception('not available/implemented in d7');
  }

  /**
   * FIX - insert comment here.
   */
  final public static function setMultipleConnectionInfo(array $databases) {
    throw new \Exception('not available/implemented yet in d7');
  }

  /**
   * FIX - insert comment here.
   */
  final public static function renameConnection($old_key, $new_key) {
    return BaseDatabase::getConnectionInfo($old_key, $new_key);
  }

  /**
   * FIX - insert comment here.
   */
  final public static function removeConnection($key) {
    return BaseDatabase::removeConnection($key);
  }

  /**
   * FIX - insert comment here.
   */
  final protected static function openConnection($key, $target) {
    throw new \Exception('not available/implemented yet in d7');
  }

  /**
   * FIX - insert comment here.
   */
  public static function closeConnection($target = NULL, $key = NULL) {
    throw new \Exception('not available/implemented yet in d7');
  }

  /**
   * FIX - insert comment here.
   */
  public static function ignoreTarget($key, $target) {
    BaseDatabase::ignoreTarget($key, $target);
  }

}
