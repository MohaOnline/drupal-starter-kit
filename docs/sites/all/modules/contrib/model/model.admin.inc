<?php

/**
 * @file
 * Model editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class ModelUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {
    
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Models',
      'description' => 'Add edit and update models.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );
    
    // Change the overview menu type for the list of models.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    
    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a model',
      'description' => 'Add a new model',
      'page callback'  => 'model_add_page',
      'access callback'  => 'model_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'model.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );
    
    // Add menu items to add each different type of entity.
    foreach (model_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'model_form_wrapper',
        'page arguments' => array(model_create(array('type' => $type->type))),
        'access callback' => 'model_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'model.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing model entities
    $items[$this->path . '/model/' . $wildcard] = array(
      'page callback' => 'model_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'model_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'model.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/model/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );
    
    $items[$this->path . '/model/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'model_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'model_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'model.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    // Menu item for viewing models
    $items['model/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'model_page_title',
      'title arguments' => array(1),
      'page callback' => 'model_page_view',
      'page arguments' => array(1),
      'access callback' => 'model_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }
  
  
  /**
   * Create the markup for the add Model Entities page within the class
   * so it can easily be extended/overriden.
   */ 
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }    
        
    return theme('model_add_list', array('content' => $content));
  }
  
}


/**
 * Form callback wrapper: create or edit a model.
 *
 * @param $model
 *   The model object being edited by this form.
 *
 * @see model_edit_form()
 */
function model_form_wrapper($model) {
  // Add the breadcrumb for the form's location.
  model_set_breadcrumb();
  return drupal_get_form('model_edit_form', $model);
}


/**
 * Form callback wrapper: delete a model.
 *
 * @param $model
 *   The model object being edited by this form.
 *
 * @see model_edit_form()
 */
function model_delete_form_wrapper($model) {
  // Add the breadcrumb for the form's location.
  //model_set_breadcrumb();
  return drupal_get_form('model_delete_form', $model);
}


/**
 * Form callback: create or edit a model.
 *
 * @param $model
 *   The model object to edit or for a create form an empty model object
 *     with only a model type defined.
 */
function model_edit_form($form, &$form_state, $model) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Model Name'),
    '#default_value' => isset($model->name) ? $model->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );
  
  $form['data']['#tree'] = TRUE;
  $form['data']['sample_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('An interesting model switch'),
    '#default_value' => isset($model->data['sample_data']) ? $model->data['sample_data'] : 1,
  );

  // Add the field related form elements.
  $form_state['model'] = $model;
  field_attach_form('model', $model, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save model'),
    '#submit' => $submit + array('model_edit_form_submit'),
  );
  
  if (!empty($model->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete model'),
      '#suffix' => l(t('Cancel'), 'admin/content/models'),
      '#submit' => $submit + array('model_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'model_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the model form
 */
function model_edit_form_validate(&$form, &$form_state) {
  $model = $form_state['model'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('model', $model, $form, $form_state);
}


/**
 * Form API submit callback for the model form.
 * 
 * @todo remove hard-coded link
 */
function model_edit_form_submit(&$form, &$form_state) {
  
  $model = entity_ui_controller('model')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the model and go back to the list of models
  
  // Add in created and changed times.
  if ($model->is_new = isset($model->is_new) ? $model->is_new : 0){
    $model->created = time();
  }

  $model->changed = time();
  
  $model->save();
  $form_state['redirect'] = 'admin/content/models';
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function model_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/models/model/' . $form_state['model']->model_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a model.
 *
 * @param $model
 *   The model to delete
 *
 * @see confirm_form()
 */
function model_delete_form($form, &$form_state, $model) {
  $form_state['model'] = $model;

  $form['#submit'][] = 'model_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete model %name?', array('%name' => $model->name)),
    'admin/content/models/model',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for model_delete_form
 */
function model_delete_form_submit($form, &$form_state) {
  $model = $form_state['model'];

  model_delete($model);

  drupal_set_message(t('The model %name has been deleted.', array('%name' => $model->name)));
  watchdog('model', 'Deleted model %name.', array('%name' => $model->name));

  $form_state['redirect'] = 'admin/content/models';
}



/**
 * Page to add Model Entities.
 *
 * @todo Pass this through a proper theme function
 */
function model_add_page() {
  $controller = entity_ui_controller('model');
  return $controller->addPage();
}


/**
 * Displays the list of available model types for model creation.
 *
 * @ingroup themeable
 */
function theme_model_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="model-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer model types')) {
      $output = '<p>' . t('Model Entities cannot be added because you have not created any model types yet. Go to the <a href="@create-model-type">model type creation page</a> to add a new model type.', array('@create-model-type' => url('admin/structure/model_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No model types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative model pages.
 */
function model_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('Models'), 'admin/content/models'),
  );

  drupal_set_breadcrumb($breadcrumb);
}



