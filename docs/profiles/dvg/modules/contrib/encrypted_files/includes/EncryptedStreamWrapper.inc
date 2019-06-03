<?php

/**
 * @file
 * Defines the EncryptedStreamWrapper class.
 */

/**
 * Handles encrypting and decrypting files written to
 * and read from Encrypted File's custom stream wrapper.
 *
 * Extends private file system's wrapper to keep files safe in private system.
 */
class EncryptedStreamWrapper extends DrupalPrivateStreamWrapper {

  /**
   * Instance URI as scheme://target.
   */
  protected $uri;

  /**
   * A generic resource handle.
   */
  public $handle = NULL;

  // Mode the file was opened with.
  protected $mode = NULL;

  // Encryption configuration to use.
  protected $encryptConfig = NULL;

  // A custom encryption configuration.
  protected static $encryptConfigCustom = NULL;

  // Important info about the file.
  protected static $fileInfo = array();

  /**
   * Returns full path to Encrypted Files subdirectory within private directory.
   *
   * @return string
   *   Directory path
   */
  public function getDirectoryPath() {
    return parent::getDirectoryPath() . '/' . ENCRYPTED_FILES_DIRECTORY;
  }

  /**
   * Opens a temporary stream in memory to store the unencrypted file.
   *
   * File is stored temporarily while Drupal is reading and writing it.
   */
  public function stream_open($uri, $mode, $options, &$opened_path) {
    // Create Encrypted Files directory if it doesn't exist.
    $ef_directory = $this->getDirectoryPath();
    if (!file_exists($ef_directory)) {
      mkdir($ef_directory, 0755);
    }
    // Load encryption configuration.
    $this->loadEncryptConfig();
    // Load resource location.
    $this->uri = $uri;
    $path = $this->getLocalPath();
    // Save the mode for later reference.
    $this->mode = $mode;
    // Load temp file as our handle.
    $this->handle = fopen('php://memory', 'w+b');
    // If file exists, decrypt and load it into memory.
    if (file_exists($path)) {
      $raw_file = file_get_contents($path);
      $decrypted_file = decrypt($raw_file);
      self::setFileInfo($decrypted_file, $uri);
      // Write to memory.
      fwrite($this->handle, $decrypted_file);
      rewind($this->handle);
    }
    // Set $opened_path.
    if ((bool) $this->handle && $options & STREAM_USE_PATH) {
      $opened_path = $path;
    }

    return (bool) $this->handle;
  }

  /**
   * Encrypts and writes the open file to disk, then closes the stream.
   */
  public function stream_close() {
    // If file mode we opened with is only for reading,
    // don't resave the file.
    if ((strpos($this->mode, 'r') !== FALSE) &&
        (strpos($this->mode, '+') === FALSE)) {
      fclose($this->handle);
      return;
    }
    // Encrypt file and save.
    rewind($this->handle);
    $file_contents = stream_get_contents($this->handle);
    file_put_contents($this->getLocalPath(), encrypt($file_contents, NULL, NULL, NULL, $this->encryptConfig));
    // Store important file info.
    self::setFileInfo($file_contents, $this->uri);
    // Close handle and reset manual key.
    fclose($this->handle);
  }

  /**
   * Overrides getExternalUrl().
   *
   * @return string
   *   The URI of an encrypted file.
   */
  public function getExternalUrl() {
    $path = str_replace('\\', '/', $this->getTarget());
    return url('system/' . ENCRYPTED_FILES_DIRECTORY . '/' .
      $path, array('absolute' => TRUE));
  }

  /**
   * Allows the encryption configuration to be overridden.
   *
   * NOTE: As a precaution, the configuration can only be manually set once
   * per file.
   *
   * @param string $encrypt_config
   *   A configuration to use for encryption instead of the default one.
   *
   * @return bool
   *   TRUE if key saved successfully, FALSE if not.
   */
  public static function setEncryptConfig($encrypt_config) {
    if (isset(self::$encryptConfigCustom)) {
      return FALSE;
    }
    self::$encryptConfigCustom = $encrypt_config;
    return TRUE;
  }

  public static function getEncryptConfig() {
    return self::$encryptConfigCustom;
  }

  /**
   * Returns file size for the given file.
   *
   * @param string $uri
   *   The URI of the file to retrieve info for
   *
   * @return array|bool
   *   Associative array with above info, or false if not found
   */
  public static function getFileInfo($uri) {
    return isset(self::$fileInfo[$uri]) ? self::$fileInfo[$uri] : FALSE;
  }

  /**
   * Stores important info about the file we're operating on.
   *
   * @param string $content
   *   The content of the file
   * @param string $name
   *   The filename
   */
  protected static function setFileInfo($content, $name) {
    self::$fileInfo[$name] = array(
      'size' => strlen($content),
    );
  }

  /**
   * Loads encryption configuration.
   */
  protected function loadEncryptConfig() {
    // If custom configuration was set, use it.
    if (!empty(self::$encryptConfigCustom)) {
      $this->encryptConfig = self::$encryptConfigCustom;
    }
    // Otherwise, use the default Encrypted Files configuration. If it is
    // not set, use the default Encrypt configuration.
    else {
      $this->encryptConfig = variable_get('encrypted_files_encryption_config', '');
    }
  }

}
