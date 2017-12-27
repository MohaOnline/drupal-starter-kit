<?php
/**
 * @file
 * Auxiliary functionality.
 */

/**
 * Scan a given path for files and set MD5 hash for each of them.
 *
 * @param string $directory
 *   Directory for scanning.
 * @param string[] $extensions
 *   Allowed file extensions.
 *
 * @return \stdClass[]
 *   An array with Drupal file objects.
 */
function file_md5_scan($directory = 'public://', $extensions = ['jpg', 'jpeg', 'png', 'gif', 'svg', 'csv']) {
  $files = [];

  /* @var \SplFileInfo $object */
  foreach (new \RecursiveIteratorIterator(new \RecursiveDirectoryIterator($directory)) as $path => $object) {
    if ($object->isFile() && in_array($object->getExtension(), $extensions)) {
      $file = file_md5_create_file($object);
      $files[$file->fid] = $file;
    }
  }

  return $files;
}

/**
 * Create Drupal file object from SplFileInfo.
 *
 * @see file_save_upload()
 *
 * @param \SplFileInfo $object
 *   File object.
 *
 * @throws \RuntimeException
 *   When file doesn't exists on a file system.
 *
 * @return \stdClass
 *   Drupal file object.
 */
function file_md5_create_file(\SplFileInfo $object) {
  global $user;

  $uri = $object->getPathname();

  if (!file_exists($uri)) {
    throw new \RuntimeException(t('File does not exist in at @uri.', [
      '@uri' => $uri,
    ]));
  }

  // Try to find an entry about file in the database (by URI and MD5 hash).
  $file = file_md5_load_file(file_md5_hash($uri));

  if (FALSE === $file) {
    $files = file_load_multiple([], ['uri' => $uri]);
    $file = reset($files);
  }

  // Create a new entry if file does not exist.
  if (empty($file)) {
    $file = new \stdClass();
    $file->uri = $uri;
    $file->uid = $user->uid;
    $file->status = FILE_STATUS_PERMANENT;
    $file->filename = $object->getFilename();
    $file->filemime = file_get_mimetype($uri);
    $file->filesize = filesize($uri);
  }

  // Set "md5" property if it is empty.
  if (empty($file->md5)) {
    // The "md5" property will be added by hook_file_presave().
    // @see file_md5_file_presave()
    file_save($file);
  }

  return $file;
}

/**
 * Generate a hash value using the contents of a given file.
 *
 * @param string $uri
 *   Path to file.
 *
 * @return string
 *   MD5 hash of a file.
 */
function file_md5_hash($uri) {
  return is_readable($uri) ? md5_file($uri) : md5($uri);
}

/**
 * Load file by ID or MD5 hash.
 *
 * @param string|int $argument
 *   File ID or MD5 hash.
 *
 * @return \stdClass
 *   Drupal file object.
 */
function file_md5_load_file($argument) {
  // What is 32?
  // @see _file_md5_schema_file_managed_field_definitions()
  if (strlen($argument) === 32) {
    $files = file_load_multiple([], ['md5' => $argument]);
  }
  else {
    $files = file_load_multiple([$argument]);
  }

  return reset($files);
}

/**
 * Delete file by ID or MD5 hash.
 *
 * @param string|int $argument
 *   File ID or MD5 hash.
 *
 * @return bool
 *   Deleting state.
 */
function file_md5_delete_file($argument) {
  $file = file_md5_load_file($argument);

  return FALSE === $file ?: file_delete($file);
}
