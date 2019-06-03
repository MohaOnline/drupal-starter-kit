<?php
/**
 * @file
 * Facet template for Funnelback.
 *
 * Available variable:
 * - $facets: An array of facet items. Each item contains related selection
 * details and urls.
 * - $query: The search key word string.
 */

if (!empty($facets)):
  foreach($facets as $facet): ?>
      <div class='facet'>
          <div class='facet-header'>
              <div class='facet-name'>Filter by <?php print $facet['name']; ?></div>
            <?php if ($facet['selected'] == TRUE): ?>
                <div class='facet-clear-all'><a href="<?php print FunnelbackQueryString::funnelbackFilterSystemQueryString($facet['unselectAllUrl']); ?>">Clear all</a></div>
            <?php endif ?>
          </div>
          <div class="facet-body">
              <ul>
                <?php foreach($facet['categories'] as $category): ?>
                  <?php foreach($category['values'] as $value): ?>
                        <li>
                          <?php switch($facet['guessedDisplayType']):
                            case 'CHECKBOX': ?>
                                <input type="checkbox" class="facet-checkbox" <?php print $value['selected'] == TRUE ? 'checked' : ''; ?> redirect="<?php print FunnelbackQueryString::funnelbackFilterSystemQueryString($value['toggleUrl']); ?>">
                              <?php break; ?>
                            <?php case 'RADIO_BUTTON': ?>
                                  <input type="radio" class="facet-radio" <?php print $value['selected'] == TRUE ? 'checked' : ''; ?> redirect="<?php print FunnelbackQueryString::funnelbackFilterSystemQueryString($value['toggleUrl']); ?>">
                              <?php break; ?>
                            <?php case 'SINGLE_DRILL_DOWN': ?>
                                  <?php print $value['selected'] == TRUE ? 'x' : ''; ?>
                              <?php break; ?>
                            <?php endswitch; ?>
                            <a href="<?php print FunnelbackQueryString::funnelbackFilterSystemQueryString($value['toggleUrl']); ?>"><span class="facet-item-label"><?php print $value['label'] ?></span> <span class="facet-item-count"><i>(<?php print $value['count']; ?>)</i></span></a>
                        </li>
                  <?php endforeach ?>
                <?php endforeach ?>
              </ul>
          </div>
      </div>
  <?php endforeach ?>

<?php endif; ?>
