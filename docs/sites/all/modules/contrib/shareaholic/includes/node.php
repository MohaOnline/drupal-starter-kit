<?php

  /**
   * @file
   *
   * This file is responsible for containing
   * hooks involving nodes
   */


  /**
   * Implements hook_node_view()
   *
   * When a node is rendered, insert the content meta tags
   *
   */
  function shareaholic_node_view($node, $view_mode, $langcode) {
    ShareaholicPublic::insert_widgets($node, $view_mode, $langcode);
    ShareaholicPublic::insert_content_meta_tags($node, $view_mode, $langcode);
    ShareaholicPublic::insert_og_tags($node, $view_mode);
  }


  /**
   * Implements hook_node_update().
   * When a node is updated, notify CM to scrape its details
   * and clear FB cache
   *
   * @param $node The node that has been updated
   */
  function shareaholic_node_update($node) {
    ShareaholicContentSettings::update($node);
    ShareaholicContentManager::single_page_worker($node);
  }


  /**
   * Implements hook_node_delete().
   * When a node is deleted, notify CM
   *
   * @param $node The node that has been deleted
   */
  function shareaholic_node_delete($node) {
    ShareaholicContentSettings::delete($node);
    ShareaholicContentManager::single_page_worker($node);
  }


  /**
   * Implements hook_node_insert().
   * When a node is created, notify CM to scrape its details
   * and clear FB cache
   *
   * @param $node The node that has been created
   */
  function shareaholic_node_insert($node) {
    ShareaholicContentSettings::insert($node);
    ShareaholicContentManager::single_page_worker($node);
  }


  /**
   * Implements hook_form_node_form_alter().
   *
   * When the node form is presented, add additional options
   * for Shareaholic Apps
   *
   * @param Array $form - Nested array of form elements
   * @param Array $form_state - keyed array containing form state
   * @param $form_id - String representing the name of the form itself
   */
  function shareaholic_form_node_form_alter(&$form, &$form_state, $form_id) {
    $node = $form['#node'];

    $form['shareaholic_options'] = array(
      '#type' => 'fieldset',
      '#access' => TRUE,
      '#title' => 'Shareaholic Options',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'additional_settings',
      '#weight' => 100,
    );

    // I have to namespace it this way because Drupal can't add
    // the shareholic_options name to it
    // when you process the form on submit!!!
    $form['shareaholic_options']['shareaholic_hide_share_buttons'] = array(
      '#type' => 'checkbox',
      '#title' => 'Hide Share Buttons'
    );

    $form['shareaholic_options']['shareaholic_hide_recommendations'] = array(
      '#type' => 'checkbox',
      '#title' => 'Hide Related Content'
    );

    $form['shareaholic_options']['shareaholic_exclude_from_recommendations'] = array(
      '#type' => 'checkbox',
      '#title' => 'Exclude from Related Content'
    );

    $form['shareaholic_options']['shareaholic_exclude_og_tags'] = array(
      '#type' => 'checkbox',
      '#title' => 'Do not include Open Graph tags'
    );

    if(!db_table_exists('shareaholic_content_settings')) {
      $form['shareaholic_options']['shareaholic_message'] = array(
        '#type' => 'markup',
        '#markup' => '<p style="color:#FF0000;">' . t('Action required: you have some pending updates required by Shareaholic. Please go to update.php for more information.') . '</p>',
      );
    }

    if($node->shareaholic_options['shareaholic_exclude_from_recommendations']) {
      $form['shareaholic_options']['shareaholic_exclude_from_recommendations']['#attributes'] = array('checked' => 'checked');
    }

    if($node->shareaholic_options['shareaholic_hide_recommendations']) {
      $form['shareaholic_options']['shareaholic_hide_recommendations']['#attributes'] = array('checked' => 'checked');
    }

    if($node->shareaholic_options['shareaholic_hide_share_buttons']) {
      $form['shareaholic_options']['shareaholic_hide_share_buttons']['#attributes'] = array('checked' => 'checked');
    }

    if($node->shareaholic_options['shareaholic_exclude_og_tags']) {
      $form['shareaholic_options']['shareaholic_exclude_og_tags']['#attributes'] = array('checked' => 'checked');
    }
  }


  /**
   * Implements hook_node_prepare().
   *
   * If the given node does not have shareaholic settings
   * set, give default values in the object
   *
   * @param Object $node The node being shown in add/edit form
   */
  function shareaholic_node_prepare($node) {
    if (empty($node->shareaholic_options)) {
      // Set default values, since this only runs when adding a new node
      // or an existing node without the values
      $node->shareaholic_options = array(
        'shareaholic_exclude_from_recommendations' => false,
        'shareaholic_hide_recommendations' => false,
        'shareaholic_hide_share_buttons' => false,
        'shareaholic_exclude_og_tags' => false,
      );
    }
  }


  /**
   * Implements hook_node_submit().
   *
   * Retrieve the inputs to the shareaholic options form
   *
   */
  function shareaholic_node_submit($node, $form, &$form_state) {
    $values = $form_state['values'];
    // Move the new data into the node object.
    // This is why I namespaced it: the form values does not preserve shareaholic_options
    $isChecked = ($values['shareaholic_exclude_from_recommendations'] === 1) ? true : false;
    $node->shareaholic_options['shareaholic_exclude_from_recommendations'] = $isChecked;

    $isChecked = ($values['shareaholic_hide_recommendations'] === 1) ? true : false;
    $node->shareaholic_options['shareaholic_hide_recommendations'] = $isChecked;

    $isChecked = ($values['shareaholic_hide_share_buttons'] === 1) ? true : false;
    $node->shareaholic_options['shareaholic_hide_share_buttons'] = $isChecked;

    $isChecked = ($values['shareaholic_exclude_og_tags'] === 1) ? true : false;
    $node->shareaholic_options['shareaholic_exclude_og_tags'] = $isChecked;
  }



  /**
   * Implements hook_node_load().
   *
   * When the nodes are being loaded, attach
   * the shareaholic node settings
   */
  function shareaholic_node_load($nodes, $types) {
    ShareaholicContentSettings::load($nodes, $types);
  }
