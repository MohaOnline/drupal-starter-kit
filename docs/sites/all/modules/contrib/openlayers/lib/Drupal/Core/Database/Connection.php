<?php

namespace OpenlayersDrupal\Core\Database;

/**
 * FIX - insert comment here.
 */
class Connection {

  /**
   * FIX - insert comment here.
   *
   * @var \DatabaseConnection
   */
  protected $connection;

  /**
   * FIX - insert comment here.
   */
  public function __construct(\DatabaseConnection $connection) {
    $this->connection = $connection;
  }

  /**
   * FIX - insert comment here.
   */
  public static function open(array &$connection_options = array()) {
    throw new \Exception('not implemented yet');
  }

  /**
   * FIX - insert comment here.
   */
  public function destroy() {
    $this->connection->destroy();
  }

  /**
   * FIX - insert comment here.
   */
  public function getConnectionOptions() {
    return $this->connection->getConnectionOptions();
  }

  /**
   * FIX - insert comment here.
   */
  public function prefixTables($sql) {
    return $this->connection->prefixTables($sql);
  }

  /**
   * FIX - insert comment here.
   */
  public function tablePrefix($table = 'default') {
    return $this->connection->tablePrefix($table);
  }

  /**
   * FIX - insert comment here.
   */
  public function prepareQuery($query) {
    return $this->connection->prepareQuery($query);
  }

  /**
   * FIX - insert comment here.
   */
  public function setTarget($target = NULL) {
    return $this->connection->setTarget($target);
  }

  /**
   * FIX - insert comment here.
   */
  public function getTarget() {
    return $this->connection->getTarget();
  }

  /**
   * FIX - insert comment here.
   */
  public function setKey($key) {
    return $this->connection->setKey($key);
  }

  /**
   * FIX - insert comment here.
   */
  public function getKey() {
    return $this->connection->getKey();
  }

  /**
   * FIX - insert comment here.
   */
  public function setLogger(Log $logger) {
    return $this->connection->setLogger($logger);
  }

  /**
   * FIX - insert comment here.
   */
  public function getLogger() {
    return $this->connection->getLogger();
  }

  /**
   * FIX - insert comment here.
   */
  public function makeSequenceName($table, $field) {
    return $this->connection->makeSequenceName($table, $field);
  }

  /**
   * FIX - insert comment here.
   */
  public function makeComment($comments) {
    return $this->connection->makeComment($comments);
  }

  /**
   * FIX - insert comment here.
   */
  public function query($query, array $args = array(), $options = array()) {
    return $this->connection->query($query, $args, $options);
  }

  /**
   * FIX - insert comment here.
   */
  public function getDriverClass($class) {
    return $this->connection->getDriverClass($class);
  }

  /**
   * FIX - insert comment here.
   */
  public function select($table, $alias = NULL, array $options = array()) {
    return $this->connection->select($table, $alias, $options);
  }

  /**
   * FIX - insert comment here.
   */
  public function insert($table, array $options = array()) {
    return $this->connection->insert($table, $options);
  }

  /**
   * FIX - insert comment here.
   */
  public function merge($table, array $options = array()) {
    return $this->connection->merge($table, $options);
  }

  /**
   * FIX - insert comment here.
   */
  public function update($table, array $options = array()) {
    return $this->connection->update($table, $options);
  }

  /**
   * FIX - insert comment here.
   */
  public function delete($table, array $options = array()) {
    return $this->connection->delete($table, $options);
  }

  /**
   * FIX - insert comment here.
   */
  public function truncate($table, array $options = array()) {
    return $this->connection->truncate($table, $options);
  }

  /**
   * FIX - insert comment here.
   */
  public function schema() {
    return $this->connection->schema();
  }

  /**
   * FIX - insert comment here.
   */
  public function escapeDatabase($database) {
    return preg_replace('/[^A-Za-z0-9_.]+/', '', $database);
  }

  /**
   * FIX - insert comment here.
   */
  public function escapeTable($table) {
    return $this->connection->escapeTable($table);
  }

  /**
   * FIX - insert comment here.
   */
  public function escapeField($field) {
    return $this->connection->escapeField($field);
  }

  /**
   * FIX - insert comment here.
   */
  public function escapeAlias($field) {
    return $this->connection->escapeAlias($field);
  }

  /**
   * FIX - insert comment here.
   */
  public function escapeLike($string) {
    return $this->connection->escapeLike($string);
  }

  /**
   * FIX - insert comment here.
   */
  public function inTransaction() {
    return $this->connection->inTransaction();
  }

  /**
   * FIX - insert comment here.
   */
  public function transactionDepth() {
    return $this->connection->transactionDepth();
  }

  /**
   * FIX - insert comment here.
   */
  public function startTransaction($name = '') {
    return $this->connection->startTransaction($name);
  }

  /**
   * FIX - insert comment here.
   */
  public function rollback($savepoint_name = 'drupal_transaction') {
    return $this->connection->rollback($savepoint_name);
  }

  /**
   * FIX - insert comment here.
   */
  public function pushTransaction($name) {
    return $this->connection->pushTransaction($name);
  }

  /**
   * FIX - insert comment here.
   */
  public function popTransaction($name) {
    return $this->connection->popTransaction($name);
  }

  /**
   * FIX - insert comment here.
   */
  public function queryRange($query, $from, $count, array $args = array(), array $options = array()) {
    return $this->connection->queryRange($query, $from, $count, $args, $options);
  }

  /**
   * FIX - insert comment here.
   */
  public function queryTemporary($query, array $args = array(), array $options = array()) {
    return $this->connection->queryTemporary($query, $args, $options);
  }

  /**
   * FIX - insert comment here.
   */
  public function driver() {
    return $this->connection->driver();
  }

  /**
   * FIX - insert comment here.
   */
  public function version() {
    return $this->connection->version();
  }

  /**
   * FIX - insert comment here.
   */
  public function supportsTransactions() {
    return $this->connection->supportsTransactions();
  }

  /**
   * FIX - insert comment here.
   */
  public function supportsTransactionalDDL() {
    return $this->connection->supportsTransactionalDDL();
  }

  /**
   * FIX - insert comment here.
   */
  public function databaseType() {
    return $this->connection->databaseType();
  }

  /**
   * FIX - insert comment here.
   */
  public function createDatabase($database) {
    throw new \Exception('Create database is not implemented.');
  }

  /**
   * FIX - insert comment here.
   */
  public function mapConditionOperator($operator) {
    return $this->connection->mapConditionOperator($operator);
  }

  /**
   * FIX - insert comment here.
   */
  public function commit() {
    return $this->connection->commit();
  }

  /**
   * FIX - insert comment here.
   */
  public function nextId($existing_id = 0) {
    return $this->connection->nextId($existing_id);
  }

  /**
   * FIX - insert comment here.
   */
  public function prepare($statement, array $driver_options = array()) {
    return $this->connection->nextId($statement, $driver_options);
  }

  /**
   * FIX - insert comment here.
   */
  public function quote($string, $parameter_type = \PDO::PARAM_STR) {
    return $this->connection->quote($string, $parameter_type);
  }

  /**
   * FIX - insert comment here.
   */
  public function serialize() {
    throw new \Exception('Serialize is not implemented yet.');
  }

  /**
   * FIX - insert comment here.
   */
  public function unserialize($serialized) {
    throw new \Exception('unserialize is not implemented yet.');
  }

}
