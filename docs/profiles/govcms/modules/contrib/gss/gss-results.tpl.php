<?php

/**
 * @file search-results.tpl.php
 * Default theme implementation for displaying search results.
 *
 * This template collects each invocation of theme_gss_result(). This and
 * the child template are dependant to one another sharing the markup for
 * definition lists.
 *
 * Note that modules may implement their own search type and theme function
 * completely bypassing this template.
 *
 * Available variables:
 * - $search_results: All results as it is rendered through
 *   search-result.tpl.php
 *
 * @see template_preprocess_gss_results()
 */
?>
<span><?php print $head; ?></span>
<ol class="google-search-results">
  <?php print $search_results; ?>
</ol>
<?php print $pager; ?>
