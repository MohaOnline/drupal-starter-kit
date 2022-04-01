<?php

/**
 * @file
 * Base classes for tests for the API module.
 */

/**
 * Provides a base class for testing the API module.
 */
abstract class ApiTestCase extends DrupalWebTestCase {

  /**
   * Default set up: Sets up branch using API calls, removes PHP branch, parses.
   */
  public function setUp() {
    $this->baseSetUp();
    $this->setUpBranchApiCall();
    $this->removePhpBranch();

    $this->resetBranchesAndCache();
    $this->updateBranches();
    $count = $this->processApiParseQueue();
    $this->assertEqual($count, 11, "11 files were parsed ($count)");
  }

  /**
   * Sets up modules for API tests, and a super-user.
   *
   * @param array $extra_modules
   *   Extra modules to install.
   */
  protected function baseSetUp(array $extra_modules = array()) {
    $modules = array_merge(array(
      'api',
      'ctools',
      'composer_manager',
      'node',
      'comment',
      'dblog',
      'views',
    ), $extra_modules);
    DrupalWebTestCase::setUp($modules);

    $this->ensureAutoLoader();

    // Set the line break tag to nothing for most tests.
    variable_set('api_breaks_tag', '');

    // For debug purposes, visit the Recent Log Messages report page.
    $this->drupalGet('admin/reports/dblog');

    $this->verifyCounts(array(
      'api_project' => 0,
      'api_branch' => 0,
      'api_documentation' => 0,
      'node' => 0,
      'comment' => 0,
      'api_file' => 0,
      'api_function' => 0,
      'api_reference_storage' => 0,
      'api_overrides' => 0,
      'api_members' => 0,
      'api_php_branch' => 1,
      'api_php_documentation' => 0,
    ), 0, 'Immediately after install');

    // Set up a super-user.
    $super_user = $this->drupalCreateUser(array(
      'access API reference',
      'administer API reference',
      'access content',
      'access administration pages',
      'administer blocks',
      'administer nodes',
      'access site reports',
      'administer site configuration',
    ));
    $this->drupalLogin($super_user);
  }

  /**
   * Ensures that the Composer auto-loader is set up properly.
   *
   * If running tests locally on an API site, the Composer auto-loader will be
   * working automatically. But on the DrupalCI testing site, Composer is
   * using a different directory, and also hook_boot()/hook_init() don't get run
   * because the Composer Manager module is enabled after Drupal starts up. So,
   * this method ensures (hopefully) that whichever platform is running the
   * tests, the auto-loader gets found and initialized.
   *
   * Must be run after the composer_manager module is enabled.
   */
  protected function ensureAutoLoader() {
    // Test to see if one of the classes that the auto-loader should know
    // about has been defined.
    if (!class_exists('Symfony\Component\Yaml\Parser')) {
      // This most likely means that we are running on DrupalCI, so set
      // up for that.
      variable_set('composer_manager_vendor_dir', 'vendor');
      composer_manager_register_autoloader();
    }
  }

  /**
   * Sets up a project and a files branch using API function calls.
   *
   * @param string $prefix
   *   Directory prefix to prepend on the data directories.
   * @param bool $default
   *   TRUE to set this as the default branch; FALSE to not set it as default.
   * @param array $info
   *   Array of information to override the defaults (see function code to see
   *   what they are). Note that $prefix is applied after this information is
   *   read, and that only one directory and one excluded are supported in this
   *   function.
   *
   * @return array
   *   Array of information (defaults with overrides) used to create the
   *   branch and project.
   */
  protected function setUpBranchApiCall($prefix = '', $default = TRUE, array $info = array()) {
    module_load_include('inc', 'api', 'api.db');

    // Set up defaults.
    $info += array(
      'project' => 'test',
      'project_title' => 'Project 6',
      'project_type' => 'module',
      'branch_name' => '6',
      'title' => 'Testing 6',
      'core_compatibility' => '7.x',
      'update_frequency' => 1,
      'directory' => drupal_get_path('module', 'api') . '/tests/sample',
      'excluded' => drupal_get_path('module', 'api') . '/tests/sample/to_exclude',
      'regexps' => '',
    );
    $info['preferred'] = $default ? 1 : 0;

    // Create the project.
    $project = new stdClass();
    $project->project_type = $info['project_type'];
    $project->project_name = $info['project'];
    $project->project_title = $info['project_title'];
    api_save_project($project);
    if ($default) {
      // Make this the default project/compatibility.
      variable_set('api_default_project', $info['project']);
      variable_set('api_default_core_compatibility', $info['core_compatibility']);
    }

    // Create the branch.
    $branch = new stdClass();
    $branch->project = $info['project'];
    $branch->branch_name = $info['branch_name'];
    $branch->title = $info['title'];
    $branch->preferred = $info['preferred'];
    $branch->core_compatibility = $info['core_compatibility'];
    $branch->update_frequency = $info['update_frequency'];
    $branch->data = array(
      'directories' => $prefix . $info['directory'],
      'excluded_directories' => $prefix . $info['excluded'],
      'exclude_files_regexp' => $info['regexps'],
    );
    api_save_branch($branch);

    if ($default) {
      $this->assertEqual(variable_get('api_default_branch', 99), $branch->branch_id, 'Variable for default branch is set correctly');
    }

    return $info;
  }

  /**
   * Removes the PHP branch, which most tests do not need.
   */
  protected function removePhpBranch() {
    db_delete('api_php_branch')
      ->execute();
    api_get_php_branches(TRUE);
  }

  /**
   * Returns the first branch in the branches list.
   */
  protected function getBranch() {
    $branches = api_get_branches();
    return reset($branches);
  }

  /**
   * Asserts the right number of documentation objects are in the given branch.
   *
   * @param object|null $branch
   *   Branch object to look in. Omit to use the default branch.
   * @param int $num
   *   Number of objects to assert. Omit to use the current number that should
   *   be present for the default branch.
   */
  protected function assertObjectCount($branch = NULL, $num = 73) {
    if (is_null($branch)) {
      $branch = $this->getBranch();
    }

    $count = db_query("SELECT count(*) FROM {api_documentation} WHERE branch_id = :branch_id", array(':branch_id' => $branch->branch_id))->fetchField();

    $this->assertEqual($count, $num, 'Found ' . $count . ' documentation objects (should be ' . $num . ')');
  }

  /**
   * Makes sure all variables and branches have been reset.
   */
  protected function resetBranchesAndCache() {
    module_load_include('inc', 'api', 'api.db');
    cache_clear_all('variables', 'cache_bootstrap', 'cache');
    drupal_static_reset();
    variable_initialize();
    api_reset_branches();
  }

  /**
   * Updates branches and runs all update branch jobs.
   */
  protected function updateBranches() {
    module_load_include('inc', 'api', 'api.update_branch');

    api_update_all_branches();

    $queues = api_cron_queue_info();
    drupal_alter('cron_queue_info', $queues);

    $queue_name = 'api_branch_update';
    $info = $queues[$queue_name];
    $function = $info['worker callback'];
    $queue = DrupalQueue::get($queue_name);

    while ($item = $queue->claimItem()) {
      $function($item->data);
      $queue->deleteItem($item);
    }

    $this->resetBranchesAndCache();
  }

  /**
   * Processes the API parse queue.
   *
   * @param bool $verbose
   *   TRUE to print verbose output; FALSE (default) to omit.
   *
   * @return int
   *   Number of files parsed.
   */
  protected function processApiParseQueue($verbose = FALSE) {
    $queues = api_cron_queue_info();
    drupal_alter('cron_queue_info', $queues);

    $queue_name = 'api_parse';
    $info = $queues[$queue_name];
    $function = $info['worker callback'];
    $queue = DrupalQueue::get($queue_name);

    $count = 0;
    while ($item = $queue->claimItem()) {
      if ($verbose) {
        $this->verbose('Processing queue ' . $queue_name . ' - file ' . $item->data['path']);
      }
      $function($item->data);
      $queue->deleteItem($item);
      $count++;
    }

    module_load_include('inc', 'api', 'api.parser');
    api_shutdown();
    $this->resetBranchesAndCache();

    return $count;
  }

  /**
   * Returns the approximate number of items in the API parse queue.
   */
  protected function countParseQueue() {
    $queue = DrupalQueue::get('api_parse');
    return $queue->numberOfItems();
  }

  /**
   * Returns the number of files that have been marked as needing to be parsed.
   */
  protected function howManyToParse() {
    return db_query('SELECT COUNT(*) from {api_file} WHERE modified < :modified', array(':modified' => 100))->fetchField();
  }

  /**
   * Verifies the count of items in database tables and parse queue.
   *
   * @param array $counts
   *   Associative array whose keys are names of database tables, and whose
   *   values are the number of records expected to be in those database
   *   tables.
   * @param int $queue
   *   Number of items expected to be in the parse queue.
   * @param string $message
   *   String to append to assertion messages.
   */
  protected function verifyCounts(array $counts, $queue, $message) {
    // Add some generic tables to test along with main tables.
    if (isset($counts['node'])) {
      $counts['node_revision'] = $counts['node'];
      $counts['node_comment_statistics'] = $counts['node'];
    }
    if (isset($counts['comment'])) {
      $counts['field_data_comment_body'] = $counts['comment'];
      $counts['field_revision_comment_body'] = $counts['comment'];
    }

    foreach ($counts as $table => $expected) {
      $query = db_select($table, 'x');
      $query->addExpression('COUNT(*)');
      $actual = $query
        ->execute()
        ->fetchField();
      $this->assertEqual($actual, $expected, "Table $table has $expected records ($actual) - $message");
    }

    $actual = $this->countParseQueue();
    $this->assertEqual($actual, $queue, "Parse queue has $queue records ($actual) - $message");
  }

  /**
   * Checks the log for messages, and then clears the log.
   *
   * @param array $messages
   *   Array of messages to assert are in the log.
   * @param array $notmessages
   *   Array of messages to assert are not in the log.
   */
  protected function checkAndClearLog(array $messages = array(), array $notmessages = array()) {
    $this->drupalGet('admin/reports/dblog');
    foreach ($messages as $message) {
      $this->assertRaw($message, "Message $message appears in the log");
    }
    foreach ($notmessages as $message) {
      $this->assertNoRaw($message, "Message $message does not appear in the log");
    }
    $this->drupalPost(NULL, array(), 'Clear log messages');
  }

  /**
   * Asserts that code formatting did not change the code.
   *
   * @param string $formatted
   *   Formatted code to check.
   * @param string $file
   *   File name to read code from, to check against.
   */
  protected function assertCodeFormatting($formatted, $file) {
    $original = file_get_contents($file);

    // In formatted output, strip out the formatting tags, and then decode
    // HTML entities, which should get us back to the original HTML that was
    // in the file. Hopefully. That is what we're testing in this assert.
    $formatted = html_entity_decode(strip_tags($formatted));

    // Remove vertical whitespace. We used to remove spaces at ends of lines,
    // but now we test that there are none.
    $patterns = array(
      '|\n+|' => "\n",
    );
    foreach ($patterns as $pattern => $replace) {
      $original = preg_replace($pattern, $replace, $original);
      $formatted = preg_replace($pattern, $replace, $formatted);
    }

    // Trim and compare.
    $original = trim($original);
    $formatted = trim($formatted);
    $this->assertEqual($original, $formatted, "Formatted code matches code in $file");
  }

}

/**
 * Provides a base class for testing web pages (user/admin) for the API module.
 */
abstract class ApiWebPagesBaseTest extends ApiTestCase {

  /**
   * Array of branch information for the sample functions branch.
   *
   * @var array
   */
  protected $branchInfo;

  /**
   * Array of branch information for the sample PHP functions branch.
   *
   * @var array
   */
  protected $phpBranchInfo;

  /**
   * Overrides ApiTestCase::setUp().
   *
   * Sets up the sample branch, using the administrative interface, removes the
   * default PHP branch, adds our fake PHP branch, and updates everything.
   */
  public function setUp() {
    $this->baseSetUp();

    // Create a "file" branch with the sample code, from the admin interface.
    $this->branchInfo = $this->setUpBranchUi();

    // Remove the default PHP branch, which most tests do not need.
    $this->removePhpBranch();

    // Create a "php" branch with the sample PHP function list, from the admin
    // interface.
    $this->phpBranchInfo = $this->createPhpBranchUi();

    // Parse the code.
    $this->resetBranchesAndCache();
    $this->updateBranches();
    $this->processApiParseQueue();
  }

  /**
   * Sets up a project and branch using the user interface.
   *
   * @param string $prefix
   *   Directory prefix to prepend on the data directories.
   * @param bool $default
   *   TRUE to set this as the default branch; FALSE to not set it as default.
   * @param array $info
   *   Array of information to override the defaults (see function code to see
   *   what they are). Note that $prefix is applied after this information is
   *   read, and that only one directory and one excluded are supported in this
   *   function.
   *
   * @return array
   *   Array of information (defaults with overrides) used to create the
   *   branch and project.
   */
  protected function setUpBranchUi($prefix = '', $default = TRUE, array $info = array()) {
    // Set up defaults.
    $info += array(
      'project' => 'test',
      'project_title' => 'Project 6',
      'project_type' => 'module',
      'branch_name' => '6',
      'title' => 'Testing 6',
      'core_compatibility' => '7.x',
      'update_frequency' => 1,
      'directory' => drupal_get_path('module', 'api') . '/tests/sample',
      'excluded' => drupal_get_path('module', 'api') . '/tests/sample/to_exclude',
      'exclude_drupalism_regexp' => '',
      'regexps' => '',
    );
    $info['preferred'] = $default ? 1 : 0;

    // Create the project.
    $project_info = array(
      'project_name' => $info['project'],
      'project_type' => $info['project_type'],
      'project_title' => $info['project_title'],
    );
    $this->drupalPost('admin/config/development/api/projects/new',
      $project_info,
      'Save project'
    );

    // Create the branch.
    $branch_info = array(
      'project' => $info['project'],
      'branch_name' => $info['branch_name'],
      'title' => $info['title'],
      'preferred' => $info['preferred'],
      'core_compatibility' => $info['core_compatibility'],
      'update_frequency' => $info['update_frequency'],
      'data[directories]' => $prefix . $info['directory'],
      'data[exclude_files_regexp]' => $info['regexps'],
      'data[exclude_drupalism_regexp]' => $info['exclude_drupalism_regexp'],
    );
    if ($info['excluded'] != 'none') {
      $branch_info['data[excluded_directories]'] = $prefix . $info['excluded'];
    }
    $this->drupalPost('admin/config/development/api/branches/new',
      $branch_info,
      'Save branch'
    );

    if ($default) {
      // Make this the default project/core compat. This has to be done after
      // the setup for the branch, because the API module will override the
      // setting if the branch doesn't exist yet, in an attempt to make sure
      // a branch exists.
      $this->drupalPost('admin/config/development/api', array(
        'api_default_core_compatibility' => $info['core_compatibility'],
        'api_default_project' => $info['project'],
      ), 'Save configuration');

      $branches = api_get_branches(TRUE);
      foreach ($branches as $branch) {
        if ($branch->title == $info['title']) {
          $this->assertEqual(variable_get('api_default_branch', 99), $branch->branch_id, 'Variable for default branch is set correctly');
          break;
        }
      }
    }

    return $info;
  }

  /**
   * Sets up a PHP reference branch using the sample code, in the admin UI.
   *
   * @return array
   *   Information array used to create the branch.
   */
  protected function createPhpBranchUi() {
    $info = array(
      'title' => 'php2',
      'data[summary]' => url('<front>', array('absolute' => TRUE)) . '/' . drupal_get_path('module', 'api') . '/tests/php_sample/funcsummary.json',
      'data[path]' => 'http://example.com/function/!function',
      'update_frequency' => 1,
    );

    $this->drupalPost('admin/config/development/api/php_branches/new',
      $info,
      'Save branch'
    );

    return $info;
  }

  /**
   * Sets up an API reference branch using the sample code, in the admin UI.
   *
   * @param array $info
   *   Array of information to override the defaults (see function code to see
   *   what they are).
   *
   * @return array
   *   Information array used to create the branch.
   */
  protected function createApiBranchUi(array $info = array()) {
    $info += array(
      'title' => 'sample_api_branch',
      'data[url]' => url('<front>', array('absolute' => TRUE)) . '/' . drupal_get_path('module', 'api') . '/tests/php_sample/sample_drupal_listing.json',
      'data[search_url]' => url('<front>', array('absolute' => TRUE)) . '/api/test_api_project/test_api_branch/search/',
      'data[core_compatibility]' => '7.x',
      'data[project_type]' => 'core',
      'data[page_limit]' => 2000,
      'data[timeout]' => 30,
      'update_frequency' => 1,
    );

    $this->drupalPost('admin/config/development/api/php_branches/new_api',
      $info,
      'Save branch'
    );

    return $info;
  }

  /**
   * Asserts that a link exists, with the given URL.
   *
   * @param string $label
   *   Label of the link to find.
   * @param string $url
   *   URL to match.
   * @param string $message_link
   *   Message to use in link exist assertion.
   * @param string $message_url
   *   Message to use for URL matching assertion.
   */
  protected function assertLinkUrl($label, $url, $message_link, $message_url) {
    // Code follows DrupalWebTestCase::clickLink() and assertLink().
    $links = $this->xpath('//a[text()="' . $label . '"]');
    $this->assert(isset($links[0]), $message_link);
    if (isset($links[0])) {
      $url_target = $this->getAbsoluteUrl($links[0]['href']);
      $this->assertEqual($url_target, $url, $message_url);
    }
  }

  /**
   * Asserts that a link exists, with substring matching on the URL.
   *
   * @param string $label
   *   Label of the link to find.
   * @param string $url
   *   URL to match. The test passes if $url is a substring of the link's URL.
   * @param string $message_link
   *   Message to use in link exist assertion.
   * @param string $message_url
   *   Message to use for URL matching assertion.
   * @param int $index
   *   (optional) Index of the link on the page, like assertLink(). Default: 0.
   */
  protected function assertLinkUrlSubstring($label, $url, $message_link, $message_url, $index = 0) {
    // Code follows DrupalWebTestCase::clickLink() and assertLink().
    $links = $this->xpath('//a[text()="' . $label . '"]');
    $this->assert(isset($links[$index]), $message_link);
    if (isset($links[$index])) {
      $url_target = $this->getAbsoluteUrl($links[$index]['href']);
      $this->assertTrue(strpos($url_target, $url) !== FALSE, $message_url);
    }
  }

  /**
   * Asserts that the current page's title contains a string.
   *
   * @param string $string
   *   String to match in the title.
   * @param string $message
   *   Message to print.
   */
  protected function assertTitleContains($string, $message) {
    $title = current($this->xpath('//title'));
    $this->assertTrue(strpos($title, $string) !== FALSE, $message);
  }

  /**
   * Asserts that the current page's URL contains a string.
   *
   * @param string $string
   *   String to match in the URL.
   * @param string $message
   *   Message to print.
   */
  protected function assertUrlContains($string, $message) {
    $this->assertTrue(strpos($this->url, $string) !== FALSE, $message);
  }

  /**
   * Asserts that the count of links with the given label is correct.
   *
   * @param string $label
   *   Label to search for.
   * @param int $count
   *   Count to assert.
   * @param string $message
   *   Message to display.
   */
  protected function assertLinkCount($label, $count, $message) {
    $links = $this->xpath('//a[normalize-space(text())=:label]', array(':label' => $label));
    $this->assertEqual(count($links), $count, $message);
  }

}
