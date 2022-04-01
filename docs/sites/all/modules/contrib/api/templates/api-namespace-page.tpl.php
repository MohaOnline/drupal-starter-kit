<?php

/**
 * @file
 * Displays an API page for a namespace, including list of classes in it.
 *
 * Available variables:
 * - $alternatives: List of alternate versions (branches) of this namespace.
 * - $listing: Listing of classes, traits, and interfaces in the namespace.
 * - $branch: Object with information about the branch.
 * - $namespace: Namespace string.
 *
 * Available variables in the $branch object:
 * - $branch->project: The machine name of the branch.
 * - $branch->title: A proper title for the branch.
 * - $branch->directories: The local included directories.
 * - $branch->excluded_directories: The local excluded directories.
 *
 * @see api_preprocess_api_namespace_page()
 *
 * @ingroup themeable
 */
?>

<?php print $alternatives; ?>

<?php print $listing; ?>
