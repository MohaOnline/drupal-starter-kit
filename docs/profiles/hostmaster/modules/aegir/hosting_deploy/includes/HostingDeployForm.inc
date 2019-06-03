<?php
/**
 * @file The HostingDeployForm class.
 */

class HostingDeployForm extends HostingForm {

  public function __construct(&$form, &$form_state, &$node = FALSE) {
    parent::__construct($form, $form_state, $node);
    if ($this->node->type == 'platform') {
      $this->platform = new HostingDeployNode($this->node);
    }
  }

  public function alter() {
    $this->makeFieldsetsIntoRadios('field_deployment_strategy', $this->getStrategyLabels());
    $this->addDefaultStrategyOption();
    $this->setStrategy();
    $readonly = isset($this->node->platform_status) &&
      $this->node->platform_status != HOSTING_PLATFORM_QUEUED &&
      !empty($this->node->field_deployment_strategy);
    if ($readonly) {
      $this->makeFieldReadOnly('field_deployment_strategy');
    }
    $this->addGitHelp();

    // TODO: Add a validator to ensure required fields for a given strategy are
    // provided. Add a submit handler to clear out values from other
    // strategies' fields?
  }

  /**
   * Add default option and make it first.
   */
  protected function addDefaultStrategyOption() {
    $default = 'none';
    $options = $this->form['field_deployment_strategy'][$this->language()]['#options'];
    $this->form['field_deployment_strategy'][$this->language()]['#options'] = [$default => $options[$default]] + $options;
  }

  protected function setStrategy() {
    if ($this->isANewNode()) {
      $this->setFormDefaultValue('field_deployment_strategy', 'none');
    }
    else {
      $this->setFormDefaultValue('field_deployment_strategy', $this->determineExistingStrategy());
    }
  }

  protected function determineExistingStrategy() {
    if (!empty($this->platform->getFieldValue('field_deployment_strategy'))) {
      return $this->platform->getFieldValue('field_deployment_strategy');
    }
    else {
      return $this->guessStrategy();
    }
  }

  protected function guessStrategy() {
    foreach ($this->getStrategyRequiredFields() as $strategy => $fields) {
      foreach ($fields as $field) {
        if (!empty($this->platform->getFieldValue($field))) {
          return $strategy;
        }
      }
    }
    return 'none';
  }

  protected function addGitHelp() {
    if (module_exists('hosting_git')) {
      $this->form['git']['#description'] = t('You may deploy and manage this platform from a Git repository. This strategy is appropriate for development scenarios or when each such platform will only host a single site.');
      if (module_exists('hosting_platform_git')) {
        $this->form['git']['#description'] .= t(' It differs from <em>Deploy from Git repository</em> in that, once deployed, platforms can be updated or otherwise changed via Git.');
        $this->form['platform_git']['#description'] .= t(' It differs from <em>Manage with Git</em> in that, once deployed, platforms are treated as immutable.');
      }
    }
  }

  static protected function getStrategies() {
    $strategies = module_invoke_all('platform_deploy_strategies');
    drupal_alter('platform_deploy_strategies', $strategies);
    return $strategies;
  }

  static public function getStrategyLabels() {
    foreach (self::getStrategies() as $name => $strategy) {
      $strategies[$name] = $strategy['label'];
    }
    return $strategies;
  }

  public function getStrategyRequiredFields() {
    foreach ($this->getStrategies() as $name => $strategy) {
      $strategies[$name] = $strategy['required_fields'];
    }
    return $strategies;
  }

}

