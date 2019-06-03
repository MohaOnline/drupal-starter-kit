<?php

/**
 * @file
 * Hooks provided by the CRM Core Data Import module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter source rows before import.
 *
 * @param StdClass $row
 *   Row data.
 * @param string $machine_name
 *   Migration machine name.
 */
function hook_crm_core_data_import_source_row_alter(&$row, $machine_name) {

}

/**
 * Alter migration before import.
 *
 * @param MigrationDataImport $migration
 *   MigrationDataImport object.
 */
function hook_crm_core_data_import_migration_alter(&$migration) {

}

/**
 * Prepare entity before saving.
 *
 * @param object $entity
 *   Entity object.
 * @param StdClass $row
 *   Row data.
 * @param string $machine_name
 *   Migration machine name.
 */
function hook_crm_core_data_import_prepare_entity_alter(&$entity, &$row, $machine_name) {

}

/**
 * Alters entity list on rewind.
 *
 * @param array $entity_ids
 *   Entity IDs.
 * @param string $entity_type
 *   Entity type.
 * @param string $bundle
 *   Bundle.
 */
function hook_crm_core_data_import_civicrm_source_entity_list_alter($entity_ids, $entity_type, $bundle) {}
/**
 * @} End of "addtogroup hooks".
 */
