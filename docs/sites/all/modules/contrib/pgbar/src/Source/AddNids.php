<?php

namespace Drupal\pgbar\Source;

/**
 * Provides widget and functionality to add a set of nids to the counter result.
 */
class AddNids {

  /**
   * @var int
   */
  protected $nid;

  /**
   * Construct a new instance.
   */
  public function __construct($nid) {
    $this->nid = $nid;
  }

  /**
   * Build the configuration form widget for adding node IDs.
   *
   * @param array $item
   *   A field item.
   * @param array $form
   *   A Form-API array to extend.
   *
   * @return array
   *   Form-API array.
   */
  public function widgetForm(array $item, array $form = []) {
    $item['options']['source'] += [
      'add_nids' => [],
    ];
    $source_options = $item['options']['source'];
    if (is_array($source_options['add_nids'])) {
      $source_options['add_nids'] = implode(',', $source_options['add_nids']);
    }
    $form['add_nids'] = [
      '#title' => t('Include nodes'),
      '#description' => t('Enter node ids (separated by comma) to add submissions from those nodes (and their translations) to the number shown by the counter.'),
      '#type' => 'textfield',
      '#default_value' => $source_options['add_nids'],
    ];
    $form['#element_validate'][] = [$this, 'widgetValidate'];
    return $form;
  }

  /**
   * Element validate callback for widgetForm().
   *
   * @see Drupal\pgbar\Source\AddNids::widgetForm()
   */
  public function widgetValidate($element, &$form_state, $form) {
    $item = &$form_state['values'];
    foreach ($element['#parents'] as $key) {
      $item = &$item[$key];
    }
    $nids = &$item['add_nids'];
    $to_int = function ($x) {
      return (int) $x;
    };
    $nids = array_map($to_int, array_filter(explode(',', $nids)));
    foreach ($nids as $n) {
      if ($n <= 0) {
        form_error($element['add_nids'], t('%name: Please enter only node ids (seperated with ‘,’) to include additional nodes.', ['%name' => $element['#title']]));
        break;
      }
    }
  }

  /**
   * Generate a SQL query to get all nodes to query.
   *
   * @param array $item
   *   A field item.
   *
   * @return SelectQueryInterface
   *   A query usable in a SQL IN-clause.
   */
  public function translationsQuery(array $item) {
    $item['options']['source'] += [
      'add_nids' => [],
    ];
    $nids = $item['options']['source']['add_nids'];
    $nids = array_merge([$this->nid], $nids);
    $q_nids = db_select('node', 'n');
    $q_nids->leftJoin('node', 'nt', 'n.tnid>0 AND nt.tnid=n.tnid');
    $q_nids->addExpression('COALESCE(nt.nid, n.nid)', 'nid');
    return $q_nids->condition('n.nid', $nids);
  }

}
