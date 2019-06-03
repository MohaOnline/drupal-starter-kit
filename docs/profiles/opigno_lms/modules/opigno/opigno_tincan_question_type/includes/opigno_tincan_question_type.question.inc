<?php

/**
 * @file
 * This file contains the class OpignoTincanQuestionTypeQuestion.
 */

use TinCan\Util;

/**
 * This class goal is to provide specific TinCan question information.
 */
class OpignoTincanQuestionTypeQuestion extends QuizQuestion {

  /***************
   *
   * CONSTANTS
   *
   **************************/

  const SESSIONKEY_FILE = 'opigno_tincan_question_type_fid';
  const SESSIONKEY_REGISTRATION = 'opigno_tincan_question_type_registration';
  const PATH_PUBLIC_PACKAGE_FOLDER = 'public://opigno_tincan/';
  const PATH_PUBLIC_EXTRACTED_PACKAGE_FOLDER = 'public://opigno_tincan_extracted/';
  const SCORE_MAX = 50;


  /***************
   *
   * OVERRIDDEN METHODS
   *
   **************************/

  /**
   * Save question type specific node properties.
   *
   * @param bool $is_new
   *   Indicate if the node is a new one or an update.
   */
  public function saveNodeProperties($is_new = FALSE) {
    // Check first if the file is a new one or an updated one.
    // If the file didn't change, leave this method.
    if (isset($this->node->fid)) {
      $is_file_updated = ($this->node->fid != $this->node->file);
      if (!$is_new && !$is_file_updated) {
        return;
      }
    }
    else {
      $is_file_updated = FALSE;
    }

    if (!isset($_SESSION[self::SESSIONKEY_FILE])) {
      form_set_error('file', t('Error while uploading the file.'));
      return;
    }

    $file = file_load($_SESSION[self::SESSIONKEY_FILE]);
    if (!$file) {
      form_set_error('file', t('Error while opening the file.'));
      return;
    }

    $package_info = self::getInfoFromExtractedPackage($file);
    if ($package_info === FALSE) {
      form_set_error(
        'file',
        t('The package does not contain an activity ID or a launch file')
      );
      return;
    }

    // Delete the old file if there is a new one.
    if ($is_file_updated) {
      $old_file = file_load($this->node->fid);
      if ($old_file) {
        file_usage_delete($old_file, 'opigno_tincan_question_type', 'node', $this->node->nid);
        file_delete($old_file);
      }
    }

    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'opigno_tincan_question_type', 'node', $this->node->nid);

    $activity_id = $package_info['id'];
    $launch_filename = $package_info['launch'];

    db_merge(OpignoTincanQuestionTypePropertiesDatabase::NAME)
      ->key(array('nid' => $this->node->nid, 'vid' => $this->node->vid))
      ->fields(array(
        'fid' => $_SESSION[self::SESSIONKEY_FILE],
        'activity_id' => $activity_id,
        'launch_filename' => $launch_filename,
      ))
      ->execute();
  }

  /**
   * Getter function returning properties to be loaded when the node is loaded.
   *
   * @see quiz_question_load()
   *
   * @return array
   *   The node properties
   */
  public function getNodeProperties() {
    if (isset($this->nodeProperties['fid'])) {
      return $this->nodeProperties;
    }

    parent::getNodeProperties();
    $result = db_select(OpignoTincanQuestionTypePropertiesDatabase::NAME, 't')
      ->condition('nid', $this->node->nid)
      ->condition('vid', $this->node->vid)
      ->fields('t')
      ->range(0, 1)
      ->execute()->fetchAssoc();

    if ($result) {
      $this->nodeProperties += $result;
    }

    return $this->nodeProperties;
  }

  /**
   * Provides validation for question before it is created.
   *
   * When a new question is created and initially submited, this is
   * called to validate that the settings are acceptable.
   *
   * @param array $form
   *   The processed form.
   */
  public function validateNode(array &$form) {
    $file = file_load($form['file']['#value']['fid']);

    if ($file && $file->status != FILE_STATUS_PERMANENT) {
      if ($this->unzipPackage($file)) {
        $package_info = self::getInfoFromExtractedPackage($file);
        if ($package_info === FALSE) {
          form_set_error(
            'file',
            t('The package does not contain the activity ID or the launch file')
          );
        }
        else {
          $_SESSION[self::SESSIONKEY_FILE] = $form['file']['#value']['fid'];
        }
      }
      else {
        form_set_error('file', t('Unable to open the archive'));
      }
    }
  }

  /**
   * Get the form used to create a new question.
   *
   * @param array $form_state
   *   FAPI form state.
   *
   * @return array
   *   Must return a FAPI array.
   */
  public function getCreationForm(array &$form_state = NULL) {
    if (isset($this->node->nid) && !isset($this->nodeProperties['fid'])) {
      $this->getNodeProperties();
    }

    return array(
      'file' => array(
        '#type' => 'managed_file',
        '#title' => t('TinCan package'),
        '#default_value' => $this->nodeProperties['fid'],
        '#upload_location' => self::PATH_PUBLIC_PACKAGE_FOLDER,
        '#upload_validators' => array(
          'file_validate_extensions' => array('zip'),
        ),
        '#required' => TRUE,
      ),
    );
  }

  /**
   * Get the maximum possible score for this question.
   */
  public function getMaximumScore() {
    return self::SCORE_MAX;
  }

  /**
   * Implementation of delete().
   *
   * @see QuizQuestion#delete($only_this_version)
   */
  public function delete($only_this_version = FALSE) {
    parent::delete($only_this_version);

    if (!isset($this->nodeProperties['fid'])) {
      $this->getNodeProperties();
    }

    $file = file_load($this->nodeProperties['fid']);
    if ($file) {
      file_usage_delete($file, 'opigno_tincan_question_type', 'node', $this->node->nid);
      file_delete($file);
    }
  }

  /**
   * Allow question types to override the body field title.
   *
   * @return string
   *   The title for the body field.
   */
  public function getBodyFieldTitle() {
    return t('Tincan package description');
  }

  /**
   * Get the form through which the user will answer the question.
   *
   * @param array $form_state
   *   The FAPI form_state array.
   * @param int $rid
   *   The result id.
   *
   * @return array
   *   Must return a FAPI array.
   */
  public function getAnsweringForm(array $form_state, $rid) {
    global $user, $base_path;

    // Init the variables.
    $this->getNodeProperties();

    $markup = '';

    $launch_file = $this->nodeProperties['launch_filename'];

    // Check first if the TinCanPHP library is installed
    // If not, return nothing but an error message
    $libraries = libraries_get_libraries();
    if (!isset($libraries['TinCanPHP'])) {
      drupal_set_message(t(
        'Please install the !tincanphp_library in the <em>sites/all/library/TinCanPHP</em> folder.',
        array('!tincanphp_library' => l(t('TinCanPHP library'), 'https://github.com/RusticiSoftware/TinCanPHP/releases'))
      ), 'error');
      return array();
    }

    // Connect to the LRS.
    $lrs_endpoint = variable_get('opigno_tincan_api_endpoint', '');
    $lrs_username = variable_get('opigno_tincan_api_username', '');
    $lrs_password = variable_get('opigno_tincan_api_password', '');

    if (
      (empty($lrs_endpoint) || empty($lrs_username) || empty($lrs_password))
      && drupal_valid_path('admin/opigno/system/tincan')
    ) {
      drupal_set_message(t('The module Opigno TinCan API is not configured. Go to !url', array('!url' => l(t('the settings page.'), 'admin/opigno/system/tincan'))));
      unset($_SESSION[self::SESSIONKEY_REGISTRATION]);
    }
    else {
      // If connected to the LRS, create the URI parameters.
      $auth = 'Basic ' . base64_encode($lrs_username . ':' . $lrs_password);

      $actor = array(
        'mbox_sha1sum' => sha1('mailto:' . $user->mail),
        'name' => $user->name,
      );

      // Get the registration UUID that will be used to recognise this answering
      // form. If the registration UUID does not exist, create one.
      $registration = OpignoTincanQuestionTypeResponse::getRegistration($rid);
      if ($registration === FALSE) {
        $registration = Util::getUUID();
      }

      $launch_file .=
        '?endpoint=' . rawurlencode($lrs_endpoint) .
        '&auth=' . rawurlencode($auth) .
        '&actor=' . rawurlencode(json_encode($actor)) .
        '&registration=' . rawurlencode($registration);

      $_SESSION[self::SESSIONKEY_REGISTRATION] = $registration;
    }

    // Get the file and construct his URI.
    $file = file_load($this->nodeProperties['fid']);
    $filename = $this->getPackageName($file);
    $url = file_create_url(self::PATH_PUBLIC_EXTRACTED_PACKAGE_FOLDER . $filename);

    $markup .= '
      <iframe
        style="width: 100%; min-height: 800px; border: 0;"
        src="' . $url .'/'. $launch_file . '">
      </iframe>';

    // Construct the form.
    $form = parent::getAnsweringForm($form_state, $rid);

    $form['iframe'] = array(
      '#type' => 'markup',
      '#markup' => $markup,
    );

    return $form;
  }


  /***************
   *
   * PROTECTED METHODS
   *
   **************************/

  /**
   * This method will unzip the package to the public extracted folder.
   *
   * It will use the constants self::PATH_PUBLIC_EXTRACTED_PACKAGE_FOLDER.
   *
   * @param object $file
   *   The file to unzip.
   *
   * @return bool
   *   TRUE if success, else FALSE.
   *
   * @throws \Exception
   *   If the file is unsupported.
   */
  protected function unzipPackage($file) {
    $archiver = archiver_get_archiver($file->uri);

    if (!$archiver) {
      return FALSE;
    }
    else {
      $archiver->extract(self::getExtractPath($file));
      return TRUE;
    }
  }

  /**
   * This method will return the Activity ID and the launch file.
   *
   * These information must be in the tincan.xml file that is in the extracted
   *   package. You can find more information in the README.md file of this
   *   module.
   *
   * @param object $file
   *   The file that was unzipped.
   *
   * @return array|bool
   *   An array('id', 'launch') if all the information are found, FALSE if not.
   */
  public static function getInfoFromExtractedPackage($file) {
    $tincan_file = self::getExtractPath($file) . 'tincan.xml';

    if (!file_exists(self::getExtractPath($file) . 'tincan.xml')) {
      return FALSE;
    }

    $xml = simplexml_load_file($tincan_file);
    if (!$xml) {
      return FALSE;
    }

    // Check if the launch exists.
    if (!isset($xml->activities->activity->launch)) {
      return FALSE;
    }

    // Check if the activity ID exists.
    if (!isset($xml->activities->activity['id'])) {
      return FALSE;
    }

    return array(
      'launch' => (string) $xml->activities->activity->launch,
      'id' => (string) $xml->activities->activity['id'],
    );
  }

  /**
   * This method gives the path to the extracted package.
   *
   * @param object $file
   *   The extracted file.
   *
   * @return string
   *   The path to the extracted package.
   */
  public static function getExtractPath($file) {
    $filename = self::getPackageName($file);
    return self::PATH_PUBLIC_EXTRACTED_PACKAGE_FOLDER . $filename . '/';
  }

  /**
   * Gives the package name, after being renamed by the system if it happened.
   *
   * @param object $file
   *   The package file.
   *
   * @return string
   *   The package name.
   */
  public static function getPackageName($file) {
    return pathinfo($file->uri, PATHINFO_FILENAME);
  }

}
