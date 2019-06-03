<?php

trait views_parity_row_base {

  function option_definition() {
    $options = parent::option_definition();

    $options['views_parity_row_enable'] = array('default' => FALSE, 'bool' => TRUE);
    $options['views_parity_row']['frequency'] = array('default' => '2');
    $options['views_parity_row']['start'] = array('default' => '0');
    $options['views_parity_row']['end'] = array('default' => '0');
    $options['views_parity_row']['view_mode'] = array('default' => 'teaser');

    return $options;
  }

  function summary_title() {
    if ($this->options['views_parity_row_enable'] == TRUE) {
      $options = $this->options_form_summary_options();
      return check_plain($options[$this->options['view_mode']] . ' | ' . $this->options['views_parity_row']['frequency'] . ' | ' . $options[$this->options['views_parity_row']['view_mode']]);
    } else {
      return parent::summary_title();
    }
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = $this->options['views_parity_row'];
    $default = $this->option_definition();

    $form['views_parity_row_enable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Alternate with a different view mode every X rows ?'),
      '#default_value' => isset($this->options['views_parity_row_enable']) ?
        $this->options['views_parity_row_enable'] : $default['views_parity_row_enable']['default'],
    );

    $form['views_parity_row'] = array(
      '#type' => 'fieldset',
      '#title' => t('Configuration of Views Parity Row'),
      '#states' => array(
        'visible' => array(
          ':input[name="row_options[views_parity_row_enable]"]' => array('checked' => TRUE),
        ),
      ),
      'frequency' => array(
        '#type' => 'textfield',
        '#title' => t('Frequency of view mode change.'),
        '#description' => t('Chose a positive integer number. This number will be the frequency of change of the content view mode. Example, if you chose <em>3</em>, it means that every 3 rows, the content will use the other View mode.'),
        '#size' => 6,
        '#maxlength' => 6,
        '#default_value' => isset($options['frequency']) ? (int) $options['frequency'] : $default['views_parity_row']['frequency']['default'],
        '#element_validate' => array('element_validate_integer_positive')
      ),
      'start' => array(
        '#type' => 'textfield',
        '#title' => t('Start'),
        '#description' => t('Start at which row ?'),
        '#size' => 6,
        '#maxlength' => 6,
        '#default_value' => isset($options['start']) ? (int) $options['start'] : $default['views_parity_row']['start']['default'],
        '#element_validate' => array('element_validate_integer')
      ),
      'end' => array(
        '#type' => 'textfield',
        '#title' => t('End'),
        '#description' => t('End at which row ? Set <em>0</em> to disable.'),
        '#size' => 6,
        '#maxlength' => 6,
        '#default_value' => isset($options['end']) ? (int) $options['end'] : $default['views_parity_row']['end']['default'],
        '#element_validate' => array('element_validate_integer')
      ),
      'view_mode' => array(
        '#type' => 'select',
        '#options' => $this->options_form_summary_options(),
        '#title' => t('Alternate view mode'),
        '#default_value' => isset($options['view_mode']) ? $options['view_mode'] : $default['views_parity_row']['view_mode']['default'],
      ),
    );
  }

  function find_view_mode($options, $index) {
    $view_mode = $options['view_mode'];

    if ($options['views_parity_row_enable'] == TRUE) {
      $view_mode_override = FALSE;
      if ($index >= $options['views_parity_row']['start']) {
        if ($options['views_parity_row']['end'] != 0) {
          if ($index <= $options['views_parity_row']['end']) {
            $view_mode_override = TRUE;
          }
        }
        else {
          $view_mode_override = TRUE;
        }
      }

      if ($view_mode_override == TRUE) {
        if (($index-$options['views_parity_row']['start']) % $options['views_parity_row']['frequency'] == 0) {
          $view_mode = $options['views_parity_row']['view_mode'];
        }
      }
    }

    return $view_mode;
  }

}
