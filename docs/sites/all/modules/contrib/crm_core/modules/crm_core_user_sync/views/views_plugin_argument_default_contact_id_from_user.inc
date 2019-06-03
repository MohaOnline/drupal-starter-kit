<?php
/**
 * @file
 * Contains the argument default plugin to fetch contact ID from current user.
 */

/**
 * Default argument plugin to extract contact ID from current user.
 */
class views_plugin_argument_default_contact_id_from_user extends views_plugin_argument_default {

  function get_argument() {
    $contact = crm_core_user_sync_get_contact_from_uid($GLOBALS['user']->uid);
    if ($contact) {
      return $contact->contact_id;
    }
  }
}
