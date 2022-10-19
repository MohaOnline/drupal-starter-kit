<?php

namespace Drupal\campaignion_wizard;

use \Drupal\campaignion\Forms\EmbeddedNodeForm;
use Drupal\campaignion_action\Redirects\Endpoint;
use Drupal\campaignion_action\Redirects\Redirect;

class ThankyouStep extends WizardStep {
  protected $step = 'thank';
  protected $title = 'Thank you';
  protected $contentType;

  /**
   * Reference to the items of the thank you page field of the edited node.
   *
   * @var array
   */
  protected $fieldItems;
  protected $doubleOptIn;

  public function __construct($wizard) {
    parent::__construct($wizard);
    $parameters = drupal_array_merge_deep(array(
      'thank_you_page' => array(
        'type' => 'thank_you_page',
        'reference' => 'field_thank_you_pages',
      ),
    ), $wizard->parameters);
    $parameters =& $parameters['thank_you_page'];
    $this->contentType = $parameters['type'];
    $this->fieldItems = &$wizard->node->{$parameters['reference']}[LANGUAGE_NONE];
    if (is_null($this->fieldItems)) {
      $this->fieldItems = [];
    }
    // Make sure we always have two field items otherwise the thank you page
    // is renumbered to 0.
    $this->fieldItems += [
      0 => ['type' => 'node', 'node_reference_nid' => NULL],
      1 => ['type' => 'node', 'node_reference_nid' => NULL],
    ];
    $this->doubleOptIn = !empty($wizard->node->nid) && $this->hasDoubleOptIn();
  }

  protected function hasDoubleOptIn() {
    $emails = $this->wizard->node->webform['emails'];
    $eid = EmailStep::WIZARD_CONFIRMATION_REQUEST_EID;
    return isset($emails[$eid]) && !empty($emails[$eid]['status']);
  }

  protected function loadIncludes() {
    module_load_include('pages.inc', 'node');
    module_load_include('inc', 'webform', 'includes/webform.emails');
    module_load_include('inc', 'webform', 'includes/webform.components');
  }

  /**
   * Get components of a node in a format needed by the redirect app.
   *
   * @param object $node
   *   This node’s components are listed.
   *
   * @return array
   *   Array of component metadata arrays with the following keys:
   *   - id: The component ID.
   *   - label: The component’s label.
   */
  protected static function components($node) {
    $fields = [];
    foreach ($node->webform['components'] as $cid => $component) {
      $fields[] = [
        'id' => $cid,
        'label' => $component['name'],
      ];
    }
    return $fields;
  }

  protected function pageForm(&$form_state, $index, $title, $prefix) {
    $item = $this->fieldItems[$index] ?? ['type' => 'node'];

    $type = $item['type'];
    $node = NULL;
    if (isset($item['node_reference_nid'])) {
      $node = node_load($item['node_reference_nid']);
    }
    if (!$node) {
      $node = $this->wizard->prepareNode($this->contentType);
    }
    $node->wizard_parent = $this->wizard->node;

    $form = array(
      '#type'  => 'fieldset',
      '#title' => $title,
      '#delta' => $index,
      '#process' => [[static::class, 'addRedirectSettings']],
    );
    $form['type'] = array(
      '#type'          => 'radio',
      '#title'         => t('Redirect supporters to a URL after the action'),
      '#return_value'  => 'redirect',
      '#default_value' => $type == 'redirect' ? 'redirect' : NULL,
      '#parents'       => array($prefix, 'type'),
    );

    // Personalized redirects widget
    $redirect_container_id = drupal_html_id('personalized-redirects-widget');
    $form['redirects'] = array(
      '#type'       => 'container',
      '#title'      => t('Manage redirects'),
      '#states'     => array('visible' => array(":input[name=\"${prefix}[type]\"]" => array('value' => 'redirect'))),
      '#id'         => $redirect_container_id,
      '#attributes' => ['class' => ['personalized-redirects-widget']],
    );

    $form['or2'] = array(
      '#type'   => 'markup',
      '#markup' => '<div class="thank-you-outer-or"><span class="thank-you-line-or"><span class="thank-you-text-or">' . t('or') . '</span></span></div>',
    );
    $form['type3'] = array(
      '#type'          => 'radio',
      '#title'         => t('Create new thank you page'),
      '#return_value'  => 'node',
      '#default_value' => $type == 'node' ? 'node' : NULL,
      '#parents'       => $form['type']['#parents'],
    );
    $embedState = array(
      '#wizard_type' => 'thank_you',
      '#wizard_node' => $this->wizard->node,
    );
    $parents = array($prefix, 'node_form');
    node_object_prepare($node);
    $formObj = new EmbeddedNodeForm($node, $form_state, $parents, $embedState);
    $node_form = array(
      '#type'    => 'container',
      '#states'  => array('visible' => array(":input[name=\"${prefix}[type]\"]" => array('value' => 'node'))),
      '#tree'    => TRUE,
    ) + $formObj->formArray($form);

    $node_form['title']['#required'] = FALSE;
    // don't publish per default
    $node_form['options']['status']['#default_value'] = 0;
    $node_form['options']['promote']['#default_value'] = 0;

    // order the form fields
    $node_form['field_main_image']['#attributes']['class'][] = 'sidebar-narrow-right';
    $node_form['#tree'] = TRUE;
    $node_form['actions']['#access'] = FALSE;

    $form['node_form'] = $node_form;
    $form['#attributes']['class'][] = 'thank-you-node-wrapper';

    $form['#tree'] = TRUE;
    return $form;
  }

  public function stepForm($form, &$form_state) {
    $form = parent::stepForm($form, $form_state);

    // check if double opt in was enabled and if yes provide a 2nd thank you page
    $thank_you_class = 'half-left';
    if ($this->doubleOptIn) {
      $form['submission_node'] = $this->pageForm($form_state, Redirect::CONFIRMATION_PAGE, t('Submission page'), 'submission_node');
      $form['submission_node']['#attributes']['class'][] = 'half-left';
      $thank_you_class = 'half-right';
      $form['#attributes']['class'][] = 'two-halfs';
    }

    $form['thank_you_node'] = $this->pageForm($form_state, Redirect::THANK_YOU_PAGE, t('Thank you page'), 'thank_you_node');
    $form['thank_you_node']['#attributes']['class'][] = $thank_you_class;

    $dir = drupal_get_path('module', 'campaignion_wizard');
    $form['#attached']['js'][] = [
      'data' => $dir . '/js/redirects_app/redirects_app.vue.min.js',
      'scope' => 'footer',
      'preprocess' => FALSE,
    ];
    $form['#attached']['css'][] = [
      'data' => $dir . '/css/redirects_app/redirects_app.css',
      'group' => 'CSS_DEFAULT',
    ];

    $form['#tree'] = TRUE;
    $form['wizard_head']['#tree'] = FALSE;
    return $form;
  }

  public function checkDependencies() {
    return isset($this->wizard->node->nid);
  }

  public function validateStep($form, &$form_state) {
    $values =& $form_state['values'];
    $thank_you_pages = array('thank_you_node');
    if ($this->doubleOptIn) {
      $thank_you_pages[] = 'submission_node';
    }
    foreach ($thank_you_pages as $page) {
      if (in_array($values[$page]['type'], array('node', 'redirect')) == FALSE) {
        form_set_error('type', t('You have to create either a thank you page or provide a redirect.'));
      }
      if ($values[$page]['type'] == 'node') {
        $form_state['embedded'][$page]['node_form']['formObject']->validate($form, $form_state);
        if (empty($values[$page]['node_form']['title'])) {
          form_set_error("$page][node_form][title", t('!name field is required.', array('!name' => 'Title')));
        }
      }
      // Redirect validating is done in the JS prior to the submit.
    }
  }

  public function submitStep($form, &$form_state) {
    $values =& $form_state['values'];
    unset($form_state['values']);
    $action = $this->wizard->node;

    $thank_you_pages = array('thank_you_node' => Redirect::THANK_YOU_PAGE);
    if ($this->doubleOptIn) {
      $thank_you_pages['submission_node'] = Redirect::CONFIRMATION_PAGE;
    }

    foreach($thank_you_pages as $page => $index) {
      $item = &$this->fieldItems[$index];
      $item['type'] = $values[$page]['type'];
      if ($values[$page]['type'] == 'node') {
        $form_state['values'] =& $values[$page]['node_form'];

        $formObj = $form_state['embedded'][$page]['node_form']['formObject'];
        $formObj->submit($form, $form_state);
        $item['node_reference_nid'] = $formObj->node()->nid;
      }
    }
    // We completely ignore $node->webform['redirect'] and the redirect urls
    // for confirmation emails here because their behavior is overriden in
    // campaignion_action.
    node_save($action);

    $form_state['values'] =& $values;
  }

  public function status() {
    $msg = [
      '#theme' => 'campaignion_wizard_thank_summary',
      '#items' => $this->fieldItems,
      '#node' => $this->wizard->node,
      '#double_optin' => $this->hasDoubleOptIn(),
    ];
    return array(
      'caption' => t('Thank you page'),
      'message' => drupal_render($msg),
    );
  }

  /**
   * Form processing handler for attaching settings for the redirect vue app.
   *
   * The existing redirects need to be added in a process handler otherwise they
   * won’t be updated on server-side validation errors.
   *
   * @see \Drupal\campaignion_wizard\ThankYouStep::pageForm()
   */
  public static function addRedirectSettings($element, &$form_state) {
    $node = $form_state['oowizard']->node;
    $delta = $element['#delta'];
    $settings = [
      'fields' => static::components($node),
      'endpoints' => [
        'nodes' => url('wizard/nodes'),
        'redirects' => url("node/{$node->nid}/redirects/$delta"),
      ],
    ] + (new Endpoint($node, $delta))->get();
    $settings['campaignion_wizard'][$element['redirects']['#id']] = $settings;
    $element['#attached']['js'][] = ['data' => $settings, 'type' => 'setting'];
    return $element;
  }

}
