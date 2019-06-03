<?php

/**
 * @file
 * Default theme implementation for displaying a single Google search result.
 *
 * This template renders a single search result and is collected into
 * gss-results.tpl.php. This and the parent template are
 * dependent to one another sharing the markup for definition lists.
 *
 * Available variables:
 * - $url: URL of the result.
 * - $title: Title of the result.
 * - $snippet: A small preview of the result. Does not apply to user searches.
 * - $thumbnail_url: The url of the image thumbnail
 * - $info: String of all the meta information ready for print. Does not apply
 *   to user searches.
 * - $info_split: Contains same data as $info, split into a keyed array.
 *
 * Default keys within $info_split:
 * - $info_split['type']: Node type (or item type string supplied by module).
 * - $info_split['user']: Author of the node linked to users profile. Depends
 *   on permission.
 * - $info_split['date']: Last update of the node. Short formatted.
 *
 *
 * Since $info_split is keyed, a direct print of the item is possible.
 * This array does not apply to user searches so it is recommended to check
 * for its existence before printing. The default keys of 'type', 'user' and
 * 'date' always exist for node searches. Modules may provide other data.
 * @code
 *   <?php if (isset($info_split['type'])) : ?>
 *     <span class="info-type">
 *       <?php print $info_split['type']; ?>
 *     </span>
 *   <?php endif; ?>
 * @endcode
 *
 * To check for all available data within $info_split, use the code below.
 * @code
 *   <?php print '<pre>'. check_plain(print_r($info_split, 1)) .'</pre>'; ?>
 * @endcode
 *
 * @see template_preprocess_gss_result()
 */
 ?>
<li class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <h3 class="gss-title"<?php print $title_attributes; ?>>
    <a href="<?php print $url; ?>"><?php print $title; ?></a>
  </h3>
  <?php if ($thumbnail_url): ?>
    <img class="gss-thumbnail-image" src="<?php print $thumbnail_url; ?>" height="62"/>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <div class="gss-search-snippet-info">
    <?php if ($snippet) : ?>
      <p class="gss-search-snippet"<?php print $content_attributes; ?>><?php print $snippet; ?></p>
    <?php endif; ?>
    <?php if ($url) : ?>
      <p class="gss-search-url"><a href="<?php print $url; ?>"><?php print $url; ?></a></p>
    <?php endif; ?>
    <?php if ($info): ?>
      <p class="gss-info">
        <?php print $info; ?>
      </p>
    <?php endif; ?>
  </div>

</li>
