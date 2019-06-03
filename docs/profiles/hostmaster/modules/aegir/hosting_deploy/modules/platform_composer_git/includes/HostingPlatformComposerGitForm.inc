<?php
/**
 * @file The HostingPlatformComposerGitForm class.
 */

class HostingPlatformComposerGitForm extends HostingForm {

  // Fields managed by this module.
  protected $fields = [
    'field_composer_git_docroot',
    'field_composer_git_project_url',
    'field_composer_git_path',
    'field_composer_git_version',
  ];

  public function __construct(&$form, &$form_state, &$node = FALSE) {
    parent::__construct($form, $form_state, $node);
    if ($this->node->type == 'platform') {
      $this->platform = new HostingPlatformComposerGitNode($node);
    }
  }

  public function alter() {
    $this->disableField('field_composer_git_path');
    $this->lockFields();
    $this->moveFieldsIntoFieldset('platform_composer_git', $this->fields);
    $this->form['platform_composer_git']['#title'] = 'Deploy a Composer project from a Git repository';
    $this->form['platform_composer_git']['#description'] = '
<p>You may deploy this platform from a Composer project Git URL.</p>

<p>You\'ll need a Git repository started with something like the following:</p>

<p><code>composer create-project drupal-composer/drupal-project:8.x-dev new-drupal-project --stability dev --no-interaction</code></p>

<p>Note that until <a href="https://www.drupal.org/project/provision/issues/2953349">Including Drush 9 in a Composer-based platform breaks Provision\'s verify</a> is resolved, the Drush dependency must be moved to the development section of the makefile in order for the build to work properly within Aegir.  So you\'ll need to either move it from the <em>require</em> section to the <em>require-dev</em> section manually, or run a command like this:</p>

<p><code>composer require --dev \'drush/drush:^9.0.0\'</code></p>
    ';
    $this->addSubmitHandler('hosting_platform_composer_git_platform_submit');
  }

  protected function lockFields() {
    if (isset($this->node->platform_status) && $this->node->platform_status != HOSTING_PLATFORM_QUEUED) {
      foreach ($this->fields as $field) {
        $this->makeFieldReadOnly($field);
      }
    }
  }

  public function submit() {
    $this->setProjectPath();
    $this->setPublishPath();
  }

  protected function setProjectPath() {
    if ($this->isANewNode() && !empty($this->getFormStateValue('field_composer_git_project_url'))) {
      $this->setFormStateValue('field_composer_git_path', $this->getFormStateValue('publish_path'));
    }
  }

  protected function setPublishPath() {
    if (!empty($this->getFormStateValue('field_composer_git_docroot'))) {
      $publish_path = $this->getFormStateValue('field_composer_git_path') . '/' . $this->getFormStateValue('field_composer_git_docroot');
      $this->setFormStateValue('publish_path', $publish_path);
    }
  }

}

