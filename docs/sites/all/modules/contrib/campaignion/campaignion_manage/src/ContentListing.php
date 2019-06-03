<?php

namespace Drupal\campaignion_manage;

use \Drupal\campaignion_action\Loader;

class ContentListing {
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
    $element['#attributes']['class'][] = 'campaignion-manage-content-listing';
    $element['#type'] = 'campaignion_manage_listing';

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
      '#description' => t('Check this if you want to apply a bulk operation to all matching content (on all pages).'),
    );
    $selectAll['data'][0] = array(
      'data' => array(
        '#name' => 'listing[bulkop_select_all_matching]',
      ) + $element['bulkop_select_all_matching'],
      'colspan' => $columns,
      'class' => array('bulkop-select-toggles', 'bulkop-button-wrapper'),
    );
    $rows[] = $selectAll;

    $element['bulk_nid'] = array(
      '#type' => 'checkboxes',
      '#options' => array(),
      '#attributes' => array('class' => array('bulk-select-target')),
    );
    $element['bulk_tnid'] = array(
      '#type' => 'checkboxes',
      '#options' => array(),
      '#attributes' => array('class' => array('bulk-select-target')),
    );

    $tnode_count = 1;
    foreach ($query->execute() as $tnode) {
      $class = ($tnode_count++ % 2 == 0) ? 'even' : 'odd';
      $row = $this->nodeRow($tnode, TRUE, $element);
      $row['class'][] = $class;
      $rows[] = $row;
      if (count($tnode->translations) > 0) {
        $bigcellrow['data']['bigcell']['colspan'] = $columns;
        $bigcellrow['class'][] = 'node-translations';
        $bigcellrow['class'][] = $class;
        $bigcellrow['no_striping'] = TRUE;

        $innerrows = array();
        foreach ($tnode->translations as $lang => $node) {
          $innerrows[] = $this->nodeRow($node, FALSE, $element);
        }
        $bigcellrow['data']['bigcell']['data'] = array(
          '#theme' => 'table',
          '#rows' => $innerrows,
        );
        $rows[] = $bigcellrow;
      }
    }

    $element['#attributes']['class'][] = 'bulkop-select-wrapper';
    $element += array(
      '#rows' => $rows,
    );
  }

  protected function nodeRow($node, $tset, &$element) {
    $row['data']['bulk']['class'] = array('manage-bulk');
    $pfx = 'bulk_' . ($tset ? 'tnid' : 'nid');
    $row['data']['bulk']['data'] = array(
      '#type' => 'checkbox',
      '#title' => $tset ? t("Select this content and all it's translations for bulk operations") : t('Select this content for bulk operations.'),
      '#return_value' => $node->nid,
      '#default_value' => isset($element[$pfx]['#default_value'][$node->nid]),
    );
    $element[$pfx][$node->nid] = &$row['data']['bulk']['data'];
    $element[$pfx]['#options'][$node->nid] = $node->nid;
    $row['data']['content']['class'] = array('campaignion-manage');
    $row['data']['content']['data'] = array(
      '#theme' => 'campaignion_manage_node',
      '#node' => $node,
      '#translation_set' => $tset,
    );
    $row['data']['links']['class'] = array('manage-links');
    $row['data']['links']['data'] = array(
      '#theme' => 'links__ctools_dropbutton',
      '#links' => $this->nodeLinks($node),
      '#image' => TRUE,
    );
    if ($tset) {
      $row['no_striping'] = TRUE;
      if (isset($node->translations) && count($node->translations) > 1) {
        $row['class'][] = 'node-translation-set';
      }
    }
    return $row;
  }

  protected function nodeLinks($node) {
    $links = array();
    $edit_path_part = 'edit';

    // set path to wizard for action content types
    if (module_exists('campaignion_wizard')) {
      if (Loader::instance()->isActionType($node->type)) {
        $edit_path_part = 'wizard';
      }
    }
    foreach (array($edit_path_part => t('Edit'), 'translate' => t('Translate'), 'view' => t('View page'), 'delete' => t('Delete')) as $path => $title) {
      $action = in_array($path, ['wizard', 'edit']) ? 'update' : $path;
      if (node_access($action, $node)) {
        $links[$path] = array(
          'href' => "node/{$node->nid}/$path",
          'title' => $title,
          'query' => array('destination' => 'admin/manage/content_and_actions')
        );
      }
    }
    return $links;
  }

  public function selectedIds(&$element, &$form_state, $baseQuery) {
    $values = &drupal_array_get_nested_value($form_state['values'], $element['#array_parents']);
    if (!empty($values['bulkop_select_all_matching'])) {
      $query = $baseQuery->filtered();
      $fields = $query->getfields();
      $fields = array();
      $query->addField('n', 'nid', 'id');
      $ids = array();
      foreach ($query->execute() as $row) {
        $ids[] = $row->id;
      }
      return $ids;
    }
    $nids = array();
    foreach ($values['bulk_nid'] as $nid => $selected) {
      if ($selected) {
        $nids[$nid] = $nid;
      }
    }
    foreach ($values['bulk_tnid'] as $nid => $selected) {
      if ($selected) {
        $nids[$nid] = $nid;
      }
    }
    return array_keys($nids);
  }
}
