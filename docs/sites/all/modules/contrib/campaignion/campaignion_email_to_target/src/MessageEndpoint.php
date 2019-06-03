<?php

namespace Drupal\campaignion_email_to_target;


class MessageEndpoint {
  protected $node;

  public function __construct($node) {
    $this->node = $node;
  }

  /**
   * Convert message data from API to model format.
   *
   * @return array
   *   - Move message content (subject, header, message, footer) into the main array.
   *   - Move filter configuration into the config sub-array.
   */
  protected function api2model($data) {
    $data = $this->flattenMessage($data);
    $data += ['filters' => []];
    $filters = [];
    foreach ($data['filters'] as $f) {
      $filters[] = $this->unflattenFilter($f);
    }
    $data['filters'] = $filters;
    return $data;
  }

  /**
   * Convert message data from model to API format.
   *
   * @return array
   *   - Build the message content sub-array (subject, header, body, footer).
   *   - Extract the filter configuration into the main filter array.
   */
  protected function model2api($data) {
    $data = $this->unflattenMessage($data);
    $data += ['filters' => []];
    $filters = [];
    foreach ($data['filters'] as &$f) {
      $filters[] = $this->flattenFilter($f);
    }
    $data['filters'] = $filters;
    return $data;
  }

  protected function flattenFilter($data) {
    if (isset($data['config']) && is_array($data['config'])) {
      $config = $data['config'];
      unset($data['config']);
      $data += $config;
    }
    return $data;
  }

  protected function unflattenFilter($data) {
    $config = $data + ['id' => NULL, 'weight' => 0, 'type' => ''];
    unset($config['message_id']);
    $data = [];
    foreach (['id', 'weight', 'type'] as $k) {
      $data[$k] = $config[$k];
      unset($config[$k]);
    }
    $data['config'] = $config;
    return $data;
  }

  protected function flattenMessage($data) {
    if (isset($data['message']) && is_array($data['message'])) {
      $message = $data['message'];
      // 'message' is called 'body' in the API.
      if (isset($message['body'])) {
        $message['message'] = $message['body'];
      }
      unset($message['body']);
      unset($data['message']);
      return $message + $data;
    }
    return $data;
  }

  protected function unflattenMessage($data) {
    $message = [];
    foreach (['subject', 'header', 'message', 'footer'] as $k) {
      $message[$k] = $data[$k];
      unset($data[$k]);
    }
    // 'message' is called 'body' in the API.
    $message['body'] = $message['message'];
    unset($message['message']);
    $data['message'] = $message;
    return $data;
  }

  public function put($data) {
    $data += ['messageSelection' => []];
    $data = $data['messageSelection'];
    $old_messages = MessageTemplate::byNid($this->node->nid);
    $w = 0;
    $new_messages = [];
    foreach ($data as $m) {
      $m = $this->api2model($m);
      if (isset($m['id']) && isset($old_messages[$m['id']])) {
        $message = $old_messages[$m['id']];
        $message->setData($m);
        unset($old_messages[$message->id]);
      }
      else {
        $message = new MessageTemplate($m);
      }
      $message->nid = $this->node->nid;
      $message->weight = $w++;
      $message->save();
      $new_messages[] = $this->model2api($message->toArray());
    }
    // Old messages that are still in there have been deleted.
    foreach ($old_messages as $message) {
      $message->delete();
    }
    return ['messageSelection' => $new_messages];
  }

  public function get() {
    $messages = [];
    $templates = MessageTemplate::byNid($this->node->nid);
    if (!$templates) {
      $templates[] = new MessageTemplate([
        'subject' => '',
        'header' => t("Dear [email-to-target:salutation],\n"),
        'message' => '',
        'footer' => t("\n\nYours sincerely,\n[submission:values:first_name] [submission:values:last_name]\n[submission:values:street_address]\n[submission:values:postcode]"),
      ]);
    }
    foreach ($templates as $m) {
      $messages[] = $this->model2api($m->toArray());
    }
    return ['messageSelection' => $messages];
  }

}
