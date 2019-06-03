<?php
/**
 * @file
 * Query string process class.
 */

class FunnelbackQueryString {

  /**
   * Get facet query string from raw query string.
   *
   * @param array $raw_queries
   * @return array
   */
  public static function funnelbackFilterFacetQueryString($raw_queries = []) {
    $facet_query = [];
    foreach ($raw_queries as $param) {
      if (substr($param, 0, 2) == 'f.') {
        // Compose query string array.
        $whole_string = explode('=', $param);
        $facet_query[$whole_string[0]][] = str_replace(' ', '+', strip_tags(urldecode($whole_string[1])));
      }
    }

    return $facet_query;
  }

  /**
   * Get facet query string from raw query string.
   *
   * @param array $raw_queries
   * @return array
   */
  public static function funnelbackFilterContextualQueryString($raw_queries = []) {
    $contextual_query = [];
    foreach ($raw_queries as $param) {
      if (substr($param, 0, 7) == 'cluster' ||
        substr($param, 0, 15) == 'clicked_fluster') {
        // Compose query string array.
        $whole_string = explode('=', $param);
        $contextual_query[$whole_string[0]] = str_replace(' ', '+', strip_tags(urldecode($whole_string[1])));
      }
    }

    return $contextual_query;
  }

  /**
   * Remove system default query strings from link.
   *
   * @param $query_string
   * @return string
   */
  public static function funnelbackFilterSystemQueryString($query_string) {
    $query_string = str_replace('?', '', $query_string);
    $strings = explode('&', $query_string);
    foreach ($strings as $key => $string) {
      if (substr($string, 0, 10) == 'remote_ip=' ||
        substr($string, 0, 8) == 'profile=' ||
        substr($string, 0, 11) == 'collection=' ||
        substr($string, 0, 5) == 'form=') {
        // Remove system query strings.
        unset($strings[$key]);
      }
    }

    return '?'. implode('&', $strings);
  }

  /**
   * Normalise query.
   *
   * @param $string
   * @return mixed|string
   */
  public static function funnelbackQueryNormaliser($string) {
    // Covert f_ to f. for facet query format in funnelback.
    $string = str_replace('f_', 'f.', $string);

    // Decode query string for later replacement.
    $string = urldecode($string);

    // Remove '[]' from facet query for funnelback.
    $string = preg_replace("/\\[(.*?)\\]/", null, $string);

    // For search query.
    $string = str_replace("`", '', $string);
    $string = str_replace(' ', '+', $string);

    // Remove tags.
    $string = strip_tags($string);

    return $string;
  }

  /**
   * Find redirect url from curator link.
   *
   * @param $link_url
   * @return bool|string
   */
  public static function funnelbackFilterCuratorLink($link_url) {
    $url = '';
    $string_segments = explode('&', $link_url);
    foreach ($string_segments as $segment) {
      if (substr($segment,0,4) === 'url=') {
        $url = urldecode(substr($segment, 4, strlen($segment)));
      }
    }

    return $url;
  }

  /**
   * Remove specific query string from the raw query string array.
   *
   * @param $string
   *   Needle.
   * @param $queryString
   *   Raw query string array.
   * @return mixed
   */
  public static function funnelbackQueryRemove($string, &$queryString) {
    foreach ($queryString as $key => $value) {
      if (strpos($value, $string) !== FALSE) {
        unset($queryString[$key]);
      }
    }

    return $queryString;
  }
}