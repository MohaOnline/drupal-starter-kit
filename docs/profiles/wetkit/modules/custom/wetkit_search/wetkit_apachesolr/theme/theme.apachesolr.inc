<?php
/**
 * @file
 * Code for the WetKit Solr feature.
 */

/**
 * Theme the highlighted snippet text for a search entry.
 */
function wetkit_apachesolr_apachesolr_search_snippets($vars) {
  $max_snippets = variable_get('apachesolr_search_max_snippets', 5);
  $max_depth = $max_snippets * 10;
  $result = '';
  if (array_key_exists('snippets', $vars) && is_array($vars['snippets'])) {
    // Flatten the snippets array. Using a reference for the array value avoids
    // a copy of $vars['snippets'] by foreach.
    foreach ($vars['snippets'] as $key => & $snippet) {
      if (is_array($snippet)) {
        $vars['snippets'] = array_merge($vars['snippets'], array_values($snippet));
        unset($vars['snippets'][$key]);
        if (--$max_depth <= 0) {
          // Avoid endless loop if someone passes in endless arrays of arrays.
          break;
        }
      }
      elseif (is_string($snippet) && --$max_snippets <= 0) {
        break;
      }
    }
    unset($snippet);
    $result .= implode(' ... ', array_unique(array_filter($vars['snippets'], 'is_string')));
  }
  return $result . ' ...';
}

/**
 * Customize the search snippets file output.
 */
function wetkit_apachesolr_apachesolr_search_snippets__file($vars) {
  $doc = $vars['doc'];
  $snippets = $vars['snippets'];
  $parent_entity_links = array();

  // Retrieve our parent entities. They have been saved as
  // a small serialized entity.
  foreach ($doc->zm_parent_entity as $parent_entity_encoded) {
    $parent_entity = (object) drupal_json_decode($parent_entity_encoded);
    $parent_entity_uri = entity_uri($parent_entity->entity_type, $parent_entity);
    $parent_entity_uri['options']['absolute'] = TRUE;
    $parent_label = entity_label($parent_entity->entity_type, $parent_entity);
    $parent_entity_links[] = l($parent_label, $parent_entity_uri['path'], $parent_entity_uri['options']);
  }

  if (module_exists('file')) {
    $file_type = t('!icon @filemime', array(
      '@filemime' => $doc->ss_filemime, '!icon' => theme('file_icon', array(
        'file' => (object) array(
          'filemime' => $doc->ss_filemime),
      ))
    ));
  }
  else {
    $file_type = t('@filemime', array('@filemime' => $doc->ss_filemime));
  }

  return implode(' ... ', $snippets) . '<span>' . $file_type . '<em>' . t('attached to:') . '</em>' . implode(', ', $parent_entity_links) . '</span>';
}

/**
 * Brief message to display when no results match the query.
 *
 * @see search_help()
 */
function wetkit_apachesolr_apachesolr_search_noresults() {
  return t('<ul>
<li>Check if your spelling is correct, or try removing filters.</li>
<li>Remove quotes around phrases to match each word individually: <em>"blue drop"</em> will match less than <em>blue drop</em>.</li>
<li>You can require or exclude terms using + and -: <em>big +blue drop</em> will require a match on <em>blue</em> while <em>big blue -drop</em> will exclude results that contain <em>drop</em>.</li>
</ul>');
}
