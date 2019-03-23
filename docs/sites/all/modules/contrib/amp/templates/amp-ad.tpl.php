<?php

/**
 * @file
 * Template for an amp-ad.
 *
 * Available variables:
 * - adtype: The ad network.
 * - height: The height of the ad.
 * - width: The width of the ad.
 * - slot_attributes: The slot attributes for the amp-ad.
 *
 * @see template_preprocess_amp_ad()
 */
?>
<amp-ad type="<?php print $adtype; ?>" height="<?php print $height; ?>" width="<?php print $width; ?>" <?php print $slot_attributes; ?>>
</amp-ad>
