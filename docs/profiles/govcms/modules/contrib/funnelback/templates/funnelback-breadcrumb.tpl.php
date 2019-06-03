<?php
/**
 * @file
 * Fannelback breadcrumb template.
 *
 * Available variables:
 * - $facets: An array of facet items. Each item contains related selection
 * details and urls.
 * - $facet_extras: A array of extra information about facets.
 * - $selected: Boolean to indicate if any facet filter is selected.
 */

if ($selected):
?>
<div id="funnelback-facets-breadcrumb"><span class="facets-breadcrumb-label">Refined by:</span>
    <ul class="facets-applied list-inline">
      <?php foreach($facets as $facet):
        if (!empty($facet['selectedValues'])): ?>
          <?php foreach($facet['selectedValues'] as $selectedValue): ?>
                <li>
                    <a class="selected-filter" href="<?php print FunnelbackQueryString::funnelbackFilterSystemQueryString($selectedValue['toggleUrl']); ?>" title="Remove <?php print $selectedValue['label']; ?>">x <?php print $selectedValue['label']; ?></a>
                </li>
          <?php endforeach; ?>
        <?php endif; ?>
      <?php endforeach; ?>
    </ul>
  <a class="clear-all-filters" href="<?php print FunnelbackQueryString::funnelbackFilterSystemQueryString($facet_extras['unselectAllFacetsUrl']); ?>" title="Remove all refinements">
    Clear all filters
  </a>
</div>
<?php endif; ?>