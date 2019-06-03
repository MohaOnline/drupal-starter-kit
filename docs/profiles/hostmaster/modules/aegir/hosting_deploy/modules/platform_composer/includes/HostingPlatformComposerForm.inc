<?php
/**
 * @file The HostingPlatformComposerForm class.
 */

class HostingPlatformComposerForm extends HostingForm {

  // Fields managed by this module.
  protected $fields = [
    'field_composer_project_docroot',
    'field_composer_project_package',
    'field_composer_project_path',
    'field_composer_project_version',
  ];

  public function __construct(&$form, &$form_state, &$node = FALSE) {
    parent::__construct($form, $form_state, $node);
    if ($this->node->type == 'platform') {
      $this->platform = new HostingPlatformComposerNode($node);
    }
  }

  public function alter() {
    $this->disableField('field_composer_project_path');
    $this->lockFields();
    $this->moveFieldsIntoFieldset('platform_composer', $this->fields);
    $this->form['platform_composer']['#title'] = 'Deploy a Composer project from a Packagist repository';
    $this->form['platform_composer']['#description'] = '
<p>You may deploy this platform from a Composer package hosted on <a href="https://packagist.org/">Packagist</a>, the PHP package repository.</p>

<p>To provision a clean Drupal 8+ core codebase for Aegir, one would typically enter <em>web</em> for the Docroot, <em>aegir/drupal-project-composer</em> for the Composer Package Name, and <em>dev-aegir3-8.x</em> for the Composer Package Version.  For more information on that project, see <a href="https://packagist.org/packages/aegir/drupal-project-composer">the project page on Packagist</a>.</p>

<p>However, you will probably want to customize your installation after starting with this project as template, and then adding contributed and custom modules and themes.  To do that, use the <em>Deploy a Composer project from a Git repository</em> option instead, which allows you to clone from your own Git repository (once you\'ve created it).</p>

<p>It should be noted that this option is not currently working with repositories requiring Drush 9 due to <a href="https://www.drupal.org/project/provision/issues/2953349">Including Drush 9 in a Composer-based platform breaks Provision\'s Verify</a>.  Either fork the repository and remove the requirement (or move it to <em>require-dev</em> as the suggested-above repository does), or do this in your own Git repository using the <em>Deploy a Composer project from a Git repository</em> option instead of this one.</p>

<p><strong>Note:</strong> This activity will recalculate all dependencies, and thus, it is very resource-intensive. It requires a miminum of 4G of RAM; we advise against performing this activity in production.</p>
    ';
    $this->addSubmitHandler('hosting_platform_composer_platform_submit');
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
    if ($this->isANewNode() && !empty($this->getFormStateValue('field_composer_project_package'))) {
      $this->setFormStateValue('field_composer_project_path', $this->getFormStateValue('publish_path'));
    }
  }

  protected function setPublishPath() {
    if (!empty($this->getFormStateValue('field_composer_project_docroot'))) {
      $publish_path = $this->getFormStateValue('field_composer_project_path') . '/' . $this->getFormStateValue('field_composer_project_docroot');
      $this->setFormStateValue('publish_path', $publish_path);
    }
  }

}

