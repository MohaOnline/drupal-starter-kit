<?php
/**
 * elFinder Integration
 *
 * Copyright (c) 2010-2018, Alexey Sukhotin. All rights reserved.
 */

/**
 * @file
 *
 * elFinder driver for Drupal filesystem.
 *
 * @author Alexey Sukhotin
 * */
class elFinderVolumeDrupal extends elFinderVolumeLocalFileSystem {

  protected $DrupalFilesACL = NULL;

  /**
   * Create Drupal file object
   *
   * @param  string $path file path
   * @return object
   * @author Alexey Sukhotin
   * */
  protected function _drupalfileobject($path) {
    $uri = $this->drupalpathtouri($path);
    return elfinder_get_drupal_file_obj($uri);
  }

  /**
   * Convert path to Drupal file URI
   *
   * @param  string $path file path
   * @return string
   * @author Alexey Sukhotin
   * */
  public function drupalpathtouri($path) {

    $pvtpath = drupal_realpath('private://');
    $pubpath = drupal_realpath('public://');
    $tmppath = drupal_realpath('temporary://');

    $uri = '';

    if (strpos($path, $pvtpath) === 0) {
      $uri = 'private://' . substr($path, strlen($pvtpath) + 1);
    } elseif (strpos($path, $tmppath) === 0) {
      $uri = 'temporary://' . substr($path, strlen($tmppath) + 1);
    } else {
      $uri = 'public://' . substr($path, strlen($pubpath) + 1);
    }

    return @file_stream_wrapper_uri_normalize($uri);
  }

  /**
   * Check if file extension is allowed
   *
   * @param stdClass $file file object
   * @return array
   * @author Alexey Sukhotin
   **/
  protected function CheckExtension(stdClass $file) {

    $allowed_extensions = variable_get('elfinder_settings_filesystem_allowed_extensions', '');

    if (!empty($allowed_extensions)) {

      $errors = file_validate_extensions($file, $allowed_extensions);

      if (!empty($errors)) {
        $this->setError(strip_tags(implode(' ', $errors)));
        return FALSE;
      }

    }
    return TRUE;
  }

  /**
   * Create dir
   *
   * @param  string $path parent dir path
   * @param string $name new directory name
   * @return bool
   * @author Alexey Sukhotin
   * */
  protected function _mkdir($path, $name) {
    $path = $path . DIRECTORY_SEPARATOR . $name;

    if (@drupal_mkdir($path)) {
      return $path;
    }
    return FALSE;
  }

  /**
   * Create file
   *
   * @param  string $path parent dir path
   * @param string $name new file name
   * @return bool
   * @author Alexey Sukhotin
   * */
  protected function _mkfile($path, $name) {
    $path = $path . DIRECTORY_SEPARATOR . $name;
    $uri = $this->drupalpathtouri($path);

    if (!$this->CheckExtension($this->_drupalfileobject($path))) {
      return FALSE;
    }

    $file = file_save_data("", $uri);

    $this->FileUsageAdd($file);

    if (isset($file->fid)) {
      return $path;
    }

    return FALSE;
  }

  /**
   * Copy file into another file
   *
   * @param  string $source source file path
   * @param  string $targetDir target directory path
   * @param  string $name new file name
   * @return bool
   * @author Alexey Sukhotin
   * */
  protected function _copy($source, $targetDir, $name) {

    $target = $targetDir . DIRECTORY_SEPARATOR . (!empty($name) ? $name : basename($source));

    if (!is_dir($target) && !$this->CheckExtension($this->_drupalfileobject($target))) {
      return FALSE;
    }

    if (!$this->CheckUserQuota()) {
      return FALSE;
    }

    if (file_copy($this->_drupalfileobject($source), $this->drupalpathtouri($target))) {
      $this->FileUsageAdd($this->_drupalfileobject($target));
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Move file into another parent dir
   * Return new file path or false
   *
   * @param  string $source source file path
   * @param  string $target target dir path
   * @param  string $name new name
   * @return bool|string
   * @author Alexey Sukhotin
   * */
  protected function _move($source, $targetDir, $name) {

    $target = $targetDir . DIRECTORY_SEPARATOR . (!empty($name) ? $name : basename($source));

    if (!is_dir($target) && !$this->CheckExtension($this->_drupalfileobject($target))) {
      return FALSE;
    }

    if (is_dir($source)) {
      $srcuri = $this->drupalpathtouri($source);
      $dsturi = $this->drupalpathtouri($target);

      $children = db_select('file_managed', 'f')
        ->condition('uri', $srcuri . '/%', 'LIKE')
        ->fields('f', array('fid', 'uri'))
        ->execute()
        ->fetchAll();

      foreach ($children as $child) {
        $newuri = str_replace("$srcuri/", "$dsturi/", $child->uri);
        db_update('file_managed')->fields(array('uri' => $newuri))->condition('fid', $child->fid)->execute();
      }

      return @rename($source, $target);
    } elseif (@file_move($this->_drupalfileobject($source), $this->drupalpathtouri($target))) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Remove file
   *
   * @param  string $path file path
   * @return bool
   * @author Alexey Sukhotin
   * */
  protected function _unlink($path) {

    $file = $this->_drupalfileobject($path);
    $this->FileUsageDelete($file);

    $result = @file_delete($file);

    if ($result === TRUE) {
      return TRUE;
    }

    if (is_array($result)) {
      return $result['file'];
    } else {
      return FALSE;
    }

  }

  /**
   * Remove dir
   *
   * @param  string $path dir path
   * @return bool
   * @author Alexey Sukhotin
   * */
  protected function _rmdir($path) {
    return @drupal_rmdir($path);
  }

  /**
   * Delete dirctory trees and included files.
   *
   * Clone of elfinderVolumeDriver::delTree().
   *
   * Using elFinderVolumeLocalFileSystem::delTree to delete a folder with files
   * in it would not update file_usage and file_managed tables. Using
   * elfinderVolumeDriver::delTree makes it work better.
   */
  protected function delTree($localpath) {
    foreach ($this->_scandir($localpath) as $p) {
      elFinder::extendTimeLimit();
      $stat = $this->stat($this->convEncOut($p));
      $this->convEncIn();
      ($stat['mime'] === 'directory') ? $this->delTree($p) : $this->_unlink($p);
    }
    $res = $this->_rmdir($localpath);
    $res && $this->clearstatcache();
    return $res;
  }

  /**
   * Create new file and write into it from file pointer.
   * Return new file path or false on error.
   *
   * @param  resource $fp file pointer
   * @param  string $dir target dir path
   * @param  string $name file name
   * @return bool|string
   * @author Dmitry (dio) Levashov, Alexey Sukhotin
   * */
  protected function _save($fp, $dir, $name, $stat) {
    $tmpname = $name;

    $bu_ret = module_invoke_all('elfinder_beforeupload', array('name' => $name, 'dir' => $dir, 'stat' => $stat));

    if (isset($bu_ret)) {
      if (!is_array($bu_ret)) {
        $bu_ret = array($bu_ret);
      }

      $tmpname = end($bu_ret);
    }

    $path = $dir . DIRECTORY_SEPARATOR . (!empty($tmpname) ? $tmpname : $name);

    if (!$this->CheckUserQuota()) {
      return FALSE;
    }

    if (!$this->CheckFolderCount($dir)) {
      return FALSE;
    }

    if (!$this->CheckExtension($this->_drupalfileobject($path))) {
      return FALSE;
    }

    if (!$this->FileValidate($name)) {
      return FALSE;
    }

    if (!($target = @fopen($path, 'wb'))) {
      return FALSE;
    }

    while (!feof($fp)) {
      fwrite($target, fread($fp, 8192));
    }


    fclose($target);
    @chmod($path, $this->options['fileMode']);

    $file = $this->_drupalfileobject($path);

    @file_save($file);
    $this->FileUsageAdd($file);

    return $path;
  }

  protected function CheckUserQuota() {
    $space = $this->CalculateUserAllowedSpace();

    if ($space == 0) {
      $this->setError(t('Quota exceeded'));
      return FALSE;
    }

    return TRUE;
  }


  protected function CheckFolderCount($dir) {
    $max_allowed = variable_get('elfinder_settings_filesystem_maxfilecount', 0);
    if ($max_allowed > 0) {
      $options = array(
        'recurse' => FALSE,
      );
      // Match name.extension. This won't count files with no extension.
      $files = file_scan_directory($dir, '/.*\..*/', $options);

      if (count($files) >= $max_allowed) {
        $this->setError(t('Max directory file count of %count reached', array('%count' => $max_allowed)));
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Let other Drupal modules perform validation on the uploaded file.
   * See hook_file_validate().
   *
   * @param  string    $name file name
   * @return bool
   */
  protected function FileValidate($name) {
    // The uploaded file is still in temp. Fetch it's name & path from $_FILES.
    $index = array_search($name, $_FILES['upload']['name']);
    if ($index !== FALSE) {
      $file = $this->_drupalfileobject($_FILES['upload']['tmp_name'][$index]);
      $validation_errors = module_invoke_all('file_validate', $file);
      if (!empty($validation_errors)) {
        $this->setError(strip_tags(implode(' ', $validation_errors)));
        return FALSE;
      }
    } else {
      watchdog('elfinder', 'File upload "' . $name . '" not found in $_FILES');
    }
    return TRUE;
  }


  /**
   * Return files list in directory.
   *
   * @param  string $path dir path
   * @return array
   * @author Dmitry (dio) Levashov
   * */
  protected function _scandir($path) {
    $files = array();

    foreach (scandir($path) as $name) {
      if ($name != '.' && $name != '..') {
        $files[] = $path . DIRECTORY_SEPARATOR . $name;
      }
    }
    return $files;
  }

  public function owner($target) {
    $path = $this->decode($target);


    $file = $this->_drupalfileobject($path);

    if ($file->fid) {
      $owneraccount = user_load($file->uid);

      /* AS */
      $owner = $owneraccount->name;

      $ownerformat = variable_get('elfinder_settings_filesystem_owner_format', '');

      if ($ownerformat != '') {
        $owner = token_replace($ownerformat, array('user' => $owneraccount));
      }

      return $owner;
    }
    return FALSE;
  }

  public function desc($target, $newdesc = NULL) {
    $path = $this->decode($target);

    $file = $this->_drupalfileobject($path);

    if ($file->fid) {
      $finfo = db_select('elfinder_file_extinfo', 'f')
        ->condition('fid', $file->fid)
        ->fields('f', array('fid', 'description'))
        ->execute()
        ->fetchObject();

      $descobj = new StdClass;
      $descobj->fid = $file->fid;
      $descobj->description = $newdesc;

      if ($newdesc != NULL && user_access('edit file description')) {
        if (($rc = drupal_write_record('elfinder_file_extinfo', $descobj, isset($finfo->fid) ? array('fid') : array())) == 0) {
          return -1;
        }
      } else {
        return $finfo->description;
      }
    }
    return $newdesc;
  }

  public function downloadcount($target) {
    $path = $this->decode($target);

    $file = $this->_drupalfileobject($path);

    if ($file->fid && module_exists('elfinder_stats')) {
      $downloads = db_select('elfinder_stats', 's')
        ->fields('s', array('fid'))
        ->condition('s.fid', $file->fid)
        ->condition('s.type', 'download')
        ->countQuery()
        ->execute()
        ->fetchField();
      return $downloads ? $downloads : 0;
    }
    return 0;
  }

  protected function _archive($dir, $files, $name, $arc) {

    if (!$this->CheckUserQuota()) {
      return FALSE;
    }

    $ret = parent::_archive($dir, $files, $name, $arc);

    if ($ret != FALSE) {
      $file = $this->_drupalfileobject($ret);
      @file_save($file);
      $this->FileUsageAdd($file);
    }

    return $ret;
  }

  /**
   * Extract files from archive.
   *
   * Run the parent extract() then add the files to the Drupal db.
   *
   * @param string $hash
   *    Archive filename hash.
   * @param bool $makedir
   *    Extract the files into a new folder.
   * @return array|bool
   */
  public function extract($hash, $makedir = NULL) {
    if (!$this->CheckUserQuota()) {
      return FALSE;
    }

    $fstat = array();

    if ($makedir == NULL) {
      $fstat = parent::extract($hash);
    } else {
      $fstat = parent::extract($hash, $makedir);
    }

    if ($fstat != FALSE) {
      $path = $this->decode($fstat['hash']);
      $this->AddToDrupalDB($path);
      $file = $this->_drupalfileobject($path);
      if ($fstat['mime'] !== 'directory') {
        $this->FileUsageAdd($file);
      }
    }

    return $fstat;
  }

  /**
   * Recursive function to add new files to Drupal's db.
   *
   * TODO: If a file with the same name already exists anywhere else, this will
   * not create a new entry.
   */
  protected function AddToDrupalDB($files) {
    foreach($files AS $file) {
      if($file['mime'] == 'directory') {
        $newfiles = $this->scandir($file['hash']);
        $this->AddToDrupalDB($newfiles);
      } else {
        $filepath = $this->decode($file['hash']);
        $file_object = $this->_drupalfileobject($filepath);
        @file_save($file_object);
        $this->FileUsageAdd($file_object);
      }
    }
    return TRUE;
  }

  protected function CalculateUserAllowedSpace($checkuser = NULL) {
    global $user;

    $realUser = isset($checkuser) ? $checkuser : $user;

    $currentSpace = $this->CalculateUserUsedSpace($realUser);

    $maxSpace = isset($this->options['userProfile']->settings['user_quota']) ? parse_size($this->options['userProfile']->settings['user_quota']) : NULL;

    $diff = $maxSpace - $currentSpace;

    if (isset($maxSpace) && $maxSpace > 0) {

      if ($diff > 0) {
        return $diff;
      } else {
        return 0;
      }
    }

    return -1;
  }

  protected function CalculateUserUsedSpace($checkuser = NULL) {
    global $user;

    $realUser = isset($checkuser) ? $checkuser : $user;

    $q = db_query("SELECT sum(filesize) FROM {file_managed} WHERE uid = :uid", array(':uid' => $realUser->uid));

    $result = $q->fetchField();

    return $result;
  }

  protected function FileUsageAdd($file) {
    // Record that the module elfinder is using the file.
    @file_usage_add($file, 'elfinder', 'elfinderFileFetcher', 0); // 0 : means that there is no reference at the moment.
  }

  protected function FileUsageDelete($file) {
    // Delete record that the module elfinder is using the file.
    @file_usage_delete($file, 'elfinder', 'elfinderFileFetcher', 0); // 0 : means that there is no reference at the moment.
  }

  protected function _checkArchivers() {
    $this->archivers = variable_get('elfinder_settings_misc_archivers', array());

    if (count($this->archivers) == 0) {
      parent::_checkArchivers();
      variable_set('elfinder_settings_misc_archivers', $this->archivers);
    }
  }

  /**
   * Rename file and return file info
   *
   * @param  string  $hash  file hash
   * @param  string  $name  new file name
   * @return array|false
   **/
  public function rename($hash, $name) {

    $results = parent::rename($hash, $name);
    // Update any fields that point to this file.
    field_cache_clear();
    return $results;
  }

  /**
   * Taken from elFinderVolumeDriver::remove().
   *
   * Adds a message if the file is in use.
   */
  protected function remove($path, $force = false) {
    $stat = $this->stat($path);

    if (empty($stat)) {
      return $this->setError(elFinder::ERROR_RM, $path, elFinder::ERROR_FILE_NOT_FOUND);
    }

    $stat['realpath'] = $path;
    $this->rmTmb($stat);
    $this->clearcache();

    if (!$force && !empty($stat['locked'])) {
      return $this->setError(elFinder::ERROR_LOCKED, $this->path($stat['hash']));
    }

    if ($stat['mime'] == 'directory' && empty($stat['thash'])) {
      $ret = $this->delTree($this->convEncIn($path));
      $this->convEncOut();
      if (!$ret) {
        return $this->setError(elFinder::ERROR_RM, $this->path($stat['hash']));
      }
    } else {
      $results = $this->_unlink($this->convEncIn($path));
      if (!$results) {
        return $this->setError(elFinder::ERROR_RM, $this->path($stat['hash']));
      }
      if (is_array($results)) {
        // File is in use and is being protected by Drupal. Fetch the first
        // entity where it's used.
        foreach($results AS $entity_type => $entity) {
          if(is_array($entity)) {
            foreach($entity AS $id => $count) {
              if($entity_type == 'node' && is_integer($id)) {
                $node = node_load($id);
                if(!empty($node->title)) {
                  return $this->setError(elFinder::ERROR_RM, $this->path($stat['hash']), '', t('File is used in @title', array('@title' => $node->title)));
                }
              }
            }
          }
        }
        return $this->setError(elFinder::ERROR_RM, $this->path($stat['hash']), t('File is in use.'));
      }
      $this->clearstatcache();
    }
    $this->removed[] = $stat;
    return true;
  }



}
