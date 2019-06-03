<?php

namespace Drupal\campaignion_manage;

class SupporterListing {
  protected $query;
  protected $size;
  public function __construct($pageSize = 20) {
    $this->size = $pageSize;
  }
  /**
   * Build a renderable array based on the data-rows.
   *
   * @param rows result from the Query object
   * @return renderable array for output.
   */
  public function build(&$element, &$form_state, $query) {
    $element += array(
      '#type' => 'campaignion_manage_listing',
      '#attributes' => array(
        'class' => array('campaignion-manage-supporter-listing'),
      ),
      '#formObj' => $this,
    );

    $element['#attributes']['data-count'] = $query->count();
    $query->setPage($this->size);
    $columns = 3;

    $rows = array();
    $selectAll = array(
      'no_striping' => TRUE,
    );
    $id = drupal_html_id('bulkop-select-all-matching');
    $element['bulkop_select_all_matching'] = array(
      '#type' => 'checkbox',
      '#id' => $id,
      '#title' => t('Select items from all pages'),
      '#description' => t('Check this if you want to apply a bulk operation to all matching contacts (on all pages).'),
    );
    $selectAll['data'][0] = array(
      'data' => array(
        '#name' => 'listing[bulkop_select_all_matching]',
      ) + $element['bulkop_select_all_matching'],
      'colspan' => $columns,
      'class' => array('bulkop-select-toggles', 'bulkop-button-wrapper'),
    );
    $rows[] = $selectAll;

    $element['bulk_id'] = array(
      '#type' => 'checkboxes',
      '#options' => array(),
      '#attributes' => array('class' => array('bulk-select-target')),
    );

    $evenodd = 1;
    foreach ($query->execute() as $contact) {
      $row = $this->renderRow($contact, $element);
      $rows[] = $row;
    }

    $element['#attributes']['class'][] = 'bulkop-select-wrapper';
    $element += array(
      '#rows' => $rows,
    );
  }

  protected function renderRow($contact, &$element) {
    $row['data']['bulk']['class'] = array('manage-bulk');
    $pfx = 'bulk_id';
    $row['data']['bulk']['data'] = array(
      '#type' => 'checkbox',
      '#title' => t("Select this contact for bulk operations"),
      '#return_value' => $contact->contact_id,
      '#default_value' => isset($element[$pfx]['#default_value'][$contact->contact_id]),
    );
    $element[$pfx][$contact->contact_id] = &$row['data']['bulk']['data'];
    $element[$pfx]['#options'][$contact->contact_id] = $contact->contact_id;
    $row['data']['content']['class'] = array('campaignion-manage');
    $row['data']['content']['data'] = array(
      '#theme' => 'campaignion_manage_contact',
      '#contact' => $contact,
    );
    $row['data']['links']['class'] = array('manage-links');
    $row['data']['links']['data'] = array(
      '#theme' => 'links__ctools_dropbutton',
      '#links' => $this->renderLinks($contact),
      '#image' => TRUE,
    );
    return $row;
  }

  protected function renderLinks($contact) {
    $links = array();
    foreach (array('edit' => t('Edit'), 'view' => t('View contact'), 'delete' => t('Delete')) as $path => $title) {
      $links[$path] = array(
        'href' => "redhen/contact/{$contact->contact_id}/view/$path",
        'title' => $title,
      );
    }
    return $links;
  }
}
