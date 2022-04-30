<?php

namespace Drupal\openlayers\Extension;

use OpenlayersDrupal\Core\Extension\Extension;
use OpenlayersDrupal\Core\Extension\ModuleHandlerInterface;
use Drupal\openlayers\Legacy\Drupal7;

/**
 * Class that manages modules in a Drupal installation.
 *
 * @codeCoverageIgnore
 *
 * FIX: Some of this might be unit-testable.
 */
class ModuleHandler implements ModuleHandlerInterface {

  /**
   * The Drupal7 service.
   *
   * @var \Drupal\openlayers\Legacy\Drupal7
   */
  protected $drupal7;

  /**
   * The app root.
   *
   * @var string
   */
  protected $root;

  /**
   * Constructs a ModuleHandler object.
   *
   * @param string $root
   *   The app root.
   * @param \Drupal\openlayers\Legacy\Drupal7 $drupal7
   *   The Drupal 7 legacy service.
   *
   * @see \OpenlayersDrupal\Core\DrupalKernel
   * @see \OpenlayersDrupal\Core\CoreServiceProvider
   */
  public function __construct($root, Drupal7 $drupal7) {
    $this->root = $root;
    $this->drupal7 = $drupal7;
  }

  /**
   * {@inheritdoc}
   */
  public function load($name) {
    return $this->drupal7->drupalLoad('module', $name);
  }

  /**
   * {@inheritdoc}
   */
  public function loadAll() {
    $this->drupal7->module_load_all();
  }

  /**
   * {@inheritdoc}
   */
  public function reload() {
    $this->drupal7->module_load_all();
  }

  /**
   * {@inheritdoc}
   */
  public function isLoaded() {
    return $this->drupal7->module_load_all();
  }

  /**
   * {@inheritdoc}
   */
  public function getModuleList() {
    $module_list = array();
    foreach ($this->drupal7->module_list() as $module) {
      $module_list[$module] = $this->getModule($module);
    }
    return $module_list;
  }

  /**
   * {@inheritdoc}
   */
  public function getModule($name) {
    if (!$this->drupal7->module_exists($name)) {
      throw new \InvalidArgumentException(sprintf('The module %s does not exist.', $name));
    }

    $filename = $this->drupal7->drupal_get_filename('module', $name);
    return new Extension($this->root, 'module', $filename, $name . '.info');
  }

  /**
   * {@inheritdoc}
   */
  public function setModuleList(array $module_list = array()) {
    // Convert an array of module filenames to an array of module info, keyed by
    // module name.
    foreach ($module_list as $module_name => $filename) {
      $module_list[$module_name] = array(
        'filename' => $filename,
      );
    }
    $this->drupal7->module_list(FALSE, FALSE, FALSE, $module_list);
  }

  /**
   * {@inheritdoc}
   */
  public function addModule($name, $path) {
    throw new \BadMethodCallException('ModuleHandler::addModule is not implemented.');
  }

  /**
   * {@inheritdoc}
   */
  public function addProfile($name, $path) {
    throw new \BadMethodCallException('ModuleHandler::addProfile is not implemented.');
  }

  /**
   * {@inheritdoc}
   */
  public function buildModuleDependencies(array $modules) {
    // FIX.
  }

  /**
   * {@inheritdoc}
   */
  public function moduleExists($module) {
    return $this->drupal7->module_exists($module);
  }

  /**
   * {@inheritdoc}
   */
  public function loadAllIncludes($type, $name = NULL) {
    $this->drupal7->module_load_all_includes($type, $name);
  }

  /**
   * {@inheritdoc}
   */
  public function loadInclude($module, $type, $name = NULL) {
    $this->drupal7->module_load_include($type, $module, $name);
  }

  /**
   * {@inheritdoc}
   */
  public function getHookInfo() {
    return $this->drupal7->module_hook_info();
  }

  /**
   * {@inheritdoc}
   */
  public function getImplementations($hook) {
    return $this->drupal7->module_implements($hook);
  }

  /**
   * {@inheritdoc}
   */
  public function writeCache() {
    $this->drupal7->module_implements_write_cache();
  }

  /**
   * {@inheritdoc}
   */
  public function resetImplementations() {
    $this->drupal7->drupal_static_reset('module_implements');
  }

  /**
   * {@inheritdoc}
   */
  public function implementsHook($module, $hook) {
    $implementations = $this->drupal7->module_implements($hook);
    return in_array($module, $implementations);
  }

  /**
   * {@inheritdoc}
   */
  public function invoke($module, $hook, array $args = array()) {
    return $this->drupal7->moduleInvoke($module, $hook, $args);
  }

  /**
   * {@inheritdoc}
   */
  public function invokeAll($hook, array $args = array()) {
    return $this->drupal7->moduleInvoke_all($hook, $args);
  }

  /**
   * {@inheritdoc}
   */
  public function alter($type, &$data, &$context1 = NULL, &$context2 = NULL) {
    // FIX Sadly ::alter() does not allow three $context values.
    $this->drupal7->drupalAlter($type, $data, $context1, $context2);
  }

  /**
   * {@inheritdoc}
   */
  public function getModuleDirectories() {
    $dirs = array();
    foreach ($this->getModuleList() as $name => $module) {
      $dirs[$name] = $this->root . '/' . $module->getPath();
    }
    return $dirs;
  }

  /**
   * {@inheritdoc}
   */
  public function getName($module) {
    $module_data = $this->drupal7->system_rebuild_module_data();
    return $module_data[$module]->info['name'];
  }

}
