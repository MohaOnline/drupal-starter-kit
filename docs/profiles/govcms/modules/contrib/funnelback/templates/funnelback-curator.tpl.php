<?php
/**
 * @file
 * Best bets template.
 *
 * Available variable:
 * - $curator: An array of curator information
 */
?>
<?php if (!empty($curator['exhibits'])): ?>
  <div id="funnelback-curator">

    <?php foreach($curator['exhibits'] as $exhibit): ?>

      <div class="funnelback-exhibit">
        <h3><a href="<?php print FunnelbackQueryString::funnelbackFilterCuratorLink($exhibit['linkUrl']) ?>" title="<?php print $exhibit['displayUrl'] ?>"><?php print $exhibit['titleHtml'] ?></a></h3>
        <p class="desc"><?php print $exhibit['descriptionHtml'] ?></p>
      </div>

    <?php endforeach; ?>
  </div>
<?php endif; ?>
