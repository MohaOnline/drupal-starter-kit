<?php
/**
 * @file
 * A bean slideshow plugin.
 */

class WetKitSlideBean extends BeanPlugin {

  /**
   * Implements BeanPlugin::values().
   */
  public function values() {
    return array(
      'image_style' => '<none>',
      'navigation' => array(
        'position' => '<none>',
        'previous' => '<',
        'next' => '>',
      ),
      'pager' => array(
        'position' => 'before',
      ),
      'plugin_settings' => array(),
      'slideshow_plugin' => 'wetkit_slide:tabbedinterface',
      'transition_duration' => 3000,
      'timeout' => 8000,
    );
  }

  /**
   * Implements BeanPlugin::form().
   */
  public function form($bean, $form, &$form_state) {
    $plugin_info_all = wetkit_bean_get_plugin_info_all();

    $form['slideshow_plugin'] = array(
      '#type' => 'select',
      '#title' => t('Slideshow plugin'),
      '#options' => array(),
      '#default_value' => $bean->slideshow_plugin,
    );

    $form['plugin_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Plugin settings'),
      '#parents' => array('plugin_settings'),
    );

    foreach ($plugin_info_all as $plugin_type => $plugin_info) {
      $form['slideshow_plugin']['#options'][$plugin_type] = $plugin_info['name'];

      $plugin = wetkit_bean_get_plugin($plugin_type);

      // Use defaults if necessary.
      $defaults = $plugin->values();
      $settings = $bean->plugin_settings + $defaults;

      $plugin_form = $plugin->form($settings);
      $plugin_form['#parents'] = array('plugin_settings');
      $plugin_form['#type'] = 'container';
      $plugin_form['#tree'] = TRUE;
      $plugin_form['#states'] = array(
        'visible' => array(
          ':input[name="slideshow_plugin"]' => array('value' => $plugin_type),
        ),
      );
      $form['plugin_settings'][$plugin_type] = $plugin_form;
    }

    $form['image_style'] = array(
      '#type' => 'select',
      '#title' => t('Image style'),
      '#options' => drupal_map_assoc(array_keys(image_styles())),
      '#default_value' => isset($bean->image_style) ? $bean->image_style : '',
      '#empty_value' => '<none>',
    );

    $form['transition_duration'] = array(
      '#type' => 'textfield',
      '#title' => t('Transition duration'),
      '#description' => t('The time it takes for each slide transition, in milliseconds.'),
      '#default_value' => $bean->transition_duration,
      '#states' => array(
        'invisible' => array(
          array(
            ':input[name="slideshow_plugin"]' => array('value' => 'tabbedinterface'),
            ':input[name="slideshow_plugin"]' => array('value' => 'wetkit_slide:tabbedinterface'),
          ),
        ),
      ),
    );

    $form['timeout'] = array(
      '#type' => 'textfield',
      '#title' => t('Timeout'),
      '#description' => t('The time each slide is shown, in milliseconds.'),
      '#default_value' => $bean->timeout,
      '#states' => array(
        'invisible' => array(
          array(
            ':input[name="slideshow_plugin"]' => array('value' => 'tabbedinterface'),
            ':input[name="slideshow_plugin"]' => array('value' => 'wetkit_slide:tabbedinterface'),
          ),
        ),
      ),
    );

    $form['pager'] = array(
      '#type' => 'fieldset',
      '#title' => t('Pager'),
      '#tree' => TRUE,
      '#states' => array(
        'invisible' => array(
          array(
            ':input[name="slideshow_plugin"]' => array('value' => 'tabbedinterface'),
            ':input[name="slideshow_plugin"]' => array('value' => 'wetkit_slide:tabbedinterface'),
          ),
        ),
      ),
    );

    $form['pager']['position'] = array(
      '#type' => 'select',
      '#title' => t('Position'),
      '#options' => array(
        'before' => t('Before slideshow'),
        'after' => t('After slideshow'),
      ),
      '#description' => t('Where to place the pager.'),
      '#default_value' => $bean->pager['position'],
      '#states' => array(
        'invisible' => array(
          'select[name="pager[type]"]' => array('value' => '<none>'),
        ),
      ),
    );

    $form['navigation'] = array(
      '#type' => 'fieldset',
      '#title' => t('Navigation controls'),
      '#tree' => TRUE,
      '#states' => array(
        'invisible' => array(
          array(
            ':input[name="slideshow_plugin"]' => array('value' => 'tabbedinterface'),
            ':input[name="slideshow_plugin"]' => array('value' => 'wetkit_slide:tabbedinterface'),
          ),
        ),
      ),
    );

    $form['navigation']['position'] = array(
      '#type' => 'select',
      '#title' => t('Position'),
      '#options' => array(
        '<none>' => t('None'),
        'before' => t('Before slideshow'),
        'after' => t('After slideshow'),
      ),
      '#default_value' => $bean->navigation,
    );

    $navigation_states = array(
      'invisible' => array(
        'select[name="navigation[position]"]' => array('value' => '<none>'),
      ),
    );

    $form['navigation']['previous'] = array(
      '#type' => 'textfield',
      '#title' => t('Previous button text'),
      '#default_value' => $bean->navigation['previous'],
      '#states' => $navigation_states,
    );

    $form['navigation']['next'] = array(
      '#type' => 'textfield',
      '#title' => t('Next button text'),
      '#default_value' => $bean->navigation['next'],
      '#states' => $navigation_states,
    );

    return $form;
  }

  /**
   * Implements BeanPlugin::view().
   */
  public function view($bean, $content, $view_mode = 'full', $langcode = NULL) {
    return $content;
  }

  /**
   * Implements BeanPlugin::submit().
   */
  public function submit(Bean $bean) {
    $plugin = wetkit_bean_get_plugin($bean->slideshow_plugin);
    $plugin->submit($bean->data['plugin_settings']);
    return parent::submit($bean);
  }
}
