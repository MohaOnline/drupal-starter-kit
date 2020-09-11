<?php

/**
 * @file
 * Theme callback preprocess functions for the campaignion_language_switcher.
 */

/**
 * Prepares variables for the campaignion language switcher.
 *
 * Default template: campaignion-language-switcher.tpl.php.
 *
 * The logic is (and should be) mostly identical to theme_links().
 *
 * @see theme_links()
 *
 * @param array $variables
 *   An associative array containing:
 *   - links: An array of links similar to theme_links().
 *   - provider: Machine name of a translation provider.
 */
function template_preprocess_campaignion_language_switcher(array &$variables) {
  global $language_url;
  $links = &$variables['links'];

  foreach ($links as $key => &$link) {
    $link['li_attributes']['class'][] = $key;

    if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page()))
        && (empty($link['language']) || $link['language']->language == $language_url->language)) {
      $link['li_attributes']['class'][] = 'active';
      $link['attributes']['class'][] = 'active';
    }

    $link['renderable'] = [];
    $link['html'] = !empty($link['html']);
    if (isset($link['href'])) {
      // Pass in $link as $options, they share the same keys.
      $link['renderable'] = [
        '#theme' => 'link',
        '#text' => $link['title'],
        '#path' => $link['href'],
        '#options' => $link,
      ];
    }
    elseif (!empty($link['title'])) {
      // Some links are actually not links, but we wrap these in <span> for
      // adding title and class attributes.
      $link['renderable'] = [
        '#theme' => 'html_tag',
        '#tag' => 'span',
        '#attributes' => $link['attributes'] ?? [],
        '#value' => $link['html'] ? $link['title'] : check_plain($link['title']),
      ];
    }
  }
  $variables['links_accessible'] = array_filter($links, function ($link) {
    return !empty($link['href']);
  });

  reset($links)['li_class'][] = 'first';
  end($links)['li_class'][] = 'last';
}
