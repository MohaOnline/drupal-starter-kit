<?php
/**
 * @file
 * Contextual navigation template for Funnelback.
 *
 * Available variables:
 * - $summary: An array of result summary information
 * - $contextual_nav: An array of contextual navigation information
 */

if(!empty($contextual_nav)): ?>
<div id="funnelback-contextual-navigation">
    <h3>Related searches for <strong><?php print $summary['query']; ?></strong></h3>
      <?php foreach($contextual_nav as $category): ?>
          <div class="contextual-navigation-wrapper">
              <h4><?php print $category['name']; ?> of <strong><?php print $summary['query']; ?></strong></h4>
              <div class="funnelback-contextual-navigation-<?php print $category['name']; ?>">
                  <ul>
                    <?php foreach($category['clusters'] as $cluster): ?>
                        <li><a href="<?php print FunnelbackQueryString::funnelbackFilterSystemQueryString($cluster['link']); ?>"><?php print str_replace('...', ' <strong>' . $summary['query'] . '</strong> ', $cluster['title']); ?></a></li>
                    <?php endforeach ?>
                  </ul>
              </div>
          </div>
      <?php endforeach ?>
</div>
<?php endif; ?>