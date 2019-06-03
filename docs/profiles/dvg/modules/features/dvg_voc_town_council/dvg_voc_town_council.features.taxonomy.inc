<?php
/**
 * @file
 * dvg_voc_town_council.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function dvg_voc_town_council_taxonomy_default_vocabularies() {
  return array(
    'town_council' => array(
      'name' => 'Town council',
      'machine_name' => 'town_council',
      'description' => 'Coalition, opposition and commissions etc',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}
