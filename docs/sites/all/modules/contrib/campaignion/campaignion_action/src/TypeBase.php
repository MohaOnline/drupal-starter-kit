<?php

namespace Drupal\campaignion_action;

class TypeBase implements TypeInterface {

  /**
   * Content-type
   */
  protected $type;
  /**
   * Parameters
   */
  public $parameters;

  public function __construct($type, array $parameters = array()) {
    $this->type = $type;
    $this->parameters = $parameters + [
      'action_class' => '\\Drupal\\campaignion_action\\ActionBase',
      'donation' => FALSE,
    ];
  }

  /**
   * Get the default template node ID for the node.
   *
   * @param object $node
   *   The node to get the template for.
   *
   * @return int|null
   *   The node ID of the template node or NULL if no templates was found
   */
  public function defaultTemplateNid($node) {
    $uuid = $this->parameters['template_node_uuid'];
    $ids = \entity_get_id_by_uuid('node', [$uuid]);
    if (($nid = array_shift($ids)) && ($template = node_load($nid))) {
      if (module_exists('translation') && $template->tnid) {
        $t = translation_node_get_translations($template->tnid);
        if (isset($t[$node->language])) {
          return $t[$node->language]->nid;
        }
      }
      return $template->nid;
    }
    return NULL;
  }

  /**
   * {@inheritdoc}
   */
  public function isDonation() {
    return $this->parameters['donation'];
  }

  /**
   * {@inheritdoc}
   */
  public function isEmailProtest() {
    return !empty($this->parameters['email_protest']);
  }

}
