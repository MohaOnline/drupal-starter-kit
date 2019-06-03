<?php

namespace Drupal\campaignion_manage\BulkOp;

use \Drupal\campaignion\ContactTypeManager;

class SupporterExportBatch extends BatchBase {
  protected $exporter;
  protected $file = NULL;
  protected $fields;
  protected $filename;

  public function __construct(&$data) {
    $this->fields = $data['fields'];
    $this->filename = $data['csv_name'];
    $this->exporter = ContactTypeManager::instance()->exporter('csv');
    $this->exporter->filterColumns($data['fields']);
  }

  public function start(&$context) {
    if (!($handle = fopen($this->filename, 'a'))) {
      $context['results']['errors'] = t('Couldn\'t open temporary file to export supporters.');
    }
    $this->file = $handle;
  }

  public function apply($contact, &$result) {
    $this->exporter->setContact($contact);
    fputcsv($this->file, $this->exporter->row());
  }

  public function commit() {
    fclose($this->file);
  }
}
