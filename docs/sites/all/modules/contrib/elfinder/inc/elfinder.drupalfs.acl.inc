<?php
/**
 * @file
 * elfinder ACL class
 */
class elFinderDrupalACL {

  public function fsAccessPublic($attr, $path, $data, $volume) {

    if (strpos(basename($path), '.') === 0 && $attr == 'hidden') {
      return TRUE;
    }

    /* Hiding thumbnail folder */
    if (strstr($path, DIRECTORY_SEPARATOR . variable_get('elfinder_settings_thumbnail_dirname', 'tmb')) && $attr == 'hidden') {
      return TRUE;
    }

    // Hide folders hidden by config settings.
    $f = drupal_realpath("public://") . "/";
    $folder = str_replace($f, "", $path);
    $hidden_folder = preg_split("/[\s,]+/", variable_get('elfinder_settings_misc_hidden_folders', ''));
    if (in_array($folder, $hidden_folder) && $attr == 'hidden') {
      return TRUE;
    }

    if (strstr($path, DIRECTORY_SEPARATOR . '.quarantine') && $attr == 'hidden') {
      return TRUE;
    }

    if ($attr == 'read') {
      return TRUE;
    }

    if ($attr == 'write') {
      return TRUE;
    }

    /* if ($attr == 'write' && user_access('write public files')) {
      return TRUE;
      }

      if ($attr == 'locked' && !user_access('write public files')) {
      return TRUE;
      } */

    return FALSE;
  }

  public function fsAccessPrivate($attr, $path, $data, $volume) {

    if (strpos(basename($path), '.') === 0 && $attr == 'hidden') {
      return TRUE;
    }

    /* Hiding thumbnail folder */
    if (strstr($path, DIRECTORY_SEPARATOR . variable_get('elfinder_settings_thumbnail_dirname', 'tmb')) && $attr == 'hidden') {
      return TRUE;
    }

    if (strstr($path, DIRECTORY_SEPARATOR . '.quarantine') && $attr == 'hidden') {
      return TRUE;
    }

    if ($attr == 'read') {
      return TRUE;
    }


    if ($attr == 'write') {
      return TRUE;
    }

    /* if ($attr == 'write' && user_access('write public files')) {
      return TRUE;
      }

      if ($attr == 'locked' && !user_access('write public files')) {
      return TRUE;
      } */


    return FALSE;
  }

  public function fsAccessUnmanaged($attr, $path, $data, $volume) {

    if (strpos(basename($path), '.') === 0 && $attr == 'hidden') {
      return TRUE;
    }

    /* Hiding thumbnail folder */
    if (strstr($path, DIRECTORY_SEPARATOR . variable_get('elfinder_settings_thumbnail_dirname', 'tmb')) && $attr == 'hidden') {
      return TRUE;
    }

    if (strstr($path, DIRECTORY_SEPARATOR . '.quarantine') && $attr == 'hidden') {
      return TRUE;
    }

    if ($attr == 'read') {
      return TRUE;
    }

    return FALSE;
  }

}