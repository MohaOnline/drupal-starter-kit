<?php

/**
 * @file
 * Template for amp-analytics.
 *
 * Available variables:
 * - content: The json output.
 * - analytics_attributes: The HTML attributes for amp-analytics, primarily:
 *   - type: The type of analytics account.
 *
 * @see template_preprocess_amp_analytics()
 */
?>
<amp-analytics <?php print $analytics_attributes; ?>>
<script type="application/json">
  <?php print $content; ?>
</script>
</amp-analytics>
