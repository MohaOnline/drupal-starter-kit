<?php
/**
 * @file
 * dvg_voc_owms_uniform_product_name.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function dvg_voc_owms_uniform_product_name_taxonomy_default_vocabularies() {
  return array(
    'owms_uniform_product_name' => array(
      'name' => 'OWMS uniform product name',
      'machine_name' => 'owms_uniform_product_name',
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
