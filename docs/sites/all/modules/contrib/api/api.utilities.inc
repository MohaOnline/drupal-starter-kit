<?php

/**
 * @file
 * Utility functions and constants used by the API module in various places.
 */

/**
 * Regular expression for starting inline \@tags.
 */
define('API_RE_TAG_START', '(?<!\\\)@');

/**
 * Regular expression for matching file names with one or more extensions.
 */
define('API_RE_FILENAME', '([a-zA-Z0-9_-]+(?:\.[a-zA-Z0-9_-]+)+)');

/**
 * Regular expression for matching PHP functions and methods in text.
 *
 * These are patterns like ClassName::methodName(), or just function_name().
 * Possibly with namespaces. Doesn't include the ().
 */
define('API_RE_FUNCTION_IN_TEXT', '\\\\*[a-zA-Z_\x7f-\xff][\\\\a-zA-Z0-9_\x7f-\xff:]*');

/**
 * Regular expression for matching characters interior to function names.
 */
define('API_RE_FUNCTION_CHARACTERS', '[a-zA-Z0-9_\x7f-\xff]+');

/**
 * Regular expression for word boundary start for API linking.
 */
define('API_RE_WORD_BOUNDARY_START', '(?<=^|[\s\(@\>|])');

/**
 * Regular expression for word boundary end for API linking.
 */
define('API_RE_WORD_BOUNDARY_END', '(?=$|[\s.,:;?!)\<\[\|])');

/**
 * Attempts to locate a core branch corresponding to the given branch.
 *
 * @param object $branch
 *   Branch object to find a core branch for.
 *
 * @return object|false
 *   Branch object for the core branch, if there is one. If not, FALSE.
 */
function api_find_core_branch($branch) {
  if (!$branch) {
    return FALSE;
  }

  $query = db_select('api_branch', 'b');
  $query->innerJoin('api_project', 'p', 'b.project = p.project_name');
  $query->condition('p.project_type', 'core');
  $query->condition('b.core_compatibility', $branch->core_compatibility);
  $query->fields('b');
  $query->fields('p', array('project_title', 'project_type'));
  $query->orderBy('b.weight', 'ASC');
  $result = $query->execute();
  // There shouldn't be more than one result, so assume that's the case and
  // return the first result found.
  foreach ($result as $branch) {
    api_unpack_branch_data($branch);
    return $branch;
  }

  return FALSE;
}

/**
 * Displays the output of a view, assumed to be a block display.
 *
 * @param string $view_name
 *   Machine name of the view.
 * @param string $display_name
 *   Machine name of the display within the view.
 * @param array $args
 *   (optional) Array of arguments for the view display.
 *
 * @return string
 *   String containing the view output. If the view was empty, an empty string.
 */
function api_display_view($view_name, $display_name, array $args = array()) {
  $output = '';

  // Load the view.
  $view = views_get_view($view_name);

  if ($view) {
    // Override the URL so that exposed filters will work on blocks even without
    // AJAX.
    $view->override_url = current_path();
    $output = $view->preview($display_name, $args);
  }

  // Check for no content.
  if (!strlen(trim(strip_tags($output)))) {
    $output = '';
  }

  return $output;
}

/**
 * Figures out the full class name of a class, with namespaces.
 *
 * @param string $name
 *   The name as it appears, which could include a namespace, and might or
 *   might not start with a backslash if it does.
 * @param string $namespace
 *   The namespace for the file the name appears in.
 * @param array $use_aliases
 *   Associative array of alias name to full name for use statements in the
 *   file the name appears in.
 * @param int $class_did
 *   If provided, figure out what class is meant for keywords like $this,
 *   self, parent, and static, within the context of the given class. If not
 *   provided, skip this step and return $name if one of these is provided.
 *
 * @return string
 *   Fully-qualified name of the class, starting with a backslash.
 */
function api_full_classname($name, $namespace = '', array $use_aliases = array(), $class_did = 0) {

  // Break off the class name from the rest.
  $classname = $name;
  $suffix = '';

  $pos = strpos($name, '::');
  if ($pos === FALSE) {
    $pos = strpos($name, '->');
  }
  if ($pos !== FALSE) {
    $classname = substr($name, 0, $pos);
    $suffix = substr($name, $pos);
  }

  // If a class DID was provided, try to look up the class for keywords. If
  // successful, use it, but if not, or there is no class DID, just return the
  // passed-in name.
  if (in_array($classname, array('self', 'static', 'parent', 'this'))) {
    if ($class_did) {
      $class = api_object_load($class_did);
      if ($class) {
        if (in_array($classname, array('self', 'static', 'this'))) {
          $classname = $class->namespaced_name;
        }
        else {
          $tmpname = db_select('api_reference_storage', 'ars')
            ->condition('ars.from_did', $class_did)
            ->condition('ars.object_type', 'class')
            ->fields('ars', array('object_name'))
            ->execute()
            ->fetchField();
          if ($tmpname) {
            $classname = $tmpname;
          }
          else {
            return $name;
          }
        }
      }
      else {
        return $name;
      }
    }
    else {
      return $name;
    }
  }

  // See if there is an alias for this class name, or if we should prepend
  // the file's namespace.
  if (isset($use_aliases[$classname])) {
    // This class was aliased.
    $classname = $use_aliases[$classname];
  }
  elseif (strpos($classname, '\\') === FALSE) {
    // There was no alias, and the classname did not have a namespace in it.
    $classname = $namespace . '\\' . $classname;
  }

  // Make sure to start with a backslash, no matter what.
  if (strpos($classname, '\\') !== 0) {
    $classname = '\\' . $classname;
  }

  return $classname . $suffix;
}

/**
 * Adds the configured line break character to text.
 *
 * @param string $text
 *   Text to add line breaks to, presumably a class member name or something
 *   similar that is too long.
 *
 * @return string
 *   Text with line break characters added.
 */
function api_add_breaks($text) {
  $where = array_filter(explode(' ', variable_get('api_breaks_where', '/ :: \\')));
  if (!count($where)) {
    return $text;
  }

  $tag = variable_get('api_breaks_tag', '<wbr />');
  if (!strlen($tag)) {
    return $text;
  }

  $replace = array();
  foreach ($where as $string) {
    $replace[] = $string . $tag;
  }

  $text = str_replace($where, $replace, $text);
  return $text;
}

/**
 * Decodes HTML entities.
 *
 * @param string $text
 *   Text to decode.
 *
 * @return string
 *   Text with all HTML entities decoded.
 */
function api_entity_decode($text) {
  $text = html_entity_decode($text);
  // html_entity_decode does not decode numeric entities, and there are
  // many cases of &#39; (quote) in here.
  $text = str_replace('&#039;', "'", $text);
  $text = str_replace('&#39;', "'", $text);
  return $text;
}

/**
 * Trims whitespace, and removes trailing directory separators and blank lines.
 *
 * @param string $list
 *   String containing a newline-separated list of directories.
 *
 * @return string
 *   String with cleaned-up directories, separated by newlines.
 */
function api_clean_directory_list($list) {
  $array = explode("\n", $list);
  foreach ($array as $key => $directory) {
    $array[$key] = rtrim(trim($directory), DIRECTORY_SEPARATOR);
    if (empty($array[$key])) {
      unset($array[$key]);
    }
  }
  return implode("\n", $array);
}

/**
 * Creates a new documentation object node (for comments).
 *
 * @return int
 *   Node ID of the created node.
 */
function api_new_documentation_node() {
  $node = new stdClass();
  $node->type = 'api';
  $node->uid = 0;
  if (module_exists('comment')) {
    $node->comment = variable_get('comment_api', COMMENT_NODE_OPEN);
  }
  node_save($node);

  return $node->nid;
}

/**
 * Adds defaults for TEXT fields to a database record.
 *
 * These cannot come from the schema, because TEXT fields have no defaults.
 *
 * @param array $record
 *   Record about to be written to the database with drupal_write_record(),
 *   passed by reference and modified in place.
 * @param string $table
 *   Table $record is going into.
 */
function api_add_text_defaults(array &$record, $table) {
  switch ($table) {
    case 'api_documentation':
      $record += array(
        'summary' => '',
        'documentation' => '',
        'code' => '',
        'see' => '',
        'deprecated' => '',
        'var' => '',
        'throws' => '',
        'namespace' => '',
        'namespaced_name' => '',
        'modifiers' => '',
      );
      break;

    case 'api_function':
      $record += array(
        'parameters' => '',
        'return_value' => '',
      );
      break;
  }
}
