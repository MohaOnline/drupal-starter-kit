<?php

/**
 * @file
 * Ctools plugin that provides a Entity reference behavior for Entityqueue.
 */

$plugin = array(
  'title' => t('Entity queue'),
  'description' => t('Enforce entity queue settings such as minimum and maximum number of items.'),
  'class' => 'EntityReferenceBehavior_EntityQueue',
  'behavior type' => 'field',
);
