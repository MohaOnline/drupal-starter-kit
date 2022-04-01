<?php

namespace Drupal\campaignion_manage\Filter;

use \Drupal\campaignion_action\Loader;


class SupporterActivity extends Base {
  protected $query;

  public function __construct(\SelectQueryInterface $query) {
    $this->query = $query;
  }

  protected function actionsWithActivity() {
    $query = db_select('node', 'n');
    $query->fields('n', array('nid'))
      ->where('n.tnid = 0 OR n.tnid = n.nid')
      ->orderBy('n.created', 'DESC');
    $nids = $query->execute()->fetchCol();
    $nodes = entity_load('node', $nids);

    $actions = array();
    foreach ($nodes as $node) {
      $actions[$node->type][$node->nid] = $node->title;
    }

    return $actions;
  }

  protected function typesInUse() {
    $available_activities = array(
      'any_activity'          => t('Any type'),
      'redhen_contact_create' => t('Contact created'),
      'webform_submission'    => t('Online action'),
      'webform_payment'       => t('Online payment'),
    );

    $activities_in_use = array('any_activity' => t('Any activity'));

    $query = db_select('campaignion_activity', 'act');
    $query->condition('act.type', array_keys($available_activities), 'IN');
    $query->fields('act', array('type'));
    $query->groupBy('act.type');

    $activities_in_use += $query->execute()->fetchAllKeyed(0,0);
    return array_intersect_key($available_activities, $activities_in_use);
  }

  public function formElement(array &$form, array &$form_state, array &$values) {
    $frequency_id  = drupal_html_id('activity-frequency');
    $date_range_id = drupal_html_id('activity-date-range');
    $activity_type_id = drupal_html_id('activity-type');
    $form['frequency'] = array(
      '#type'          => 'select',
      '#title'         => t('Activity'),
      '#attributes'    => array('id' => $frequency_id),
      '#options'       => array('any' => t('Any frequency'), 'never' => t('Never'), 'how_many' => t('How many times?')),
      '#default_value' => isset($values['frequency']) ? $values['frequency'] : NULL,
    );
    $form['how_many_op'] = array(
      '#type'          => 'select',
      '#options'       => array('=' => t('Exactly'), '>' => t('More than'), '<' => t('Less than')),
      '#states'        => array('visible' => array('#' . $frequency_id => array('value' => 'how_many'))),
      '#default_value' => isset($values['how_many_op']) ? $values['how_many_op'] : NULL,
    );
    $form['how_many_nr'] = array(
      '#type'          => 'textfield',
      '#size'          => 10,
      '#maxlength'     => 10,
      '#states'        => array('visible' => array('#' . $frequency_id => array('value' => 'how_many'))),
      '#default_value' => isset($values['how_many_nr']) ? $values['how_many_nr'] : NULL,
      '#element_validate' => array('campaignion_manage_activity_how_many_validate'),
    );
    $form['activity'] = array(
      '#type'          => 'select',
      '#id'            => $activity_type_id,
      '#options'       => $this->typesInUse(),
      '#default_value' => isset($values['activity']) ? $values['activity'] : NULL,
    );

    $form_types = array('any' => t('Any type of action'));
    $payment_types = array('any' => t('Any type of payment'));
    $actions = $this->actionsWithActivity();
    foreach (Loader::instance()->allTypes() as $type => $action_type) {
      if (empty($actions[$type])) {
        continue;
      }
      if ($action_type->isDonation()) {
        $payment_types[$type] = node_type_get_name($type);
      }
      else {
        $form_types[$type] = node_type_get_name($type);
      }
    }
    $form += $this->actionSubForm('form', $form_types, $actions, $values, 'webform_submission', $activity_type_id);
    $form += $this->actionSubForm('payment', $payment_types, $actions, $values, 'webform_payment', $activity_type_id);

    $form['date_range'] = array(
      '#type'          => 'select',
      '#attributes'    => array('id' => $date_range_id),
      '#options'       => array('all' => t('Any time'), 'range' => t('Date range'), 'to' => t('to'), 'from' => t('from')),
      '#default_value' => isset($values['date_range']) ? $values['date_range'] : NULL,
    );
    $form['date_from'] = array(
      '#type'          => 'date_popup',
      '#title'         => t('from'),
      '#date_format'   => 'Y/m/d',
      '#states'        => array('visible' => array('#' . $date_range_id => array('value' => 'from'))),
      '#default_value' => isset($values['date_from']) ? $values['date_from'] : NULL,
      '#attributes'    => array('class' => array('campaignion-manage-date')),
    );
    $form['date_to'] = array(
      '#type'          => 'date_popup',
      '#title'         => t('to'),
      '#date_format'   => 'Y/m/d',
      '#states'        => array('visible' => array('#' . $date_range_id => array('value' => 'to'))),
      '#default_value' => isset($values['date_to']) ? $values['date_to'] : NULL,
      '#attributes'    => array('class' => array('campaignion-manage-date')),
    );
    $form['date_range_from'] = array(
      '#type'          => 'date_popup',
      '#title'         => t('from'),
      '#date_format'   => 'Y/m/d',
      '#states'        => array('visible' => array('#' . $date_range_id => array('value' => 'range'))),
      '#default_value' => isset($values['date_from']) ? $values['date_from'] : NULL,
      '#attributes'    => array('class' => array('campaignion-manage-date')),
    );
    $form['date_range_to'] = array(
      '#type'          => 'date_popup',
      '#title'         => t('to'),
      '#date_format'   => 'Y/m/d',
      '#states'        => array('visible' => array('#' . $date_range_id => array('value' => 'range'))),
      '#default_value' => isset($values['date_to']) ? $values['date_to'] : NULL,
      '#attributes'    => array('class' => array('campaignion-manage-date')),
    );
  }

  protected function actionSubForm($pfx, $types, $actions, $values, $activity_type, $activity_type_id) {
    $node_type_id = drupal_html_id('node-type');
    $form["${pfx}_node_type"] = array(
      '#type' => 'select',
      '#id' => $node_type_id,
      '#options' => $types,
      '#states'  => array('visible' => array(
        '#' . $activity_type_id => array('value' => $activity_type)
      )),
      '#default_value' => isset($values["${pfx}_node_type"]) ? $values["${pfx}_node_type"] : 'any',
    );
    unset($types['any']);
    foreach ($types as $type => $type_name) {
      reset($actions[$type]);
      $default = key($actions[$type]);
      $form["node_${type}_nid"] = array(
        '#type'          => 'select',
        '#options'       => array('no_specific' => t('No specific action')) + $actions[$type],
        '#states'        => array('visible' => array(
          '#' . $node_type_id => array('value' => $type),
          '#' . $activity_type_id => array('value' => $activity_type)
        )),
        '#attributes' => array('class' => array('filter-action')),
        '#default_value' => isset($values["node_${type}_nid"]) ? $values["node_${type}_nid"] : $default,
      );
    }
    return $form;
  }

  protected function addWebformFilter($query, $type, $nid) {
    $query->innerJoin('campaignion_activity_webform', 'wact', "act.activity_id = wact.activity_id");
    $query->innerJoin('node', 'n', "wact.nid = n.nid");
    if ($nid !== 'no_specific') {
      $query->where('n.nid = :nid OR n.tnid = :nid', array(':nid' => $nid));
    }
    else {
      $query->condition('n.type', $type);
    }
  }

  public function title() { return t('Activity'); }


  protected function isNoop(array $values) {
    if ($values['activity'] == 'any_activity' && $values['frequency'] == 'any') {
      return $values['date_range'] == 'all' || !$this->dateFilterIsComplete($values);
    }
    return FALSE;
  }

  /**
   * Check if we have a complete set of values for the date filter.
   *
   * The auto-submit leads to one submit every time some value in the date form
   * is changed. This means we need to ignore all incomplete filters.
   *
   * @param array $values
   *   The filter values that were submitted.
   *
   * @return bool
   *   TRUE if all needed values were provided otherwise FALSE.
   */
  protected function dateFilterIsComplete(array $values) {
    switch ($values['date_range']) {
      case 'all':
      case 'never':
        return TRUE;

      case 'range':
        return $values['date_range_from'] && $values['date_range_to'];

      case 'to':
        return (bool) $values['date_to'];

      case 'from':
        return (bool) $values['date_from'];
    }
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function intermediateResult(array $values) {
    return !$this->isNoop($values);
  }

  public function apply($query, array $values) {
    // Do nothing for any number of times + any type + any time.
    if ($this->isNoop($values)) {
      return;
    }

    $inner = db_select('campaignion_activity', 'act');
    $inner->fields('act', array('contact_id'));

    if ($values['activity'] != 'any_activity') {
      $inner->condition('act.type', $values['activity']);
    }
    else {
      // "RedHen contact was edited" activities are never shown
      $inner->condition('act.type', 'redhen_contact_edit', '!=');
    }
    if ($values['activity'] == 'webform_submission') {
      $type = $values['form_node_type'];
      if ($type != 'any') {
        $this->addWebformFilter($inner, $values['form_node_type'], $values["node_${type}_nid"]);
      }
    }
    elseif ($values['activity'] == 'webform_payment') {
      $type = $values['payment_node_type'];
      if ($type != 'any') {
        $this->addWebformFilter($inner, $values['payment_node_type'], $values["node_${type}_nid"]);
      }
    }

    if ($values['frequency'] === 'how_many') {
      if ($values['activity'] === 'any_activity') {
        // when the user selects any activity but wants to filter for number of
        // activities we don't want to include "RedHen contact was created" activities
        $inner->condition('act.type', 'redhen_contact_create', '!=');
      }
      $inner->groupBy('act.contact_id');
      $inner->having('COUNT(*)' . $values['how_many_op'] . ' :nr', array(':nr' => $values['how_many_nr']));
    }

    if ($this->dateFilterIsComplete($values)) {
      switch ($values['date_range']) {
        case 'range':
          $date_range = array(strtotime($values['date_range_from']), strtotime($values['date_range_to']));
          $inner->condition('act.created', $date_range, 'BETWEEN');
          break;
        case 'to':
          $to = strtotime($values['date_to']);
          $inner->condition('act.created', $to, '<');
          break;
        case 'from':
          $from  = strtotime($values['date_from']);
          $inner->condition('act.created', $from, '>');
          break;
      }
    }
    if ($values['frequency'] == 'never') {
      $inner = db_select('redhen_contact', 'r')
        ->fields('r', array('contact_id'))
        ->condition('r.contact_id', $inner, 'NOT IN');
    }
    else {
      $inner->distinct();
    }
    $tname = db_query_temporary((string) $inner, $inner->getArguments());
    $query->innerJoin($tname, 'af', "%alias.contact_id = r.contact_id");
  }

  public function isApplicable($current) {
    return count($this->typesInUse()) > 1;
  }

  public function defaults() {
    $types = $this->typesInUse();
    return array(
      'frequency'   => 'any',
      'how_many_op' => '=',
      'how_many_nr' => '1',
      'activity'    => key($types),
      'action_type' => 'any',
      'date_range'  => 'all',
      'date_after'  => '',
      'date_before' => '',
    );
  }
}
