<?php

namespace Drupal\campaignion_action\Redirects;

/**
 * API-Endpoint for editing redirects.
 */
class Endpoint {

  /**
   * The node that is being edited.
   *
   * @var object
   */
  protected $node;

  /**
   * The number of the redirect set.
   *
   * @var int
   */
  protected $delta;

  /**
   * Construct a new API-endpoint.
   *
   * @param object $node
   *   The node which’s redirects are being edited.
   * @param int $delta
   *   The type of redirect that‘s being edited. Currently valid values are:
   *   - 0: Redirect after sending the confirmation request email.
   *   - 1: Redirect for when no confirmation is needed or the submission is
   *        being confirmed.
   */
  public function __construct($node, $delta) {
    $this->node = $node;
    $this->delta = $delta;
  }

  /**
   * Convert redirect data from API to model format.
   *
   * @param array $data
   *   Data to convert.
   *
   * @return array
   *   Converted data.
   */
  protected function api2model(array $data) {
    $data += ['filters' => []];
    return $data;
  }

  /**
   * Convert redirect data from model to API format.
   *
   * @param array $data
   *   Data to convert.
   *
   * @return array
   *   Converted data.
   */
  protected function model2api(array $data) {
    $data += ['filters' => []];
    return $data;
  }

  /**
   * Handle a PUT request.
   *
   * @param array $data
   *   Parsed JSON data sent to the API.
   *
   * @return array
   *   API representation of the redirects.
   */
  public function put(array $data) {
    $data += ['redirects' => []];
    $data = $data['redirects'];
    $old_redirects = Redirect::byNid($this->node->nid, $this->delta);
    $w = 0;
    $new_redirects = [];
    foreach ($data as $r) {
      $r = $this->api2model($r);
      if (isset($r['id']) && isset($old_redirects[$r['id']])) {
        $redirect = $old_redirects[$r['id']];
        $redirect->setData($r);
        unset($old_redirects[$redirect->id]);
      }
      else {
        $redirect = new Redirect($r);
      }
      $redirect->nid = $this->node->nid;
      $redirect->delta = $this->delta;
      $redirect->weight = $w++;
      $redirect->save();
      $new_redirects[] = $this->model2api($redirect->toArray());
    }
    // Old redirects that are still in there have been deleted.
    foreach ($old_redirects as $redirect) {
      $redirect->delete();
    }
    return ['redirects' => $new_redirects];
  }

  /**
   * Handle a GET request.
   *
   * @return array
   *   API representation of the stored redirects.
   */
  public function get() {
    $values = [];
    $redirects = Redirect::byNid($this->node->nid, $this->delta);
    if (!$redirects) {
      $redirects[] = new Redirect([
        'label' => '',
        'destination' => '',
      ]);
    }
    foreach ($redirects as $r) {
      $values[] = $this->model2api($r->toArray());
    }
    return ['redirects' => $values];
  }

}
