<?php
/**
 * Output for the e2t_selector component in the results table.
 */
?>
<dl class="campaignion-email-to-target-message">
  <dt><?php echo t('To')?></dt><dl><?php echo $element['#data']['target']['salutation']; ?></dl>
  <dt><?php echo t('Party')?></dt><dl><?php echo $element['#data']['target']['political_affiliation']; ?></dl>
  <dt><?php echo t('Constituency')?></dt><dl><?php echo $element['#data']['constituency']['name']; ?></dl>
  <dt><?php echo t('Devolved country')?></dt><dl><?php echo $element['#data']['constituency']['country']['name']; ?></dl>
</dl>
