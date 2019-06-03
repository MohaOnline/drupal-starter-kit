<?php
/**
 * @file
 * dvg_voc_owms_authority.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function dvg_voc_owms_authority_taxonomy_default_vocabularies() {
  return array(
    'owms_authority' => array(
      'name' => 'OWMS authority',
      'machine_name' => 'owms_authority',
      'description' => '',
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
