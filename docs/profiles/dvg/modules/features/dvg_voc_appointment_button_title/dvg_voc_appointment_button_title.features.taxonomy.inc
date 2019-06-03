<?php
/**
 * @file
 * dvg_voc_appointment_button_title.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function dvg_voc_appointment_button_title_taxonomy_default_vocabularies() {
  return array(
    'appointment_button_title' => array(
      'name' => 'Appointment button title',
      'machine_name' => 'appointment_button_title',
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
