<?php

/**
 * @file
 * Helper functions for the funnelback module.
 */

class Funnelback {

  protected $funnelback_title_max_length = 80;

  protected $funnelback_suggest_path = 's/suggest.json';

  protected $funnelback_api_path = 's/search.json';

  protected $collection;

  protected $profile;

  protected $number;

  protected $baseUrl;

  public function __construct($collection, $profile, $number, $baseUrl) {
    $this->collection     = $collection;
    $this->profile        = $profile;
    $this->number         = $number;
    $this->baseUrl        = $baseUrl;
  }

  /**
   * Cache search results.
   *
   * Need to keep a cache of the search results for the entire page duration,
   * so blocks can access it.
   */
  public static function funnelbackStaticResultsCache($results = NULL) {
    static $_results = NULL;
    if (is_array($results)) {
      $_results = $results;
    }

    return $_results;
  }

  /**
   * Calls the appropriate Funnelback web service interface.
   *
   * @param string $query
   *   The query.
   * @param int $start
   *   A start value.
   * @param string $partial_query
   *   Partial query for autocompletion.
   * @param array $facet_query
   *   An array of facet queries.
   * @param array $contextual_query
   *   An array of contextual queries.
   * @param \FunnelbackClient $funnelbackClient
   *   FunnelbackClient object.
   *
   * @return array|null
   *   An array of results when successful or NULL on failure.
   */
  public function funnelbackDoQuery($query, $start = 1, $partial_query = NULL, $facet_query = NULL, $contextual_query = NULL, $funnelbackClient = NULL, $custom_template = NULL) {

    $base_url = $this->funnelbackGetBaseUrl();

    // Set API paths.
    $api_path = $this->funnelback_api_path;

    $query = FunnelbackQueryString::funnelbackQueryNormaliser($query);

    $request_params = [
      'remote_ip' => ip_address(),
      'query' => $query,
      'start_rank' => $start,
      'collection' => $this->collection,
      'profile' => $this->profile,
    ];

    // Add custom template.
    if ($custom_template) {
      $request_params['form'] = $custom_template;
      $api_path = 's/search.html';
    }

    // Add facet query to request.
    if (is_array($facet_query)) {
      $request_params = array_merge($request_params, $facet_query);
    }

    // Add contextual query to request.
    if (is_array($contextual_query)) {
      $request_params = array_merge($request_params, $contextual_query);
    }

    // Compose autocomplete request.
    if (isset($partial_query)) {
      // It is from autocompletion request.
      $request_params = [
        'remote_ip' => ip_address(),
        'partial_query' => $partial_query,
        'collection' => $this->collection,
        'show' => $this->number,
        'fmt' => 'json++'
      ];
      // Set API paths.
      $api_path = $this->funnelback_suggest_path;
    }

    if (!empty($profile)) {
      $request_params['profile'] = $profile;
    }

    // Allow modules to modify the query parameters.
    drupal_alter('funnelback_query', $request_params);

    // Do the request.
    $response = $funnelbackClient->request($base_url, $api_path, $request_params);

    if ($response->code == 200) {
      $result = $this->funnelbackJsonQuery(drupal_json_decode($response->data), $base_url);
    }
    else {
      $funnelbackClient->debug('The search query failed due to "%error".', [
        '%error' => $response->code . ' ' . $response->error,
      ], WATCHDOG_WARNING);
      return FALSE;
    }

    // Allow modules to modify the query result.
    drupal_alter('funnelback_result', $result);

    return $result;
  }

  /**
   * Calls the Funnelback JSON web service interface.
   *
   * @param object $json
   *   A HTML response object.
   * @param string $base_url
   *   The base URL of this search.
   *
   * @return array|object
   *   An array containing results data.
   */
  public function funnelbackJsonQuery($json, $base_url) {

    if (!isset($json['response'])) {
      // This is the autocompletion response or custom template.
      $this->funnelbackStaticResultsCache([]);

      return $json;
    }

    $result = $json['response']['resultPacket'];

    if (!$result) {
      // Profile name not found.
      $this->funnelbackStaticResultsCache([]);

      return [];
    }

    // Load up the results summary.
    $summary = [];
    $summary['start']     = (int) $result['resultsSummary']['currStart'];
    $summary['end']       = (int) $result['resultsSummary']['currEnd'];
    $summary['page_size'] = (int) $result['resultsSummary']['numRanks'];
    $summary['total']     = (int) $result['resultsSummary']['totalMatching'];
    $summary['query']     = (string) $result['query'];
    $summary['base_url']  = $base_url;

    $spell = [];
    if (!empty($result['spell'])) {
      $suggestion         = [];
      $suggestion['url']  = $result['spell']['url'];
      $suggestion['text'] = $result['spell']['text'];
      $spell[]            = $suggestion;
    }

    $curator = $json['response']['curator'];

    $items = [];
    if (!empty($result)) {
      foreach ($result['results'] as $result_item) {
        $item = [];
        $title = $result_item['title'];
        if (strlen($title) > $this->funnelback_title_max_length) {
          $title = substr_replace($title, '&hellip;', $this->funnelback_title_max_length);
        }
        $item['title'] = $title;
        $item['date'] = (string) $result_item['date'];
        $item['summary'] = (string) $result_item['summary'];
        $live_url = (string) $result_item['liveUrl'];
        $item['live_url'] = $this->funnelbackTruncateUrl($live_url);
        $item['cache_url'] = (string) $result_item['cacheUrl'];
        $item['display_url'] = $result_item['displayUrl'];
        $item['metaData'] = $result_item['metaData'];
        if (isset($result_item['metaData']['nodeId'])) {
          $item['metaData']['nodeId'] = $result_item['metaData']['nodeId'];
        }
        else {
          $item['metaData']['nodeId'] = NULL;
        }

        $items[] = $item;
      }
    }

    // Load up the contextual navigation.
    $contextual_nav = [];
    if (!empty($result['contextualNavigation']['categories'])) {
      foreach ($result['contextualNavigation']['categories'] as $category) {
        $nav_item = [];
        $nav_item['name'] = $category['name'];
        if (!empty($category['more_link'])) {
          $nav_item['more_link'] = $category['more_link'];
        }

        $clusters = [];
        if (!empty($category['clusters'])) {
          foreach ($category['clusters'] as $cluster) {
            $clusters[] = [
              'title' => $cluster['label'],
              'count' => $cluster['count'],
              'link' => $cluster['href'],
            ];
          }
          $nav_item['clusters'] = $clusters;
        }

        $contextual_nav[] = $nav_item;
      }
    }

    // Load up the facet content.
    $facets = [];
    if (!empty($json['response']['facets'])) {
      $facets = $json['response']['facets'];
    }

    // Return the results.
    $results = [
      'summary' => $summary,
      'spell' => $spell,
      'curator' => $curator,
      'results' => $items,
      'contextual_nav' => $contextual_nav,
      'facets' => $facets,
      'facetExtras' => $json['response']['facetExtras'],
    ];

    $this->funnelbackStaticResultsCache($results);

    return $results;
  }


  /**
   * Return the base URL.
   *
   * @return string|null
   *   The base URL.
   */
  protected function funnelbackGetBaseUrl() {

    $base_url = rtrim($this->baseUrl, '/');

    return $base_url . '/';
  }

  /**
   * Make sure we only have non web files being displayed as file types.
   *
   * I.e. not html, cfm, etc.
   */
  protected function funnelbackCheckFiletype($type) {
    $accepted_types = ['pdf', 'xls', 'ppt', 'rtf', 'doc', 'docx'];
    if (in_array($type, $accepted_types)) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Truncate the display url so it displays on one line.
   */
  protected function funnelbackTruncateUrl($url) {
    // Split the url into bits so we can choose what to keep.
    $url_arr = parse_url($url);
    $host = $url_arr['host'];
    // Always keep the host.
    $max_len = $this->funnelback_title_max_length - strlen($host);
    $path    = $url_arr['path'];
    $query   = (!empty($url_arr['query'])) ? $url_arr['query'] : NULL;
    if (!empty($query)) {
      $path = $path . '?' . $query;
    }
    // Put elipsis in the middle of the path.
    $path_len = strlen($path);
    if ($path_len > $max_len) {
      $start = $max_len / 2;
      $trunc = $path_len - $max_len;
      $path  = substr_replace($path, '&hellip;', $start, $trunc);
    }

    return $host . $path;
  }

  /**
   * Removed unsupported display formats from facets array.
   *
   * @param $facets
   */
  public static function funnelbackFilterFacetDisplay(&$facets) {
    $supported_format = [
      'SINGLE_DRILL_DOWN',
      'CHECKBOX',
      'RADIO_BUTTON',
    ];

    foreach($facets as $key => $facet) {
      // Filter other display types out.
      if (!in_array($facet['guessedDisplayType'], $supported_format)) {
        unset($facets[$key]);
      }
    }
  }

  /**
   * Helper function to validate search result JSON.
   *
   * @param $results
   * @return bool
   */
  public static function funnelbackResultValidator($results) {
    $default_result_keys = [
      'results',
      'summary',
      'facets',
      'facetExtras',
      'spell',
      'curator',
    ];
    foreach ($default_result_keys as $key) {
      if (!in_array($key, array_keys($results))) {
        // Default key is not in results, custom template used.
        return FALSE;
      }
    }

    return TRUE;
  }
}
