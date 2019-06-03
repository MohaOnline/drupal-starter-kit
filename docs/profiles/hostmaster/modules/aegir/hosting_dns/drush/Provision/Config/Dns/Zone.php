<?php

/**
 * @file
 * Representation of a DNS zonefile
 *
 * This is the internal representation of a zonefile. It can be
 * extended by other subclasses to implement various engines, but it
 * has its own internal storage (through
 * Provision_Config_Dns_Zone_Store).
 *
 * It assumes a certain structure in the records of the store.
 *
 * The zonefile's serial number is incremented automatically when the
 * file is written (in process()).
 *
 * @see drush_dns_provision_zone()
 * @see increment_serial()
 * @see Provision_Config_Dns_Zone_Store
 */
class Provision_Config_Dns_Zone extends Provision_Config_Dns {
  public $template = 'zone.tpl.php';
  public $description = 'Zone-wide DNS configuration';

  public $data_store_class = 'Provision_Config_Dns_Zone_Store';

  function filename() {
    return "{$this->data['server']->dns_zoned_path}/{$this->data['server']->deploy_zone}.zone";
  }

  function process() {
    parent::process();
    $records = $this->store->merged_records();
    $this->data['records'] = array();

    // Increment the serial.
    $serial = (isset($records['serial']) ? $records['serial'] : NULL);
    $this->store->records = NULL; // TODO: Make sure not to save any records. Ultimately remove the record_set's from the Controller.
    $this->store->records['serial'] = $this->data['serial'] = Provision_Service_dns::increment_serial($serial);

    $this->data['dns_email'] = str_replace('@', '.', $this->data['server']->admin_email);

    // Reconstruct the array keys.
    $zone = $this->data['server']->deploy_zone;
    $this->data['records'] = !empty(d()->dns_records[$zone]) ? d()->dns_records[$zone] : array();

    $new_records = array();
    foreach ($this->data['records'] as $key => &$records) {

      if ($key == '@') {
        $key = '';
      }

      $new_records[$key] = $records;
    }
    $this->data['records'] = $new_records;

    // Append double quotes to TXT records before writing them.
    foreach ($this->data['records'] as &$records) {
      foreach ($records as $key => $record) {
        if ($record['dns_type'] == 'TXT') {
          $records[$key]['dns_value'] = '"' . trim($record['dns_value']) . '"';
        }
      }
    }
  }

  function write() {
    // lock the store until we are done generating our config.
    $this->store->lock();

    if ($this->is_empty()) {
      $this->unlink();
    } else {
      parent::write();
      $this->store->write();
    }
    $this->store->close();
  }

  /**
   * this destroys this zonefile, without any checks
   *
   * It actually removes the zonefile, the internal storage and the
   * record in the server config.
   */
  function unlink() {
    $zone = $this->data['server']->deploy_zone;

    // remove the zonefile
    if (parent::unlink()) {
      // remove the master record
      // XXX: need to do this for slaves too
      $this->server->service('dns')->config('server')->record_del($zone)->write();
      // remove the zonefile storage
      $this->store->unlink();
    }
    $this->store->unlock();
  }

  /**
   * test to see if the
   */
  function is_empty() {
    $records = $this->store->merged_records();
    // if there is any record that is not SOA or NS, this is
    // considered empty
    if (empty($records)) {
      return TRUE;
    }
    foreach ($records as $name => $record) {
      if ($name != '@') {
        return FALSE;
      } else {
        foreach ($record as $type => $destination) {
          if ($type != 'SOA' && $type != 'NS' && !is_null($destination)) {
            return FALSE;
          }
        }
      }
    }
    return TRUE;
  }

}
