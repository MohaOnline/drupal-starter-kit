<?php

namespace Drupal\campaignion_newsletters;

class ProviderFactory {
  protected static $instance = NULL;

  protected $providers;
  public static function getInstance() {
    if (!isset(self::$instance)) {
      $providers = module_invoke_all('campaignion_newsletters_provider_info');
      self::$instance = new static($providers);
    }
    return self::$instance;
  }

  public function __construct(array $providers) {
    $this->providers = $providers;
  }

  public function providers() {
    return array_keys($this->providers);
  }

  public function providerByKey($key) {
    if (isset($this->providers[$key])) {
      $info = $this->providers[$key];
      $func = [$info['class'], 'fromParameters'];
      return call_user_func($func, $info['params']);
    } else {
      \watchdog('campaignion_newsletters', 'No provider found for source-key: !key', array('!key' => $key), WATCHDOG_ERROR);
    }
  }
}
