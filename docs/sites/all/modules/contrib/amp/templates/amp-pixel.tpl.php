<?php

/**
 * @file
 * Template for an amp-pixel.
 *
 * Available variables:
 * - domain: The domain name for the amp-pixel.
 * - query_string: The path for the query string.
 * - subs: An array of possible variable substitutions
 *
 * @see template_preprocess_amp_pixel()
 */
?>
<?php if (!empty($domain) && !empty($query_string)): ?>
  <?php
    $src = 'https://' . $domain . '/' . $query_string . '?';
    $activeSubs = array();

    foreach ($subs as $sub => $subDetails) {
      if ($subDetails['active'] === TRUE) {
        $activeSubs = array_merge($activeSubs, array($sub => $subDetails));
      }
    }
  ?>

  <?php if (($activeSubCount = count($activeSubs)) !== 0): ?>
    <?php
      $activeSubKeys = array_keys($activeSubs);
      for ($i = 0; $i < $activeSubCount; $i++) {
        $activeKey = $activeSubKeys[$i];
        switch ($activeKey) {
          case 'AMPDOC_HOST':
            $src = $src . 'host=' . $activeKey;
            break;
          case 'AMPDOC_URL':
            $src = $src . 'ref=' . $activeKey;
            break;
          case 'CANONICAL_HOST':
            $src = $src . 'host=' . $activeKey;
            break;
          case 'CANONICAL_PATH':
            $src = $src . 'path=' . $activeKey;
            break;
          case 'CANONICAL_URL':
            $src = $src . 'href=' . $activeKey;
            break;
          case 'SOURCE_URL':
            $src = $src . 'href=' . $activeKey;
            break;
          case 'SOURCE_HOST':
            $src = $src . 'host=' . $activeKey;
            break;
          case 'DOCUMENT_CHARSET':
            $src = $src . 'charSet=' . $activeKey;
            break;
          case 'DOCUMENT_REFERRER':
            $src = $src . 'referrer=' . $activeKey;
            break;
          case 'TITLE':
            $src = $src . 'title=' . $activeKey;
            break;
          case 'VIEWER':
            $src = $src . 'viewer=' . $activeKey;
            break;
          case 'CONTENT_LOAD_TIME':
            $src = $src . 'contentLoadTime=' . $activeKey;
            break;
          case 'DOMAIN_LOOKUP_TIME':
            $src = $src . 'domainLookupTime=' . $activeKey;
            break;
          case 'DOM_INTERACTIVE_TIME':
            $src = $src . 'domInteractiveTime=' . $activeKey;
            break;
          case 'PAGE_DOWNLOAD_TIME':
            $src = $src . 'pageDownloadTime=' . $activeKey;
            break;
          case 'PAGE_LOAD_TIME':
            $src = $src . 'pageLoadTime=' . $activeKey;
            break;
          case 'REDIRECT_TIME':
            $src = $src . 'redirectTime=' . $activeKey;
            break;
          case 'SERVER_RESPONSE_TIME':
            $src = $src . 'serverResponseTime=' . $activeKey;
            break;
          case 'TCP_CONNECT_TIME':
            $src = $src . 'tcpConnectTime=' . $activeKey;
            break;
          case 'AVAILABLE_SCREEN_HEIGHT':
            $src = $src . 'availScreenHeight=' . $activeKey;
            break;
          case 'AVAILABLE_SCREEN_WIDTH':
            $src = $src . 'availScreenWidth=' . $activeKey;
            break;
          case 'BROWSER_LANGUAGE':
            $src = $src . 'lang=' . $activeKey;
            break;
          case 'SCREEN_COLOR_DEPTH':
            $src = $src . 'colorDepth=' . $activeKey;
            break;
          case 'VIEWPORT_HEIGHT':
            $src = $src . 'viewportHeight=' . $activeKey;
            break;
          case 'VIEWPORT_WIDTH':
            $src = $src . 'viewportHeight=' . $activeKey;
            break;
          case 'PAGE_VIEW_ID':
            $src = $src . $activeKey;
            break;
          case 'RANDOM':
            $src = $src . $activeKey;
            break;
          case 'TIMESTAMP':
            $src = $src . 'timestamp=' . $activeKey;
            break;
          case 'TOTAL_ENGAGED_TIME':
            $src = $src . $activeKey;
            break;
        }

        if ($i < ($activeSubCount - 1)) {
          $src = $src . '&';
        }
      }
    ?>

    <amp-pixel src="<?php print $src; ?>">
    </amp-pixel>

  <?php endif; ?>
<?php endif; ?>
