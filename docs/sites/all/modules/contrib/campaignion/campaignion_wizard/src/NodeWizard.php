<?php

namespace Drupal\campaignion_wizard;

use Drupal\oowizard\Wizard;

/**
 * Base class for wizards dealing with editing or creating a node.
 */
abstract class NodeWizard extends Wizard {
  public $node;
  public $parameters;
  protected $levels;
  protected $status;

  /**
   * Create a new instance of this wizard.
   *
   * @param array $parameters
   *   Custom parameters for this wizard.
   * @param object $node
   *   The node to be edited or NULL if a new one should be created.
   * @param string $type
   *   The node type.
   * @param object $user
   *   The user who is editing the node.
   */
  public function __construct(array $parameters = array(), $node = NULL, $type = NULL, $user = NULL) {
    $this->parameters = $parameters;
    foreach ($this->steps as &$class) {
      if ($class[0] != '\\') {
        $class = '\\' . __NAMESPACE__ . '\\' . $class;
      }
    }
    $this->levels = array_flip(array_keys($this->steps));
    $this->status = NULL;

    $this->user = $user ? $user : $GLOBALS['user'];
    $this->node = $node ? $node : $this->prepareNode($type);
    node_object_prepare($this->node);
    parent::__construct($user);
    $this->formInfo['path'] = $node ? "node/{$node->nid}/wizard/%step" : "wizard/{$this->node->type}";

    drupal_set_title(t('Create @action', ['@action' => node_type_get_name($this->node)]));
    $this->formInfo += array(
      'show return' => TRUE,
      'return path' => $node ? 'node/' . $this->node->nid : 'node',
    );
    $this->status = !empty($this->node->nid) ? Status::loadOrCreate($this->node->nid) : new Status();
  }

  /**
   * Set the wizard step that’s to be displayed.
   *
   * @param string $step
   *   The machine readable name of the step.
   */
  public function setStep($step) {
    parent::setStep($step);
    if (!$this->status->step || $this->levels[$this->status->step] < $this->levels[$step]) {
      $this->status->step = $step;
    }
  }

  /**
   * Generate the wizard form.
   *
   * @return array
   *   A form-API array.
   */
  public function wizardForm() {
    $form = parent::wizardForm() + array(
      'wizard_advanced' => array(),
    );
    return $form;
  }

  /**
   * Create a new node of a specific type and prepare it for being edited.
   *
   * @param string $type
   *   The node-type / bundle of the new node.
   *
   * @return object
   *   A newly created node-object.
   */
  public function prepareNode($type) {
    $node = (object) array('type' => $type);
    $node->uid = $this->user->uid;
    $node->name = isset($this->user->name) ? $this->user->name : NULL;
    $node->language = LANGUAGE_NONE;
    $node->title = '';
    $node->sticky = 0;

    if (module_exists('change_publishing_status_permission')
      && !user_access('change publishing status')) {
      $node->status = FALSE;
    }

    return $node;
  }

  /**
   * Generate the links for the wizard trail.
   *
   * @return array
   *   An array of trail items. Each item has the following keys:
   *   - url: The path pointing to the step.
   *   - title: A title to display for this step.
   *   - accessible: Whether a user should be able to navigate to this step
   *     directly.
   *   - current: TRUE if this step is being displayed right now.
   */
  public function trailItems() {
    $trail = array();
    $accessible = TRUE;
    $completed = empty($this->status) ? -1 : $this->levels[$this->status->step];
    foreach ($this->stepHandlers as $urlpart => $step) {
      $is_current = $urlpart == $this->currentStep;
      $trail[] = array(
        'url' => strtr($this->formInfo['path'], array('%step' => $urlpart)),
        'title' => $step->getTitle(),
        'accessible' => $accessible = ($accessible && ($this->levels[$urlpart] <= $completed) && $step->checkDependencies()),
        'current' => $urlpart == $this->currentStep,
      );
    }
    return $trail;
  }

  /**
   * Form submit handler: Save the current node.
   */
  public function submit($form, &$form_state) {
    parent::submit($form, $form_state);
    $this->status->nid = $this->node->nid;
    $this->status->save();
  }

}
