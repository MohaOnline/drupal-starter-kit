<?php

namespace Drupal\campaignion_manage\BulkOp;

class ContentUnpublish {
  public function __construct() {
  }
  public function title() { return t('Unpublish'); }
  public function helpText() {
    return t('Unpublishing your content will make it invisible to most users of your site. Only users with a special permission are allowed to see unpublished content.');
  }
  public function formElement(&$element, &$form_state) {
    $element['warn']['#type'] = 'container';
    $element['warn']['#attributes']['class'][] = 'warn';
    $message = <<<STR
You're about to unpublish !count pages. Unpublished pages are hidden from
most visitors of your website. If a user tries to access an unpublished
page (ie. via an already shared link) he/she will get an "Access denied" error.
You might want to add redirects to avoid that.
STR;
    $element['warn']['message']['#markup'] = t($message, array('!count' => '<span class="bulkop-count"></span>'));
  }
  public function apply($nids, $values) {
    $messages = [];
    $nodes = node_load_multiple($nids);

    foreach ($nodes as $node) {
      if ($node->status) {
        if (node_access('update', $node)) {
          node_unpublish_action($node);
          node_save($node);
        }
        else {
          array_push($messages, t("Could not unpublish '!node' due to lacking permissions.", ['!node' => $node->title]));
        }
      }
    }

    return $messages;
  }
}
