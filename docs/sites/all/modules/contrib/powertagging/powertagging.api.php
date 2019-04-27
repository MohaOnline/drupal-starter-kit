<?php

/**
 * @file
 * API documentation for PowerTagging.
 */

/**
 * Alter the PowerTagging tag list.
 *
 * Replace the default HTML output of the field formatter "Tag list" of a
 * "PowerTagging Tags" field with your custom HTML output.
 *
 * @param string $markup
 *   The markup to alter. This parameter will always be NULL at the beginning,
 *   set it to whatever you want the HTML output to look like.
 * @param array $context
 *   An array of context variables possibly required to alter the tag list.
 *   Available array keys are:
 *   - items "items" => parameter "items" of hook_field_formatter_view()
 *   - items "entity" => parameter "entity" of hook_field_formatter_view()
 *   - string "langcode" => parameter "langcode" of hook_field_formatter_view()
 *   - int "instance" => parameter "instance" of hook_field_formatter_view()
 *
 * @see powertagging_field_formatter_view()
 * @see hook_field_formatter_view()
 */
function hook_powertagging_tag_list_alter(&$markup, $context) {
}

/**
 * Customize the ouput of the PowerTagging Tag Glossary block.
 *
 * @param array $taxonomy_terms
 *   Array of taxonomy term objects of tags to display
 * @param array $counts
 *   Associative array of how often a tag was already used per taxonomy term ID
 *   (tid => $count)
 *
 * @return string
 *   The HTML output of the block.
 */
function hook_powertagging_tag_glossary_output($taxonomy_terms, $counts) {
  $block_html = '<div id="custom_powertagging_glossary_terms">';
  foreach (array_keys($counts) as $tid) {
    if (isset($taxonomy_terms[$tid])) {
      $term = $taxonomy_terms[$tid];
      $block_html .= '<div class="custom_powertagging_glossary_terms_term">';
      $block_html .= '<h3>' . $term->name . '</h3>';
      if (!empty($term->description)) {
        $block_html .= '<p>' . $term->description . '</p>';
      }
      $block_html .= '</div>';
    }
  }
  $block_html .= '</div>';

  return $block_html;
}

/**
 * Do something before a PowerTagging configuration gets deleted.
 *
 * @param int $powertagging_id
 *   The ID of the PowerTagging configuration that gets deleted.
 */
function hook_powertagging_config_delete($powertagging_id) {
}
