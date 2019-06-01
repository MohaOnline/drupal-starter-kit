<?php

/**
 * Class for deploying to a REST server using JSON
 */
class WetKitServiceRestJSON extends DeployServiceRest {

  public function deploy(Traversable $iterator) {
    foreach ($iterator as $entity) {
      $json_data = drupal_json_encode($entity);
      $this->config['headers']['Content-Type'] = 'application/json';

      // Language Handling for Entity Translation.
      if (function_exists('entity_translation_enabled') && entity_translation_enabled($entity->__metadata['type'], $entity, TRUE)) {
        global $language;
        $tmp_url = parse_url($this->config['url']);

        if ($entity->__metadata['type'] == 'taxonomy_term' && !isset($entity->name_original)) {
          $url = $this->config['url'] . '/' . $entity->__metadata['uri'] . '.json';
        }
        else {
          $count = drupal_strlen('/' . $language->prefix);
          if ((substr($tmp_url['path'], 0, $count) == '/' . $language->prefix)) {
            $tmp_url['path'] = substr($tmp_url['path'], $count);
          }
          $tmp_url['path'] = '/' . $language->prefix . $tmp_url['path'];
          $this->config['url'] = $this->unparse_url($tmp_url);
        }
      }

      // Language Handling for Menu Link.
      if ($entity->__metadata['type'] == 'menu_link') {
        global $language;
        $tmp_url = parse_url($this->config['url']);

        $count = drupal_strlen('/' . $language->prefix);
        if ((substr($tmp_url['path'], 0, $count) == '/' . $language->prefix)) {
          $tmp_url['path'] = substr($tmp_url['path'], $count);
        }
        $tmp_url['path'] = '/' . $language->prefix . $tmp_url['path'];
        $this->config['url'] = $this->unparse_url($tmp_url);
      }

      // TODO: Consider making the resource path configurable.
      $url = $this->config['url'] . '/' . $entity->__metadata['uri'] . '.json';
      // Temporary hack for the built-in PHP 5.4 web server that doesn't route
      // filenames correctly.
      // TODO: See if we can remove the .json suffix for all server. The
      // Content-Type header should be enough in most (all?) cases.
      if (php_sapi_name() == 'cli-server') {
        $url = $this->config['url'] . '/' . $entity->__metadata['uri'];
      }
      $this->httpRequest($url, 'PUT', $json_data);
    }
  }

  public function publish(Traversable $iterator) {
    foreach ($iterator as $entity) {
      $this->config['headers']['Content-Type'] = 'application/json';

      // TODO: Consider making the resource path configurable.
      $url = $this->config['url'] . '/' . $entity->__metadata['uri'] . '/publish';
    }
  }

  public function unparse_url($parsed_url) {
    $scheme = isset($parsed_url['scheme']) ? $parsed_url['scheme'] . '://' : '';
    $host = isset($parsed_url['host']) ? $parsed_url['host'] : '';
    $port = isset($parsed_url['port']) ? ':' . $parsed_url['port'] : '';
    $user = isset($parsed_url['user']) ? $parsed_url['user'] : '';
    $pass = isset($parsed_url['pass']) ? ':' . $parsed_url['pass'] : '';
    $pass = ($user || $pass) ? "$pass@" : '';
    $path = isset($parsed_url['path']) ? $parsed_url['path'] : '';
    $query = isset($parsed_url['query']) ? '?' . $parsed_url['query'] : '';
    $fragment = isset($parsed_url['fragment']) ? '#' . $parsed_url['fragment'] : '';

    return "$scheme$user$pass$host$port$path$query$fragment";
  }
}
