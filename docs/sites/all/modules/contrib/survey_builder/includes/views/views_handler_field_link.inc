<?php
/**
 * @file
 * Link handlers for surveys and survey responses
 */

/*
 * Implementation of views_handler_field
 */
abstract class survey_builder_views_handler_field_link extends views_handler_field {

  protected $label;
  protected $path;
  protected $permission;

  public function construct() {
    parent::construct();
    $this->additional_fields['pid'] = 'pid';
  }

  public function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  public function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);
  }

  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  public function render($values) {
    if (!user_access($this->permission)) {
      return;
    }
    $pid = $this->sanitize_value($this->get_value($values, 'pid'));
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = sprintf($this->path, $pid);
    $text = !empty($this->options['text']) ? check_plain($this->options['text']) : t($this->get_label($pid));
    return $text;
  }

  public function get_label($pid) {
    return $this->label;
  }

}

/**
 * Complete survey link
 */
class survey_builder_views_handler_field_survey_link extends survey_builder_views_handler_field_link {

  protected $path = 'survey/%d';
  protected $permission = 'survey complete';

  public function get_label($pid) {
    module_load_include('inc', 'survey_builder', 'includes/survey_builder.survey');
    $survey = survey_load($pid);
    if (!$survey) {
      return;
    }
    return check_plain($survey->title);
  }

}

/**
 * Edit survey link
 */
class survey_builder_views_handler_field_survey_edit_link extends survey_builder_views_handler_field_link {
  protected $label = 'Edit';
  protected $path = 'survey/%d/edit';
  protected $permission = 'survey create';
}

/**
 * Edit survey link
 */
class survey_builder_views_handler_field_survey_delete_link extends survey_builder_views_handler_field_link {
  protected $label = 'Delete';
  protected $path = 'survey/%d/delete';
  protected $permission = 'survey delete';
}

/**
 * View survey's responses link
 */
class survey_builder_views_handler_field_survey_responses_link extends survey_builder_views_handler_field_link {
  protected $label = 'View';
  protected $path = 'survey/%d/responses';
  protected $permission = 'survey view';
}

/**
 * View survey response link
 */
class survey_builder_views_handler_field_survey_response_link extends survey_builder_views_handler_field_link {
  protected $label = 'View';
  protected $path = 'survey-response/%d';
  protected $permission = 'survey view';
}
