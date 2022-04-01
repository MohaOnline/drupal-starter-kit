<?php

/**
 * @file
 * The PHP documentation parser that generates content for api.module.
 *
 * Functions whose names start with _ in this file are only intended to be
 * called by other functions in this file.
 */

use PhpParser\Comment\Doc as CommentDoc;
use PhpParser\Node\Stmt\Function_ as NodeFunction;
use PhpParser\Node\Stmt\Class_ as NodeClass;
use PhpParser\Node\Stmt\ClassLike as NodeClassLike;
use PhpParser\ParserFactory;
use PhpParser\Error as ParserError;
use PhpParser\PrettyPrinter\Standard as StandardPrettyPrinter;
use Symfony\Component\Yaml\Parser as YamlParser;
use Symfony\Component\Yaml\Dumper as YamlDumper;

module_load_include('inc', 'api', 'api.utilities');

/**
 * Reads in a file and calls a callback function to parse and save it.
 *
 * @param string $callback
 *   Name of the function to call to parse and save the file contents, such as
 *   api_parse_php_file(), api_parse_text_file(), or api_parse_html_file()
 *   (pass the function name as a string).
 * @param string $file_path
 *   Full path to the file to read in.
 * @param object $branch
 *   Object representing the branch to assign the file contents to. It is
 *   assumed to be a fresh load of this branch from the branch database table,
 *   and a valid branch.
 * @param string $file_name
 *   File name to store in the database for this file.
 */
function api_parse_file($callback, $file_path, $branch, $file_name) {
  _api_branch_touched($branch->branch_id);

  // See if this is a Drupal file or a Drupal-excluded file.
  $drupal_regexps = (isset($branch->exclude_drupalism_regexp) ? explode("\n", $branch->exclude_drupalism_regexp) : array());
  $drupal_regexps = array_filter(array_map('trim', $drupal_regexps));
  $is_drupal = TRUE;
  foreach ($drupal_regexps as $regexp) {
    if (preg_match($regexp, $file_path)) {
      $is_drupal = FALSE;
      break;
    }
  }

  $basename = drupal_basename($file_name);
  $docblock = array(
    'object_name' => $file_name,
    'branch' => $branch,
    'object_type' => 'file',
    'file_name' => $file_name,
    'title' => $basename,
    'basename' => $basename,
    'documentation' => '',
    'references' => array(),
    'modified' => filemtime($file_path),
    'queued' => 0,
    'source' => str_replace(array("\r\n", "\r"), array("\n", "\n"), file_get_contents($file_path)),
    'content' => '',
    'class' => '',
    'namespaced_name' => '',
    'modifiers' => '',
    'is_drupal' => $is_drupal,
    'code' => '',
  );

  $callback($docblock);
}

/**
 * Saves contents of a Twig file as API documentation.
 *
 * Callback for api_parse_file().
 *
 * @param array $docblock
 *   Array from api_parse_file() containing the file contents and information
 *   about the file, branch, etc.
 */
function api_parse_twig_file(array $docblock) {
  // Use the text file function, but escape HTML characters.
  api_parse_text_file($docblock, TRUE);
}

/**
 * Saves contents of a file as a single piece of text documentation.
 *
 * Callback for api_parse_file().
 *
 * @param array $docblock
 *   Array from api_parse_file() containing the file contents and information
 *   about the file, branch, etc.
 * @param bool $escape_html
 *   If TRUE, escpae HTML characters in the source code listing (for Twig
 *   files).
 */
function api_parse_text_file(array $docblock, $escape_html = FALSE) {
  // See if the file contains an @file block, and use that for the
  // documentation if so; otherwise, just use the file as a whole. This is
  // probably only present for Twig files.
  $matches = array();
  if (preg_match('|/\*\*[\s\*]+@file.+\*/|Us', $docblock['source'], $matches)) {
    $docblock['content'] = _api_clean_comment($matches[0]);
  }
  else {
    $tmp = array();
    $docblock['documentation'] = _api_format_documentation($docblock['source'], FALSE, $tmp);
  }

  // Escape HTML and number the lines.
  $output = $docblock['source'];
  if ($escape_html) {
    $output = htmlspecialchars($docblock['source']);
  }
  $output = _api_number_lines($output);
  $output = '<pre class="php"><code>' . $output . '</code></pre>';
  $docblock['code'] = $output;

  _api_save_docblocks(array($docblock));
}

/**
 * Saves contents of a file as a single piece of HTML documentation.
 *
 * Escapes any HTML special characters in the text, so that it can be
 * displayed to show the HTML tags.
 *
 * Callback for api_parse_file().
 *
 * @param array $docblock
 *   Array from api_parse_file() containing the file contents and information
 *   about the file, branch, etc.
 */
function api_parse_html_file(array $docblock) {
  $docblock['code'] = '<pre>' . htmlspecialchars($docblock['source']) . '</pre>';
  $title_match = array();
  if (preg_match('!<title>([^<]+)</title>!is', $docblock['source'], $title_match)) {
    $docblock['title'] = trim($title_match[1]);
    $docblock['summary'] = $docblock['title'];
  }
  $documentation_match = array();
  if (preg_match('!<body>(.*?</h1>)?(.*)</body>!is', $docblock['source'], $documentation_match)) {
    $docblock['documentation'] = $documentation_match[2];
  }

  _api_save_docblocks(array($docblock));
}

/**
 * Parses a PHP file and saves the file and its contents as documentation.
 *
 * PHP functions, classes, global variables, constants, etc. in the file
 * are saved as documentation, if they have docblocks.
 *
 * Callback for api_parse_file().
 *
 * @param array $docblock
 *   Array from api_parse_file() containing the file contents and information
 *   about the file, branch, etc.
 */
function api_parse_php_file(array $docblock) {
  if (!api_libraries_loaded()) {
    return;
  }

  $error_logged = FALSE;
  $parser = (new ParserFactory())->create(ParserFactory::PREFER_PHP7);
  try {
    $statements = $parser->parse($docblock['source']);
  }
  catch (ParserError $e) {
    $error_logged = TRUE;
    watchdog('api', 'File @name could not be parsed in %project %branch. Message: %msg',
      array(
        '@name' => $docblock['file_name'],
        '%project' => $docblock['branch']->project,
        '%branch' => $docblock['branch']->branch_name,
        '%msg' => $e->getMessage(),
      ), WATCHDOG_ERROR);
  }

  if ($statements && is_array($statements)) {
    // Find all the references in the whole file. We will omit the ones that
    // belong to particular functions etc. in the file.
    $references = _api_find_php_references($statements, $docblock['is_drupal'], $docblock['file_name'], $docblock['branch']);

    // Make the first doc block be for the file as a whole.
    $docblock['code'] = _api_format_statements($statements, $docblock['is_drupal'], TRUE);
    $docblocks = array($docblock);

    // Set default documentation block array for items other than the file.
    $default_block = array(
      'branch' => $docblock['branch'],
      'file_name' => $docblock['file_name'],
      'class' => '',
      'object_type' => '',
      'documentation' => '',
      'references' => array(),
      'see' => '',
      'deprecated' => '',
      'start_line' => 0,
      'namespaced_name' => '',
      'modifiers' => '',
      'is_drupal' => $docblock['is_drupal'],
      'code' => '',
    );

    $found_references = _api_build_php_docblocks($statements, $default_block, $docblocks);

    // For the file, save only the references not found in parsing the
    // statements within the file. This will only save references to function
    // calls in the global area of the file, excluding those found in
    // functions declared in the file. This doesn't apply to namespace and use
    // references though -- these are found only in _api_build_php_docblocks().
    foreach ($found_references as $type => $list) {
      // For namespaces and use aliases, merge these into the main references.
      if ($type == 'namespace' || $type == 'use_alias') {
        $refs = array($type => $list);
        $references = _api_merge_references($references, $refs, $docblock['file_name'], $docblock['branch']);
      }
      else {
        // For other references, remove them.
        foreach ($list as $name => $info) {
          unset($references[$type][$name]);
        }
      }
    }
    $docblocks[0]['references'] = $references;
  }
  else {
    // We at least want to save the empty docblock so the file record is
    // updated.
    $docblocks = array($docblock);
    if (!$error_logged) {
      watchdog('api',
        'File @name had no statements in %project %branch.',
        array(
          '@name' => $docblock['file_name'],
          '%project' => $docblock['branch']->project,
          '%branch' => $docblock['branch']->branch_name,
        ), WATCHDOG_WARNING);
    }
  }

  _api_save_docblocks($docblocks);

  // Free up memory.
  $references = NULL;
  $found_references = NULL;
  $docblock = NULL;
  $default_block = NULL;
  $docblocks = NULL;
  $statements = NULL;
}

/**
 * Saves contents of a YAML file.
 *
 * Callback for api_parse_file().
 *
 * @param array $docblock
 *   Array from api_parse_file() containing the file contents and information
 *   about the file, branch, etc.
 */
function api_parse_yaml_file(array $docblock) {
  // Just use the file name as the documentation, since the file contents
  // are not good documentation.
  $bare_docblock = $docblock;
  $docblock['documentation'] = $docblock['file_name'];

  // Parse the YAML in the file.
  $parser = new YamlParser();
  try {
    $parsed = $parser->parse($docblock['source']);
  }
  catch (Exception $e) {
    $parsed = array();
    watchdog('api',
      'YAML parsing failed for %filename in %project %branch with message %msg',
      array(
        '%filename' => $docblock['file_name'],
        '%msg' => $e->getMessage(),
        '%project' => $docblock['branch']->project,
        '%branch' => $docblock['branch']->branch_name,
      ), WATCHDOG_ERROR);
  }

  // Find potential references, which are array values that look like they
  // could be callback function names, in addition to some array keys. The
  // level of keys we want depends on (a) if it's a Drupal file, and (b) the
  // file extension.
  $key_level = 0;
  $is_services = FALSE;
  if ($docblock['is_drupal']) {
    $basename = $docblock['basename'];
    $matches = array();
    if (preg_match('|\.([^.]+)\.yml$|', $basename, $matches)) {
      if ($matches[1] == 'services') {
        // We want to keep references to 2nd-level keys in services.yml files.
        $key_level = 2;
        $is_services = TRUE;
      }
      elseif ($matches[1] == 'routing' || $matches[1] == 'local_tasks' || $matches[1] == 'contextual_links') {
        // We want to keep references to 1st-level keys in routing.yml and
        // related files.
        $key_level = 1;
      }
    }
  }

  $references = _api_find_yaml_references($parsed, $key_level, $docblock['file_name'], $docblock['branch']);
  // For services files, we do not want the YML strings references, because
  // really only the services are relevant.
  if ($is_services) {
    unset($references['yaml string']);
  }
  $docblock['references'] = $references;

  // Format the code, number the lines and put into a code block.
  // Escape HTML tags and entities.
  $code = $docblock['source'];
  $code = htmlspecialchars($code, ENT_NOQUOTES, 'UTF-8');
  $code = _api_format_yaml_code($code, $references);
  $code = _api_number_lines($code);
  $code = '<pre class="php"><code>' . $code . '</code></pre>';
  $docblock['code'] = $code;
  $full_references = $references;

  $docblocks = array($docblock);

  // For services files, make docblocks for each service.
  if ($is_services && isset($parsed['services'])) {
    $dumper = new YamlDumper();
    $dumper->setIndentation(2);

    foreach ($parsed['services'] as $name => $info) {
      try {
        $code = $dumper->dump($info, 2);
      }
      catch (Exception $e) {
        // We should be able to dump, but just in case, fall back to printing,
        // which is better than nothing.
        $code = print_r($info, TRUE);
      }
      $code = htmlspecialchars($code, ENT_NOQUOTES, 'UTF-8');
      $code = _api_format_yaml_code($code, $full_references);
      $code = _api_number_lines($code);
      $code = '<pre class="php"><code>' . $code . '</code></pre>';

      $references = array('service_tag' => array());
      $class = '';
      if (isset($info['class']) && $info['class']) {
        $class = $info['class'];
        // Make sure the class name starts with a backslash.
        $ref = $class;
        $pos = strpos($ref, '\\');
        if ($pos !== 0) {
          $ref = '\\' . $ref;
        }

        $references['service_class'] = array($ref);
      }
      $alias = (isset($info['alias']) && $info['alias']) ? $info['alias'] : '';
      $abstract = (isset($info['abstract']) && $info['abstract']);

      if (isset($info['tags'])) {
        foreach ($info['tags'] as $tag) {
          $tag_name = $tag['name'];
          if ($tag_name) {
            $references['service_tag'][$tag_name] = $tag_name;
          }
        }
      }

      $service = array(
        'object_name' => $name,
        'title' => $name,
        'object_type' => 'service',
        'code' => $code,
        'source' => '',
        'documentation' => $class ? $class : ($alias ? t('Alias of %alias', array('%alias' => $alias)) : ($abstract ? t('Abstract') : '')),
        'references' => $references,
      ) + $bare_docblock;

      $docblocks[] = $service;
    }
  }

  _api_save_docblocks($docblocks);
}

/**
 * Sort callback for usort in api_parse_yaml_file().
 *
 * Sorts with longest first.
 */
function _api_length_compare($a, $b) {
  $lena = strlen($a);
  $lenb = strlen($b);
  if ($lena < $lenb) {
    return 1;
  }
  return ($lena > $lenb) ? -1 : 0;
}

/**
 * Recursively finds potential references in a parsed YAML array.
 *
 * @param object $yaml
 *   Parsed YAML object.
 * @param int $key_refs_level
 *   Store references to the keys on this level, where 1 is the current level.
 * @param string $filename
 *   File name for watchdog messages.
 * @param object $branch
 *   Branch for watchdog references.
 *
 * @return array
 *   Array of references suitable for use in $docblock['references'].
 */
function _api_find_yaml_references($yaml, $key_refs_level, $filename, $branch) {
  if (empty($yaml)) {
    return array('potential callback' => array());
  }

  $yaml = (array) $yaml;
  $references = array('potential callback' => array(), 'yaml string' => array());
  foreach ($yaml as $key => $value) {
    if ($key_refs_level == 1 && is_string($key)) {
      $key = trim($key);
      if ($key) {
        $references['yaml string'][$key] = $key;
      }
    }
    if (is_string($value)) {
      $matches = array();
      if (preg_match("|^['\"]*(" . API_RE_FUNCTION_IN_TEXT . ")['\"]*$|", $value, $matches)) {
        // Special case the commonly-found TRUE and FALSE.
        $val = trim($matches[1]);
        if ($val && $val != 'TRUE' && $val != 'FALSE' && !is_numeric($val)) {
          $references['potential callback'][$val] = $val;
        }
      }
    }
    elseif (is_array($value) || is_object($value)) {
      $references = _api_merge_references($references, _api_find_yaml_references($value, $key_refs_level - 1, $filename, $branch), $filename, $branch);
    }
  }

  return $references;
}

/**
 * Formats YAML code.
 *
 * @param string $code
 *   The code to format.
 * @param array $references
 *   Array of found references, used to put spans around strings that can
 *   turn into link.
 * @param string $wrappers
 *   (optional) Characters that can wrap strings, formatted so it can go into
 *   a [] character class in a regular expression.
 * @param string $span_class
 *   (optional) Class to put on spans in the text.
 *
 * @return string
 *   Formatted code.
 */
function _api_format_yaml_code($code, array $references, $wrappers = '\'"\s', $span_class = 'yaml-reference') {
  // Wrap each found callback reference string in a span. We have to use
  // preg_replace() to do this, because we only want to match whole strings,
  // enclosed in single quotes, double quotes, or whitespace. And we want to
  // do the whole thing in one replace, so it matches the longest possible
  // string in each case. So we also need to sort the strings by length,
  // longest first, because PHP regular expressions with alternatives take the
  // first matching one.
  $callbacks = $references['potential callback'];
  if (count($callbacks)) {
    $newstrings = array();
    usort($callbacks, '_api_length_compare');
    foreach ($callbacks as $string) {
      $newstrings[] = preg_quote($string, '/');
    }

    $regexp = '/(?<=[' . $wrappers . '])(' . implode('|', $newstrings) . ')(?=[' . $wrappers . '])/';
    $code = preg_replace($regexp, '<span class="' . $span_class . '">$1</span>', $code);
  }

  return $code;
}

/**
 * Builds documentation blocks and finds references for parsed PHP code.
 *
 * @param array $statements
 *   An array of PHP parser output statements to look through.
 * @param array $default_block
 *   The default documentation block to use.
 * @param array $docblocks
 *   The array of documentation blocks, passed by reference. Documentation and
 *   code items found in the PHP statements are added to the end of this array.
 *
 * @return array
 *   An array of all the references found while parsing the statements.
 */
function _api_build_php_docblocks(array $statements, array $default_block, array &$docblocks) {
  if (!api_libraries_loaded()) {
    return array();
  }

  // Keep track of all references found.
  $all_references = array();

  // Traverse top-level statement list to gather documentation items.
  $in_class = !empty($default_block['class']);
  $class_prefix = $in_class ? $default_block['class'] . '::' : '';

  foreach ($statements as $statement) {
    $docblock = $default_block;
    $docblock['start_line'] = $statement->getLine();
    $docblock['content'] = '';
    $type = $statement->getType();

    // Process the comments for this statement. The parser makes an array of
    // all the comments that precede the statement; the last one is the doc
    // block (for statements that support doc blocks). Other than the official
    // doc bloc, other ones can be saved as global doc blocks, such as @file
    // and @defgroup doc blocks, but only if we are outside of classes.
    $comments = $statement->getAttribute('comments');
    $types_without_comments = array(
      'Stmt_Nop',
      'Stmt_Namespace',
      'Stmt_Use',
    );

    if ($comments) {
      $count = count($comments);
      foreach ($comments as $index => $comment) {
        if (!$in_class &&
          ($index < $count - 1 || in_array($type, $types_without_comments)) &&
          $comment instanceof CommentDoc) {

          // This is a global comment. Add it to the list of doc blocks.
          $comment_docblock = $default_block;
          $comment_docblock['content'] = _api_clean_comment($comment->getText());
          $comment_docblock['start_line'] = $comment->getLine();
          $docblocks[] = $comment_docblock;
        }
        elseif ($index == $count - 1 && $comment instanceof CommentDoc) {
          // This is the doc comment for this statement.
          $docblock['content'] = _api_clean_comment($comment->getText());
        }
      }
    }

    // Clear out the comments, so that we don't encounter them later.
    $statement->setAttribute('comments', array());

    // Process the actual statement.
    switch ($type) {
      case 'Expr_FuncCall':
        // Process this only if it is a call to define(CONST_NAME, value);.
        $name = $statement->name->toString();
        if ($name == 'define') {
          $args = $statement->args;
          if (count($args) > 0) {
            $docblock['object_type'] = 'constant';
            $docblock['member_name'] = $args[0]->value->value;
            $docblock['object_name'] = $class_prefix . $docblock['member_name'];
            $docblock['title'] = $docblock['object_name'];
            $docblock['code'] = _api_format_statements(array($statement), $docblock['is_drupal']);
            $docblocks[] = $docblock;
          }
        }
        break;

      case 'Stmt_ClassConst':
      case 'Stmt_Const':
      case 'Stmt_Property':
      case 'Stmt_Global':
        $sub_objects = array();
        $title_prefix = '';
        if ($type == 'Stmt_ClassConst' || $type == 'Stmt_Const') {
          $sub_objects = $statement->consts;
          $docblock['object_type'] = 'constant';
        }
        elseif ($type == 'Stmt_Property') {
          $sub_objects = $statement->props;
          $docblock['object_type'] = 'property';
          $title_prefix = '$';
        }
        elseif ($type == 'Stmt_Global') {
          $sub_objects = $statement->vars;
          $docblock['object_type'] = 'global';
          $title_prefix = '$';
        }

        if (!empty($sub_objects)) {
          $sub_object = $sub_objects[0];
          $docblock['member_name'] = $sub_object->name;
          $docblock['object_name'] = $class_prefix . $docblock['member_name'];
          $docblock['title'] = $class_prefix . $title_prefix . $docblock['member_name'];
          $docblock['code'] = _api_format_statements(array($statement), $docblock['is_drupal']);
          if ($type != 'Stmt_Global' && $type != 'Stmt_Const') {
            $docblock['modifiers'] = _api_get_statement_modifiers($statement);
          }

          $docblocks[] = $docblock;
        }
        break;

      case 'Stmt_Function':
      case 'Stmt_ClassMethod':
        $docblock['object_type'] = 'function';
        $docblock['member_name'] = $statement->name;
        $docblock['object_name'] = $class_prefix . $docblock['member_name'];
        $docblock['title'] = $docblock['object_name'];
        $docblock['code'] = _api_format_statements(array($statement), $docblock['is_drupal']);
        $docblock['references'] = _api_find_php_references(array($statement), $docblock['is_drupal'], $docblock['file_name'], $docblock['branch']);
        $all_references = _api_merge_references($all_references, $docblock['references'], $docblock['file_name'], $docblock['branch']);
        $docblock['signature'] = _api_get_function_signature($statement);
        if ($type == 'Stmt_ClassMethod') {
          $docblock['modifiers'] = _api_get_statement_modifiers($statement);
        }
        $docblocks[] = $docblock;
        break;

      case 'Stmt_Class':
      case 'Stmt_Interface':
      case 'Stmt_Trait':
        $docblock['object_name'] = $statement->name;
        $docblock['title'] = $docblock['object_name'];
        $docblock['member_name'] = $docblock['object_name'];
        // Note that we are not finding references here. We use the ones
        // from the child statements instead.
        $docblock['code'] = _api_format_statements(array($statement), $docblock['is_drupal']);

        $docblock['extends'] = array();
        $docblock['implements'] = array();

        if ($type == 'Stmt_Class') {
          $docblock['object_type'] = 'class';
          $docblock['modifiers'] = _api_get_statement_modifiers($statement);

          if ($statement->extends) {
            $docblock['extends'] = array($statement->extends->toString());
          }
          if (!empty($statement->implements) && count($statement->implements)) {
            foreach ($statement->implements as $item) {
              $docblock['implements'][] = $item->toString();
            }
          }
        }
        elseif ($type == 'Stmt_Interface') {
          $docblock['object_type'] = 'interface';
          if ($statement->extends) {
            foreach ($statement->extends as $extend) {
              $docblock['extends'][] = $extend->toString();
            }
          }
        }
        else {
          $docblock['object_type'] = 'trait';
        }

        $docblocks[] = $docblock;

        // Process the class's internal/body statements.
        if (!empty($statement->stmts)) {
          $last_index = count($docblocks) - 1;
          $references = _api_build_php_docblocks($statement->stmts, array_merge($default_block, array('class' => $docblock['object_name'])), $docblocks);
          $all_references = _api_merge_references($all_references, $references, $docblock['file_name'], $docblock['branch']);
          $docblocks[$last_index]['references'] = $references;
        }

        break;

      case 'Stmt_Namespace':
        if ($statement->name) {
          $namespace = $statement->name->toString();
          $references = array('namespace' => $namespace);
          $all_references = _api_merge_references($all_references, $references, $docblock['file_name'], $docblock['branch']);
        }
        else {
          watchdog('api',
            'Empty namespace declaration in file @name in %project %branch',
            array(
              '@name' => $docblock['file_name'],
              '%project' => $docblock['branch']->project,
              '%branch' => $docblock['branch']->branch_name,
            ), WATCHDOG_WARNING);
        }

        // The rest of the statements in this file are inside this namespace.
        if (!empty($statement->stmts)) {
          $references = _api_build_php_docblocks($statement->stmts, $default_block, $docblocks);
          $all_references = _api_merge_references($all_references, $references, $docblock['file_name'], $docblock['branch']);
        }
        break;

      case 'Stmt_Use':
        $references = array('use_alias' => array());
        foreach ($statement->uses as $use) {
          $alias = $use->alias;
          $class = $use->name->toString();
          if (!$alias) {
            $alias = $class;
          }
          $references['use_alias'][$alias] = $class;
        }
        $all_references = _api_merge_references($all_references, $references, $docblock['file_name'], $docblock['branch']);
        break;

      case 'Stmt_TraitUse':
        $trait = $statement->traits[0]->toString();
        $references = array(
          'use_trait' => array(
            $trait => array(
              'class' => $trait,
              'details' => array(),
            ),
          ),
        );
        foreach ($statement->adaptations as $adaptation) {
          $ad_type_parts = explode('_', $adaptation->getType());
          $ad_type = strtolower(array_pop($ad_type_parts));
          if ($ad_type == 'precedence') {
            foreach ($adaptation->insteadof as $node) {
              $references['use_trait'][$trait]['details'][$ad_type][$node->toString()] = $adaptation->method;
            }
          }
          else {
            $references['use_trait'][$trait]['details'][$ad_type][$adaptation->newName] = $adaptation->method;
          }
        }
        $all_references = _api_merge_references($all_references, $references, $docblock['file_name'], $docblock['branch']);
        break;
    }
  }

  return $all_references;
}

/**
 * Returns the function signature from a PhpParser function node object.
 *
 * @param object $statement
 *   A function statement to get the signature of.
 *
 * @return string
 *   The function signature.
 */
function _api_get_function_signature($statement) {
  // Make a function with empty body, pretty-print it, and remove the {}.
  $empty_function = new NodeFunction(
    $statement->name,
    array(
      'byRef' => $statement->byRef,
      'params' => $statement->params,
      'returnType' => $statement->returnType,
    ));

  // Note: Use the Standard pretty-printer here, not our class that does
  // HTML formatting.
  $printer = new StandardPrettyPrinter();
  $output = $printer->prettyPrint(array($empty_function));
  $output = preg_replace('|\{.*\}|s', '', $output);
  $output = str_replace('function ', '', $output);
  return trim($output);
}

/**
 * Returns the modifiers from a PhpParser statement.
 *
 * @param object $statement
 *   A class, method, property, etc. statement to get the modifiers of. Must
 *   have a flags property.
 *
 * @return string
 *   The modifiers.
 */
function _api_get_statement_modifiers($statement) {
  $flags = $statement->flags;
  $modifiers = '';
  $modifier_list = array(
    // Note: Keep this list in the order that the modifiers should appear.
    NodeClass::MODIFIER_ABSTRACT => 'abstract',
    NodeClass::MODIFIER_FINAL => 'final',
    NodeClass::MODIFIER_PUBLIC => 'public',
    NodeClass::MODIFIER_PROTECTED => 'protected',
    NodeClass::MODIFIER_PRIVATE => 'private',
    NodeClass::MODIFIER_STATIC => 'static',
  );
  foreach ($modifier_list as $flag => $name) {
    if ($flags & $flag) {
      $modifiers .= $name . ' ';
    }
  }
  return trim($modifiers);
}

/**
 * Cleans the comment characters out of a doc comment.
 *
 * @param string $text
 *   Comment text to clean.
 *
 * @return string
 *   Cleaned text.
 */
function _api_clean_comment($text) {
  // @todo This is used to replicate what Grammar Parser did with comments,
  // which may not be desirable if we switch to a different comment parser.
  $text = str_replace(array('/**', '*/'), '', $text);
  $text = preg_replace('|^ *\* ?|m', '', $text);
  return $text;
}

/**
 * Merges references, with warnings for duplicate namespaces.
 *
 * @param array $master
 *   Master list of references.
 * @param array $new
 *   New references to merge in.
 * @param string $filename
 *   File name for watchdog messages.
 * @param object $branch
 *   Branch object for watchdog messages.
 *
 * @return array
 *   Merged references. References in $new are appended to references in
 *   $master, and if there are duplicate namespace or use references, the
 *   $master list is used and a warning is generated.
 */
function _api_merge_references(array $master, array $new, $filename, $branch) {

  // We're supporting only one namespace declaration per file.
  if (isset($master['namespace']) && isset($new['namespace'])) {
    if ($master['namespace'] != $new['namespace']) {
      watchdog('api',
        'Multiple namespace declarations found in file @file in %project %branch. Only first is used.',
        array(
          '@file' => $filename,
          '%project' => $branch->project,
          '%branch' => $branch->branch_name,
        ), WATCHDOG_WARNING);
    }
    unset($new['namespace']);
  }

  // We're supporting only non-conflicting use declarations.
  if (isset($new['use_alias']) && isset($master['use_alias'])) {
    foreach ($new['use_alias'] as $alias => $class) {
      if (isset($master['use_alias'][$alias]) && $master['use_alias'][$alias] != $class) {
        watchdog('api',
          'Conflicting use declarations for %name found in file @file in %project %branch. Only first is used.',
          array(
            '%name' => $alias,
            '@file' => $filename,
            '%project' => $branch->project,
            '%branch' => $branch->branch_name,
          ), WATCHDOG_WARNING);
        unset($new['use_alias'][$alias]);
      }
    }
  }

  // Use array_replace_recursive here so we do not get duplicate references.
  return array_replace_recursive($master, $new);
}

/**
 * Saves documentation information to the database.
 *
 * @param array $docblocks
 *   An array containing information about API documentation items. The first
 *   array element should be for the file as a whole, and subsequent elements
 *   should have information for items declared in that file (if any).
 */
function _api_save_docblocks(array $docblocks) {
  $nested_groups = array();
  $namespace = '';
  $use_aliases = array();

  // Make a list of the documentation IDs that were contained in this file
  // the last time through. We'll remove any items that are not in this file
  // any more after we get through the $docblocks array.
  $old_dids = db_select('api_documentation', 'd')
    ->fields('d', array('did'))
    ->condition('branch_id', $docblocks[0]['branch']->branch_id)
    ->condition('file_name', $docblocks[0]['file_name'])
    ->execute()
    ->fetchCol();

  $dids = array();
  $class_dids = array();
  $function_insert_query = NULL;
  $functions_to_delete = array();

  // Look for @file block first so $docblocks[0] gets filled in before it
  // is processed.
  foreach ($docblocks as $docblock) {
    if (preg_match('/' . API_RE_TAG_START . 'file/', $docblock['content'])) {
      $content = $docblock['content'];
      // Remove @file tag from this docblock.
      $content = str_replace('@file', '', $content);

      // If this docblock contains @mainpage or @defgroup, this will cause
      // problems, because we won't have a @file doc block any more -- it will
      // be co-opted, and then the site will be screwed up. So, remove these
      // tags and save a watchdog message.
      if (preg_match('/' . API_RE_TAG_START . 'mainpage/', $content) ||
        preg_match('/' . API_RE_TAG_START . 'defgroup/', $content)) {

        $content = str_replace('@mainpage', '', $content);
        $content = str_replace('@defgroup', '', $content);

        watchdog('api',
          '@file docblock containing @defgroup or @mainpage in %file at line %line in %project %branch. Extraneous tags ignored.',
          array(
            '%file' => $docblocks[0]['file_name'],
            '%line' => $docblock['start_line'],
            '%project' => $docblock['branch']->project,
            '%branch' => $docblock['branch']->branch_name,
          ), WATCHDOG_WARNING);
      }

      $docblocks[0]['content'] = $content;
      break;
    }
  }

  // Take care of the (bad, but possible) case where the doc block for one
  // of the items in the file (class, function, etc.) has a @defgroup
  // or @mainpage in it, by separating the doc block from the item.
  $old_blocks = $docblocks;
  $docblocks = array();
  foreach ($old_blocks as $docblock) {
    if ($docblock['code'] && $docblock['content'] &&
      (preg_match('/' . API_RE_TAG_START . 'mainpage/', $docblock['content']) ||
       preg_match('/' . API_RE_TAG_START . 'defgroup/', $docblock['content']))) {
      $new_block = $docblock;
      // Make one block have just the code and the other, just the docs.
      $new_block['code'] = '';
      $docblock['content'] = '';
      $docblocks[] = $new_block;
      $docblocks[] = $docblock;
      watchdog('api',
        'Item docblock containing @defgroup or @mainpage in %file at line %line in %project %branch. Separated into two blocks.',
        array(
          '%file' => $docblocks[0]['file_name'],
          '%line' => $docblock['start_line'],
          '%project' => $docblock['branch']->project,
          '%branch' => $docblock['branch']->branch_name,
        ), WATCHDOG_WARNING);
    }
    else {
      $docblocks[] = $docblock;
    }
  }

  $need_to_reparse = FALSE;
  foreach ($docblocks as $docblock) {
    // Keep track of the namespace and add it to all docblocks for this file.
    if (isset($docblock['references']['namespace']) && !empty($docblock['references']['namespace'])) {
      $namespace = $docblock['references']['namespace'];
    }
    $docblock['namespace'] = $namespace;

    // Keep track of the use aliases so we can put the right classes into the
    // extends/implements references for classes we encounter.
    if (isset($docblock['references']['use_alias'])) {
      $use_aliases = array_merge($docblock['references']['use_alias'], $use_aliases);
    }

    // Change @Annotation to @ingroup annotation.
    $annotation_matches = array();
    $docblock['annotation_class'] = FALSE;
    if (preg_match('/' . API_RE_TAG_START . 'Annotation' . API_RE_WORD_BOUNDARY_END . '/', $docblock['content'], $annotation_matches)) {
      if ($docblock['is_drupal']) {
        $docblock['content'] = str_replace($annotation_matches[0], "\n@ingroup annotation\n", $docblock['content']);
      }
      $docblock['annotation_class'] = TRUE;
    }

    // Change @Event to @ingroup events.
    $event_matches = array();
    if (preg_match('/' . API_RE_TAG_START . 'Event' . API_RE_WORD_BOUNDARY_END . '/', $docblock['content'], $event_matches)) {
      $docblock['content'] = str_replace($event_matches[0], "\n@ingroup events\n", $docblock['content']);
    }

    // Deal with @mainpage.
    if (preg_match('/' . API_RE_TAG_START . 'mainpage/', $docblock['content'])) {
      preg_match('/' . API_RE_TAG_START . 'mainpage (.*?)\n/', $docblock['content'], $mainpage_matches);
      $docblock['title'] = (isset($mainpage_matches[1]) ? $mainpage_matches[1] : '');
      $docblock['content'] = preg_replace('/' . API_RE_TAG_START . 'mainpage.*?\n/', '', $docblock['content']);
      $docblock['object_type'] = 'mainpage';
      $docblock['object_name'] = $docblocks[0]['branch']->branch_name;
    }
    // Deal with @defgroup.
    elseif (preg_match('/' . API_RE_TAG_START . 'defgroup/', $docblock['content'])) {
      if (preg_match('/' . API_RE_TAG_START . 'defgroup ([a-zA-Z0-9_.-]+) +(.*?)\n/', $docblock['content'], $group_matches)) {
        $group_name = $group_matches[1];
        // See if the group already exists in another file in this branch, and
        // if so, treat this as an @addtogroup.
        $did = db_select('api_documentation', 'd')
          ->condition('object_name', $group_name)
          ->condition('object_type', 'group')
          ->condition('file_name', $docblock['file_name'], '<>')
          ->condition('branch_id', $docblocks[0]['branch']->branch_id)
          ->fields('d', array('did'))
          ->execute()
          ->fetchField();
        if ($did > 0) {
          $docblock['content'] = str_replace('defgroup', 'addtogroup', $docblock['content']);
          watchdog('api',
            'Duplicate @defgroup in %file at line %line treated as @addtogroup in %project %branch.',
            array(
              '%file' => $docblocks[0]['file_name'],
              '%line' => $docblock['start_line'],
              '%project' => $docblock['branch']->project,
              '%branch' => $docblock['branch']->branch_name,
            ), WATCHDOG_WARNING);
          // This is commonly because the defgroup was moved from one file
          // to another. So requeue both the found ID and the current file
          // we are working on.
          _api_requeue_after_run($did);
          $need_to_reparse = TRUE;
        }
        else {
          $docblock['object_name'] = $group_name;
          $docblock['title'] = $group_matches[2];
          $docblock['content'] = preg_replace('/' . API_RE_TAG_START . 'defgroup.*?\n/', '', $docblock['content']);
          $docblock['object_type'] = 'group';
        }
      }
      else {
        watchdog('api',
          'Malformed @defgroup in %file at line %line in %project %branch.',
          array(
            '%file' => $docblocks[0]['file_name'],
            '%line' => $docblock['start_line'],
            '%project' => $docblock['branch']->project,
            '%branch' => $docblock['branch']->branch_name,
          ), WATCHDOG_WARNING);
      }
    }

    // Determine group membership.
    $match = array();
    if (preg_match_all('/' . API_RE_TAG_START . '(ingroup|addtogroup) ([a-zA-Z0-9_.-]+)/', $docblock['content'], $match)) {
      $docblock['groups'] = $match[2];
      $docblock['content'] = preg_replace('/' . API_RE_TAG_START . '(ingroup|addtogroup).*?\n/', '', $docblock['content']);
    }

    // Handle nested groups.
    if (!isset($nested_groups[$docblock['class']])) {
      $nested_groups[$docblock['class']] = array();
    }
    foreach ($nested_groups[$docblock['class']] as $group_id) {
      if (!empty($group_id)) {
        $docblock['groups'][] = $group_id;
      }
    }
    if (preg_match('/' . API_RE_TAG_START . '{/', $docblock['content'])) {
      if ($docblock['object_type'] === 'group') {
        array_push($nested_groups[$docblock['class']], $docblock['object_name']);
      }
      elseif (isset($docblock['groups'])) {
        array_push($nested_groups[$docblock['class']], reset($docblock['groups']));
      }
      else {
        array_push($nested_groups[$docblock['class']], '');
      }
    }
    if (preg_match('/' . API_RE_TAG_START . '}/', $docblock['content'])) {
      array_pop($nested_groups[$docblock['class']]);
    }

    // At this point, we might have been dealing with a "block" that is
    // just an @} or an object with no name, or something like that. We needed
    // to do the processing above, but we don't want to save this as an object.
    if (empty($docblock['object_type']) || empty($docblock['object_name'])) {
      continue;
    }

    // Treat {@inheritdoc} as a blank doc block, meaning inherit from parent.
    $docblock['content'] = str_replace('{@inheritdoc}', '', $docblock['content']);
    // Some vendor files use @inheritDoc instead.
    $docblock['content'] = str_replace('{@inheritDoc}', '', $docblock['content']);

    if ($docblock['content'] && trim($docblock['content'])) {
      // Find parameter definitions with @param.
      $params = '';
      while (preg_match('/' . API_RE_TAG_START . 'param\s(.*?)(?=\n' . API_RE_TAG_START . '|$)/s', $docblock['content'], $param_match)) {
        $docblock['content'] = str_replace($param_match[0], '', $docblock['content']);
        // Add some formatting to the parameter -- strong tag for everything
        // that was on the @param line, and a colon after. Note that tags
        // are stripped out below, so we use [strong] and then fix it later.
        $this_param = $param_match[1];
        $this_param = preg_replace('|^([^\n]+)|', '[strong]$1[/strong]:', $this_param);
        $params .= "\n\n" . $this_param;
      }
      // Format and then replace our fake tags with real ones.
      $tmp = array();
      $params = _api_format_documentation($params, TRUE, $tmp);
      $params = str_replace('[strong]', '<strong>', $params);
      $params = str_replace('[/strong]', '</strong>', $params);
      $docblock['parameters'] = $params;

      // Find return value definitions with @return.
      $docblock['return_value'] = '';
      preg_match_all('/' . API_RE_TAG_START . 'return\s(.*?)(?=\n' . API_RE_TAG_START . '|$)/s', $docblock['content'], $return_matches, PREG_SET_ORDER);
      foreach ($return_matches as $return_match) {
        $docblock['content'] = str_replace($return_match[0], '', $docblock['content']);
        $docblock['return_value'] .= "\n\n" . $return_match[1];
      }
      $docblock['return_value'] = _api_format_documentation($docblock['return_value'], TRUE, $tmp);

      // Find @see lines.
      $docblock['see'] = '';
      while (preg_match('/' . API_RE_TAG_START . 'see\s(.*?)\n/s', $docblock['content'], $match)) {
        $docblock['content'] = str_replace($match[0], '', $docblock['content']);
        $docblock['see'] .= "\n\n" . $match[1];
      }
      $docblock['see'] = _api_format_documentation($docblock['see'], TRUE, $tmp);

      // Find @var, a class or variable type name.
      $docblock['var'] = '';
      if (preg_match('/' . API_RE_TAG_START . 'var\s(.*?)\n/s', $docblock['content'], $match)) {
        $docblock['content'] = str_replace($match[0], '', $docblock['content']);
        $docblock['var'] = trim($match[1]);
      }

      // Find @throws, a paragraph (could be more than one).
      $docblock['throws'] = '';
      while (preg_match('/' . API_RE_TAG_START . 'throws\s(.*?)(?=\n' . API_RE_TAG_START . '|$)/s', $docblock['content'], $match)) {
        $docblock['content'] = str_replace($match[0], '', $docblock['content']);
        $docblock['throws'] .= "\n\n" . $match[1];
      }
      $docblock['throws'] = _api_format_documentation($docblock['throws'], TRUE, $tmp);

      // Find @deprecated, a paragraph (could be more than one).
      $docblock['deprecated'] = '';
      while (preg_match('/' . API_RE_TAG_START . 'deprecated\s(.*?)(?=\n' . API_RE_TAG_START . '|$)/s', $docblock['content'], $match)) {
        $docblock['content'] = str_replace($match[0], '', $docblock['content']);
        $docblock['deprecated'] .= "\n\n" . $match[1];
      }
      $docblock['deprecated'] = _api_format_documentation($docblock['deprecated'], TRUE, $tmp);

      // Format everything remaining as the main documentation.
      $docblock['documentation'] = _api_format_documentation($docblock['content'], TRUE, $docblock['references']);
    }

    // Grab the first line as a summary, unless already provided.
    if (!isset($docblock['summary'])) {
      $docblock['summary'] = _api_documentation_summary($docblock['documentation']);
    }

    if (!empty($docblock['class'])) {
      $docblock['class_did'] = $class_dids[$docblock['class']];
    }

    // Figure out the namespaced name.
    $docblock['namespaced_name'] = api_full_classname($docblock['object_name'], $namespace, $use_aliases);

    // See if this docblock already existed, and get its ID.
    $docblock['did'] = db_select('api_documentation', 'd')
      ->fields('d', array('did'))
      ->condition('object_name', $docblock['object_name'])
      ->condition('file_name', $docblock['file_name'])
      ->condition('object_type', $docblock['object_type'])
      ->condition('branch_id', $docblocks[0]['branch']->branch_id)
      ->execute()
      ->fetchField();

    if ($docblock['did'] > 0) {
      // Verify that this same object wasn't already in this file in this run.
      if (in_array($docblock['did'], $dids)) {
        watchdog('api', 'Duplicate item found in file %file at line %line in %project %branch. Only first instance of %name is saved', array(
          '%file' => $docblocks[0]['file_name'],
          '%line' => $docblock['start_line'],
          '%name' => $docblock['object_name'],
          '%project' => $docblock['branch']->project,
          '%branch' => $docblock['branch']->branch_name,
        ), WATCHDOG_WARNING);
        continue;
      }
      api_add_text_defaults($docblock, 'api_documentation');
      drupal_write_record('api_documentation', $docblock, 'did');
    }
    else {
      $nid = api_new_documentation_node();
      $docblock['did'] = $nid;
      $docblock['branch_id'] = $docblocks[0]['branch']->branch_id;
      api_add_text_defaults($docblock, 'api_documentation');
      drupal_write_record('api_documentation', $docblock);
    }

    // Keep track of class membership.
    if ($docblock['object_type'] === 'class' || $docblock['object_type'] === 'interface' || $docblock['object_type'] == 'trait') {
      $class_dids[$docblock['object_name']] = $docblock['did'];
      _api_touched($docblock['did']);
    }

    switch ($docblock['object_type']) {
      case 'function':
        api_add_text_defaults($docblock, 'api_function');
        // Combine all the deletes/inserts for {api_function} into one query
        // each so that they will run faster, since we don't need this table
        // to be updated during the processing of this file.
        $functions_to_delete[] = $docblock['did'];
        if (is_null($function_insert_query)) {
          $function_insert_query = db_insert('api_function')
            ->fields(array('did', 'signature', 'parameters', 'return_value'));
        }
        $function_insert_query->values(array(
          'did' => $docblock['did'],
          'signature' => $docblock['signature'],
          'parameters' => $docblock['parameters'],
          'return_value' => $docblock['return_value'],
        ));
        _api_replace_references($docblocks[0]['branch'], $docblock['did'], $docblock['object_name'], $docblock['references'], FALSE, $namespace, $use_aliases);
        break;

      case 'service':
        _api_replace_references($docblock['branch'], $docblock['did'], $docblock['object_name'], $docblock['references'], FALSE, $namespace, $use_aliases);
        break;

      case 'file':
        // Note that this will set the 'queued' field back to zero, since
        // $docblock['queued'] is set to 0 up at the top of api_parse_file().
        drupal_write_record('api_file', $docblock, 'did');
        _api_replace_references($docblock['branch'], $docblock['did'], $docblock['object_name'], $docblock['references'], TRUE, $namespace, $use_aliases);
        break;

      case 'interface':
      case 'class':
      case 'trait':
        db_delete('api_reference_storage')
          ->condition('branch_id', $docblocks[0]['branch']->branch_id)
          ->condition('object_type', array(
            'class',
            'interface',
            'trait',
            'annotation_class',
            'annotation',
            'element',
          ))
          ->condition('from_did', $docblock['did'])
          ->execute();
        db_delete('api_namespace')
          ->condition('did', $docblock['did'])
          ->condition('object_type', array('trait_alias', 'trait_precedence'))
          ->execute();

        foreach ($docblock['extends'] as $extend) {
          $refname = api_full_classname($extend, $namespace, $use_aliases);
          _api_reference($docblocks[0]['branch'], 'class', $refname, $docblock['did']);
        }
        foreach ($docblock['implements'] as $implement) {
          $refname = api_full_classname($implement, $namespace, $use_aliases);
          _api_reference($docblocks[0]['branch'], 'interface', $refname, $docblock['did']);
        }

        if (isset($docblock['references']['use_trait'])) {
          foreach ($docblock['references']['use_trait'] as $alias => $info) {
            $class = $info['class'];
            $refname = api_full_classname($class, $namespace, $use_aliases);
            $refalias = api_full_classname($alias, $namespace, $use_aliases);
            if ($refname != $refalias) {
              watchdog('api', 'Aliases for use statements for traits are not supported in %filename for alias %alias of %class in %project %branch', array(
                '%filename' => $docblock['file_name'],
                '%alias' => $alias,
                '%class' => $class,
                '%project' => $docblock['branch']->project,
                '%branch' => $docblock['branch']->branch_name,
              ), WATCHDOG_WARNING);
            }
            _api_reference($docblocks[0]['branch'], 'trait', $refname, $docblock['did']);
            // If there are insteadof/alias details for this trait, save them
            // in the namespaces table (because it has the right columns).
            if (isset($info['details'])) {
              foreach ($info['details'] as $type => $list) {
                foreach ($list as $name => $item) {
                  if ($type == 'precedence') {
                    // This is an insteadof statement.
                    $name = api_full_classname($name, $namespace, $use_aliases);
                    _api_namespace($docblock['did'], 'trait_' . $type, $name, $item);
                  }
                  elseif (in_array($name, array(
                    'public',
                    'protected',
                    'private',
                  ))) {
                    watchdog('api', 'Trait inheritance that changes visibility is not supported in %filename for %item in %project %branch', array(
                      '%filename' => $docblock['file_name'],
                      '%item' => $item,
                      '%project' => $docblock['branch']->project,
                      '%branch' => $docblock['branch']->branch_name,
                    ), WATCHDOG_WARNING);
                  }
                  else {
                    _api_namespace($docblock['did'], 'trait_' . $type, $name, $refname . '::' . $item);
                  }
                }
              }
            }
          }
        }
        if (isset($docblock['references']['annotation'])) {
          foreach ($docblock['references']['annotation'] as $class) {
            _api_reference($docblocks[0]['branch'], 'annotation', $class, $docblock['did']);
          }
        }
        if (isset($docblock['references']['element'])) {
          foreach ($docblock['references']['element'] as $element_type) {
            _api_reference($docblocks[0]['branch'], 'element', $element_type, $docblock['did']);
          }
        }
        if ($docblock['annotation_class']) {
          _api_reference($docblocks[0]['branch'], 'annotation_class', $docblock['object_name'], $docblock['did']);
        }

        break;
    }

    db_delete('api_reference_storage')
      ->condition('branch_id', $docblocks[0]['branch']->branch_id)
      ->condition('object_type', 'group')
      ->condition('from_did', $docblock['did'])
      ->execute();

    if (isset($docblock['groups'])) {
      foreach ($docblock['groups'] as $group_name) {
        _api_reference($docblocks[0]['branch'], 'group', $group_name, $docblock['did']);
      }
    }

    $dids[] = $docblock['did'];
  }

  if ($need_to_reparse && count($dids)) {
    // Mark any DID found as the one to requeue -- they should all be from
    // the same file.
    _api_requeue_after_run($dids[0]);
  }

  // Run the queued-up queries on the api_function table.
  if (count($functions_to_delete) > 0) {
    db_delete('api_function')
      ->condition('did', $functions_to_delete)
      ->execute();
  }
  if (!is_null($function_insert_query)) {
    $function_insert_query->execute();
  }

  // Invoke hook_api_updated for doc objects that have been updated.
  $changed_dids = array_intersect($dids, $old_dids);
  if (count($changed_dids)) {
    module_invoke_all('api_updated', $changed_dids);
  }

  // Clean out all of the doc objects from this file that no longer exist.
  $old_dids = array_diff($old_dids, $dids);
  if (count($old_dids)) {
    module_load_include('inc', 'api', 'api.db');
    api_delete_items($old_dids, FALSE);
  }

  _api_schedule_shutdown();
}

/**
 * Replaces any existing references for function/hook calls with new ones.
 *
 * @param object $branch
 *   Branch object for the item making the calls.
 * @param int $did
 *   Documentation ID of the item making the calls.
 * @param string $name
 *   Name of the item making the calls.
 * @param array $references
 *   Array of references set up by _api_find_php_references().
 * @param bool $do_namespaces
 *   TRUE to save namespace references. FALSE (default) to ignore them.
 * @param string $namespace
 *   Namespace for the file these references are from.
 * @param array $use_aliases
 *   Use aliases for the file these references are from.
 */
function _api_replace_references($branch, $did, $name, array $references, $do_namespaces, $namespace, array $use_aliases) {
  // Find the name without class prefix, if any.
  $shortname = $name;
  if (($pos = strpos($name, '::')) !== FALSE && $pos > 1) {
    $shortname = substr($name, $pos + 2);
  }

  // Remove any existing references.
  $types_to_remove = array(
    'function',
    'potential hook',
    'potential fieldhook',
    'potential entityhook',
    'potential userhook',
    'potential theme',
    'potential element',
    'potential alter',
    'potential callback',
    'potential file',
    'constant',
    'member-parent',
    'member-self',
    'member',
    'member-class',
    'yaml string',
    'service_tag',
    'service_class',
  );

  db_delete('api_reference_storage')
    ->condition('branch_id', $branch->branch_id)
    ->condition('object_type', $types_to_remove)
    ->condition('from_did', $did)
    ->execute();

  if ($do_namespaces) {
    $types_to_remove = array('namespace', 'use_alias');
    db_delete('api_namespace')
      ->condition('did', $did)
      ->condition('object_type', $types_to_remove)
      ->execute();
  }

  // Add the new references.
  foreach ($references as $type => $list) {
    switch ($type) {
      case 'namespace':
        if ($do_namespaces) {
          // In this case, $list is actually a string containing the namespace
          // for a file, not a list.
          _api_namespace($did, $type, '', $list, TRUE);
        }
        break;

      case 'use_alias':
        if ($do_namespaces) {
          // Save all of the use aliases.
          foreach ($list as $alias => $class) {
            _api_namespace($did, $type, $alias, $class, TRUE);
          }
        }
        break;

      case 'member-class':
        // Don't save a reference to the item itself.
        unset($list[$shortname]);
        foreach ($list as $call) {
          // These are references to ClassName::method(). Make sure
          // they are fully namespaced.
          $call = api_full_classname($call, $namespace, $use_aliases);
          _api_reference($branch, $type, $call, $did, TRUE);
        }

        break;

      case 'potential callback':
      case 'function':
      case 'member-self':
        // Don't save a reference to the item itself.
        unset($list[$shortname]);

        // Intentional fallthrough here.
      default:
        foreach ($list as $call) {
          // If the name contains a backslash, and the first occurrence is not
          // at the beginning, make sure it starts with a backslash
          // so it is a fully-namespaced reference.
          $pos = strpos($call, '\\');
          if ($pos !== FALSE && $pos !== 0) {
            $call = '\\' . $call;
          }

          _api_reference($branch, $type, $call, $did, TRUE);
        }
    }
  }

  // Cause all the queued references to get saved.
  _api_reference(TRUE);
  if ($do_namespaces) {
    _api_namespace(TRUE);
  }
}

/**
 * Formats documentation comment text as HTML.
 *
 * First escapes all HTML tags. Then processes links and code blocks, and
 * converts newlines into paragraphs. Note that this function does not do any
 * Drupal-specific formatting, aside from formatting plugin annotation, which
 * should be fine for vendor files as well.
 *
 * @param string $documentation
 *   Documentation string to format.
 * @param bool $make_paragraphs
 *   TRUE (default) to convert to paragraphs. FALSE to skip this conversion and
 *   put the documentation in PRE tags.
 * @param array $references
 *   Array of references. If this function finds references (only for plugin
 *   annotation), this array may be added to.
 *
 * @return string
 *   Formatted documentation.
 */
function _api_format_documentation($documentation, $make_paragraphs, array &$references) {
  // Don't do processing on empty text (so we don't end up with empty
  // paragraphs).
  if (empty($documentation)) {
    return '';
  }

  // Check for invalid Unicode, which screws everything up.
  if (!drupal_validate_utf8($documentation)) {
    return t('Non-displayable characters.');
  }

  $connection = Database::getConnection();
  if (!method_exists($connection, 'utf8mb4IsActive') || !$connection->utf8mb4IsActive()) {
    // Replace 4-byte characters not supported by MySQL's utf8 encoding.
    $documentation = preg_replace('/[^\x{0000}-\x{FFFF}]/u', '�', $documentation);
  }

  $documentation = check_plain($documentation);

  // @link full URLs.
  $documentation = preg_replace('/' . API_RE_TAG_START . 'link ((http:\/\/|https:\/\/|ftp:\/\/|mailto:|smb:\/\/|afp:\/\/|file:\/\/|gopher:\/\/|news:\/\/|ssl:\/\/|sslv2:\/\/|sslv3:\/\/|tls:\/\/|tcp:\/\/|udp:\/\/)([a-zA-Z0-9@:%_+*!~#?&=.,\/;-]*[a-zA-Z0-9@:%_+*!~#&=\/;-])) (.*?) ' . API_RE_TAG_START . 'endlink/', '<a href="$1">$4</a>', $documentation);
  // Site URLs.
  $documentation = preg_replace('/' . API_RE_TAG_START . 'link \/([a-zA-Z0-9_\/-]+) (.*?) ' . API_RE_TAG_START . 'endlink/', str_replace('%24', '$', '<a href="$1">$2</a>'), $documentation);

  // Process sections.
  $regexp = '/^' . API_RE_TAG_START . 'section ([a-zA-Z0-9_-]+) (.*)$/m';
  preg_match_all($regexp, $documentation, $section_matches, PREG_SET_ORDER);
  if (!empty($section_matches)) {
    $documentation = preg_replace($regexp, '<h3 id="$1">$2</h3>', $documentation);
  }

  // Process sub-sections.
  $regexp = '/^' . API_RE_TAG_START . 'subsection ([a-zA-Z0-9_-]+) (.*)$/m';
  preg_match_all($regexp, $documentation, $subsection_matches, PREG_SET_ORDER);
  if (!empty($subsection_matches)) {
    $documentation = preg_replace($regexp, '<h4 id="$1">$2</h4>', $documentation);
  }

  // Process in-page references to sections/subsections.
  if (!empty($section_matches) || !empty($subsection_matches)) {
    $search = array();
    $replace = array();
    foreach (array_merge($section_matches, $subsection_matches) as $match) {
      array_shift($match);
      $id = array_shift($match);
      $caption = trim(array_shift($match));
      $search[] = '/' . API_RE_TAG_START . 'ref ' . $id . '/';
      // Note that we cannot use l() here to make the link -- it doesn't have
      // a way to make a self-link, since url('') is a synonym for front page.
      // Also note that the ID here is already constrained to legal characters
      // by the regexp above.
      $replace[] = '<a href="#' . $id . '">' . check_plain($caption) . '</a>';
    }
    $documentation = preg_replace($search, $replace, $documentation);
  }

  // Replace left over curly braces.
  $documentation = preg_replace('/' . API_RE_TAG_START . '[{}]/', '', $documentation);

  // Change @Plugin and other annotation sections into @code. They have to be
  // at the very end of the documentation block. And add a reference.
  $annotation_matches = array();
  if (preg_match('/' . API_RE_TAG_START . '(' . API_RE_FUNCTION_CHARACTERS . ')(\(.*\))\s*$/s', $documentation, $annotation_matches)) {
    $class = $annotation_matches[1];
    $references['annotation'][$class] = $class;
    $documentation = str_replace($annotation_matches[0], '<h3>' . t('Plugin annotation') . '</h3>' . "\n@code\n@" . $annotation_matches[1] . $annotation_matches[2] . "\n@endcode", $documentation);
    $element_types = array('FormElement', 'RenderElement');
    if (in_array($class, $element_types)) {
      // This is annotation like @FormElement("button")
      // or @RenderElement("table"). Extract the element machine name
      // (button/table) from $annotation_matches[2] and save it as a reference.
      $element_type = trim(decode_entities($annotation_matches[2]), " \t\n\r()'\"");
      $references['element'][$element_type] = $element_type;
    }
  }

  // Process the @code @endcode tags.
  $documentation = preg_replace_callback('/' . API_RE_TAG_START . 'code(.+?)' . API_RE_TAG_START . 'endcode/s', '_api_format_embedded_php', $documentation);

  // Convert newlines into paragraphs.
  if ($make_paragraphs) {
    $documentation = _api_make_paragraphs($documentation);
  }
  else {
    $documentation = '<pre class="api-text">' . $documentation . '</pre>';
  }

  return $documentation;
}

/**
 * Converts newlines into paragraphs.
 *
 * Like _filter_autop(), but does not add <br /> tags.
 *
 * @param string $text
 *   Text to convert.
 *
 * @return string
 *   Converted text.
 */
function _api_make_paragraphs($text) {
  // All block level tags.
  $block = '(?:table|thead|tfoot|caption|colgroup|tbody|tr|td|th|div|dl|dd|dt|ul|ol|li|pre|select|form|blockquote|address|p|h[1-6]|hr)';

  // Split at <pre>, <script>, <style> and </pre>, </script>, </style> tags.
  // We don't apply any processing to the contents of these tags to avoid
  // messing up code. We look for matched pairs and allow basic nesting. For
  // example: "processed <pre> ignored <script> ignored </script>
  // ignored </pre> processed".
  $chunks = preg_split('@(</?(?:pre|script|style|object)[^>]*>)@i', $text, -1, PREG_SPLIT_DELIM_CAPTURE);
  // Note: PHP ensures the array consists of alternating delimiters and literals
  // and begins and ends with a literal (inserting NULL as required).
  $ignore = FALSE;
  $ignoretag = '';
  $output = '';
  foreach ($chunks as $i => $chunk) {
    if ($i % 2) {
      // Opening or closing tag?
      $open = ($chunk[1] != '/');
      list($tag) = preg_split('/[ >]/', substr($chunk, 2 - $open), 2);
      if (!$ignore) {
        if ($open) {
          $ignore = TRUE;
          $ignoretag = $tag;
        }
      }
      // Only allow a matching tag to close it.
      elseif (!$open && $ignoretag == $tag) {
        $ignore = FALSE;
        $ignoretag = '';
      }
    }
    elseif (!$ignore) {
      $chunk = _api_format_documentation_lists($chunk);
      // Just to make things a little easier, pad the end.
      $chunk = preg_replace('|\n*$|', '', $chunk) . "\n\n";
      $chunk = preg_replace('|<br />\s*<br />|', "\n\n", $chunk);
      // Space things out a little.
      $chunk = preg_replace('!(<' . $block . '[^>]*>)!', "\n$1", $chunk);
      $chunk = preg_replace('!(</' . $block . '>)!', "$1\n\n", $chunk);
      // Take care of duplicates.
      $chunk = preg_replace("/\n\n+/", "\n\n", $chunk);
      // Make paragraphs, including one at the end.
      $chunk = preg_replace('/\n?(.+?)(?:\n\s*\n|\z)/s', "<p>$1</p>\n", $chunk);
      // Under certain strange conditions it could create a P of entirely
      // whitespace.
      $chunk = preg_replace('|<p>\s*</p>\n|', '', $chunk);
      // Problem with nested lists.
      $chunk = preg_replace("|<p>(<li.+?)</p>|", "$1", $chunk);
      $chunk = preg_replace('|<p><blockquote([^>]*)>|i', "<blockquote$1><p>", $chunk);
      $chunk = str_replace('</blockquote></p>', '</p></blockquote>', $chunk);
      $chunk = preg_replace('!<p>\s*(</?' . $block . '[^>]*>)!', "$1", $chunk);
      $chunk = preg_replace('!(</?' . $block . '[^>]*>)\s*</p>!', "$1", $chunk);
      $chunk = preg_replace('/&([^#])(?![A-Za-z0-9]{1,8};)/', '&amp;$1', $chunk);
    }
    $output .= $chunk;
  }
  return $output;
}

/**
 * Regular expression callback for \@code tags in _api_format_documentation().
 */
function _api_format_embedded_php($matches) {
  $code = decode_entities($matches[1]);
  $parser = (new ParserFactory())->create(ParserFactory::PREFER_PHP7);
  try {
    $statements = $parser->parse("<?php" . $code . "?" . ">");
  }
  catch (ParserError $e) {
    // The code block is not valid PHP. It could be JavaScript, annotation,
    // PHP code with a syntax error, or something else. The best we can do is
    // try to find function-like strings and wrap them in spans for formatting.
    // And recode the HTML entities, so they don't screw up other formatting,
    // but this time avoiding quotes.
    $code = htmlentities($code, ENT_NOQUOTES, 'UTF-8');
    $string_matches = array();
    $wrappers = '\'" @(';
    $possible_matches = array();
    if (preg_match_all('|[' . $wrappers . '](' . API_RE_FUNCTION_IN_TEXT . ')[' . $wrappers . ']|', $code, $string_matches)) {
      $possible_matches = array_unique($string_matches[1]);
      $code = _api_format_yaml_code($code, array('potential callback' => $possible_matches), $wrappers, 'php-function-or-constant-declared');
    }

    return "\n" . '<pre class="php"><code>' . $code . "</code></pre>\n";
  }

  return "\n" . _api_format_statements($statements) . "\n";
}

/**
 * Formats documentation lists as HTML lists.
 *
 * Parses a block of text for lists that uses hyphens or asterisks as bullets,
 * and format the lists as proper HTML lists.
 *
 * @param string $documentation
 *   Documentation string to format.
 *
 * @return string
 *   $documentation with lists formatted.
 */
function _api_format_documentation_lists($documentation) {
  $lines = explode("\n", $documentation);
  $output = '';
  $bullet_indents = array(-1);

  foreach ($lines as $line) {
    preg_match('!^( *)([*-] )?(.*)$!', $line, $matches);
    $indent = strlen($matches[1]);
    $bullet_exists = $matches[2];
    $is_start = FALSE;

    if ($indent < $bullet_indents[0]) {
      // First close off any lists that have completed.
      while ($indent < $bullet_indents[0]) {
        array_shift($bullet_indents);
        $output .= '</li></ul>';
      }
    }

    if ($indent == $bullet_indents[0]) {
      if ($bullet_exists) {
        // A new bullet at the same indent means a new list item.
        $output .= '</li><li>';
        $is_start = TRUE;
      }
      else {
        // If the indent is the same, but there is no bullet, that also
        // signifies the end of the list.
        array_shift($bullet_indents);
        $output .= '</li></ul>';
      }
    }

    if ($indent > $bullet_indents[0] && $bullet_exists) {
      // A new list at a lower level.
      array_unshift($bullet_indents, $indent);
      $output .= '<ul><li>';
      $is_start = TRUE;
    }

    // At the start of a bullet, if there is a ":" followed by a space, put
    // everything before the : in bold.
    if ($is_start && (($p = strpos($matches[3], ': ')) > 0)) {
      $matches[3] = '<strong>' . substr($matches[3], 0, $p) . '</strong>' .
        substr($matches[3], $p);
    }
    $output .= $matches[3] . "\n";
  }

  // Clean up any unclosed lists.
  array_pop($bullet_indents);
  foreach ($bullet_indents as $indent) {
    $output .= '</li></ul>';
  }

  // To make sure that _api_make_paragraphs() doesn't get confused, remove
  // newlines immediately before </li> tags.
  $output = str_replace("\n</li>", "</li>", $output);

  return $output;
}

/**
 * Retrieves a summary from a documentation block.
 *
 * @param string $documentation
 *   Documentation block to find the summary of. Should be pre-formatted into
 *   paragraphs.
 *
 * @return string
 *   First paragraph of the documentation block, stripped of tags, and
 *   truncated to 255 characers.
 */
function _api_documentation_summary($documentation) {
  $pos = strpos($documentation, '</p>');
  if ($pos !== FALSE) {
    $documentation = substr($documentation, 0, $pos);
  }
  $documentation = trim(strip_tags($documentation));

  if (strlen($documentation) > 255) {
    return substr($documentation, 0, strrpos(substr($documentation, 0, 252), ' ')) . '…';
  }
  else {
    return $documentation;
  }
}

/**
 * Traverses PHP statements to find references.
 *
 * @param array $statements
 *   Array of statements to traverse from PhpParser parsing.
 * @param bool $is_drupal
 *   TRUE if this is Drupal code; FALSE if not. This turns on recognition of
 *   things like hooks and theme calls.
 * @param string $filename
 *   File name for watchdog messages.
 * @param object $branch
 *   Branch for watchdog messages.
 * @param array $state
 *   (optional) Array to keep track of state for recursive calls.
 *
 * @return array
 *   Array of references found. References are hook invocations, function calls,
 *   etc., and they are put into an associative array where the keys are the
 *   types of references ('function', 'potential hook', etc.), and the values
 *   are arrays of the names of this type that were found.
 */
function _api_find_php_references(array $statements, $is_drupal, $filename, $branch, array $state = array()) {
  $references = array();

  $invoke_function_info = ApiPrettyPrinter::invokeFunctions();
  $statement_count = 0;

  foreach ($statements as $statement) {
    $statement_count++;
    if (!$statement || !is_object($statement)) {
      // This could happen if some of the "sub-statements" in a recursive call
      // were actually empty or scalars.
      continue;
    }

    $type = $statement->getType();
    $sub_statements = array();
    $sub_state = $state;
    $omit_sub_types = array();

    // Find references that are directly in this statement.
    if ($type == 'Expr_FuncCall') {
      // Function call. Only store a reference if it is a directly-
      // named function, not a variable.
      $name = '';
      if ($statement->name && is_object($statement->name) && $statement->name->getType() == 'Name') {
        $name = $statement->name->toString();
      }
      elseif ($statement->name && is_string($statement->name)) {
        $name = $statement->name;
      }

      if ($name) {
        $references['function'][$name] = $name;
        if ($is_drupal && isset($invoke_function_info[$name])) {
          $sub_state['invoke_call'] = $invoke_function_info[$name];
        }
      }

      if (!empty($statement->args)) {
        $sub_statements = _api_combine_statements($sub_statements, $statement->args);
        $omit_sub_types = array('args' => 'args');
      }
    }
    elseif ($type == 'Expr_MethodCall') {
      // Method call. Only store a reference if it is a directly-named method,
      // not a variable.
      $name = '';
      if (is_string($statement->name)) {
        $name = $statement->name;
      }
      // Save as a call reference if it's a method on $this.
      if ($name && $statement->var && isset($statement->var->name) && $statement->var->name == 'this') {
        $references['member-self'][$name] = $name;
      }
      if ($name && $is_drupal && isset($invoke_function_info[$name])) {
        $sub_state['invoke_call'] = $invoke_function_info[$name];
      }

      if (!empty($statement->args)) {
        $sub_statements = _api_combine_statements($sub_statements, $statement->args);
        $omit_sub_types = array('args' => 'args');
      }
    }
    elseif ($type == 'Expr_StaticCall') {
      // Method call on a static class. Only save if it is a directly-named
      // method, not a variable, on a directly-named class or 'self'.
      $name = '';
      if (is_string($statement->name)) {
        $name = $statement->name;
      }
      $class = _api_extract_class_name($statement);
      if ($name && $class) {
        if ($class == 'self' || $class == 'static') {
          $references['member-self'][$name] = $name;
        }
        elseif ($class == 'parent') {
          $references['member-parent'][$name] = $name;
        }
        else {
          $references['member-class'][$class . '::' . $name] = $class . '::' . $name;
        }
        if ($is_drupal && isset($invoke_function_info[$name])) {
          $sub_state['invoke_call'] = $invoke_function_info[$name];
        }
      }

      if (!empty($statement->args)) {
        $sub_statements = _api_combine_statements($sub_statements, $statement->args);
        $omit_sub_types = array('args' => 'args');
      }
    }
    elseif ($type == 'Expr_ConstFetch') {
      // Reference to a constant.
      $name = $statement->name->toString();
      $references['constant'][$name] = $name;
    }
    elseif ($type == 'Expr_ClassConstFetch') {
      // Reference to a class constant. Only store if it is a directly-named
      // class, not a variable like $myclass.
      $class = _api_extract_class_name($statement);
      if ($class) {
        $name = $statement->name;
        $references['constant'][$name] = $class . '::' . $name;
      }
    }
    elseif ($type == 'Scalar_String') {
      $name = $statement->value;
      if ($name) {
        if (!empty($state['invoke_call'])) {
          $references['potential ' . $state['invoke_call'][0]][$name] = $name;
        }
        elseif ($is_drupal && isset($state['array_key']) && $state['array_key'] == '#theme') {
          $references['potential theme'][$name] = $name;
        }
        elseif ($is_drupal && isset($state['array_key']) && $state['array_key'] == '#type') {
          $references['potential element'][$name] = $name;
        }
        elseif (preg_match("|^" . DRUPAL_PHP_FUNCTION_PATTERN . "$|", $name)) {
          $references['potential callback'][$name] = $name;
        }

        if (preg_match("|^" . API_RE_FILENAME . "$|", $name)) {
          // Some of these may be quite long, so truncate.
          $newname = drupal_substr($name, 0, 127);
          $references['potential file'][$newname] = $newname;
        }
      }
    }
    elseif ($type == 'Arg') {
      // Function argument.
      $sub_statements = _api_combine_statements($sub_statements, $statement->value);
      $omit_sub_types = array('value' => 'value');
    }
    elseif ($type == 'Expr_ArrayItem') {
      // Array item.
      if ($statement->key && $statement->key->getType() == 'Scalar_String') {
        $sub_state['array_key'] = $statement->key->value;
      }
      $sub_statements = _api_combine_statements($sub_statements, $statement->value);
      $omit_sub_types = array('value' => 'value');
    }
    elseif ($statement instanceof NodeClassLike) {
      $sub_state['class'] = $statement->name;
      $sub_statements = _api_combine_statements($sub_statements, $statement->stmts);
      $omit_sub_types = array('stmts' => 'stmts');
    }

    // Handle remaining sub-statements.
    $sub_types = array(
      'expr',
      'left',
      'right',
      'vars',
      'items',
      'value',
      'stmts',
      'args',
      'cond',
      'if',
      'else',
      'elseifs',
      'init',
      'loop',
      'cases',
      'catches',
      'finally',
    );
    $sub_types = array_unique(array_merge($statement->getSubNodeNames(), $sub_types));
    foreach ($sub_types as $thing) {
      if (!isset($omit_sub_types[$thing]) && isset($statement->$thing)) {
        $sub_statements = _api_combine_statements($sub_statements, $statement->$thing);
      }
    }

    // Recursively find references in sub-statements.
    if ($sub_statements) {
      $references = _api_merge_references($references, _api_find_php_references($sub_statements, $is_drupal, $filename, $branch, $sub_state), $filename, $branch);
    }

    // After processing the argument of functions where a hook name could be,
    // remove the possibility of finding more matches in later arguments.
    if (!empty($state['invoke_call']) &&
      $statement_count >= $state['invoke_call'][1]) {
      $state['invoke_call'] = FALSE;
    }
  }

  return $references;
}

/**
 * Adds items to statements array.
 *
 * @param array $old_statements
 *   Existing statements array.
 * @param array|object $new_statements
 *   New statement or statements to add.
 *
 * @return array
 *   Array of statements containing all of them.
 */
function _api_combine_statements($old_statements, $new_statements) {
  if (is_array($new_statements)) {
    foreach ($new_statements as $item) {
      $old_statements[] = $item;
    }
  }
  else {
    $old_statements[] = $new_statements;
  }
  return $old_statements;
}

/**
 * Extracts the class name from a statement.
 *
 * @param object $statement
 *   Statement to extract the class name from.
 *
 * @return string
 *   Class name, if the statement has one; empty string otherwise.
 */
function _api_extract_class_name($statement) {
  if (!$statement->class) {
    return '';
  }

  // $statement->class is an object, hopefully some type of a "name".
  $type = $statement->class->getType();
  if ($type != 'Name' && strpos($statement->class->getType(), 'Name_') !== 0) {
    return '';
  }

  $class = $statement->class->toString();
  if ($statement->class->isFullyQualified()) {
    $class = '\\' . $class;
  }

  return $class;
}

/**
 * Formats statements as PHP code set up for linking at output time.
 *
 * @param array $statements
 *   Array of statements from PhpParser parsing.
 * @param bool $is_drupal
 *   (optional) TRUE (default) if this is Drupal code; FALSE if not. This turns
 *   on recognition of things like hooks and theme calls.
 * @param bool $is_file
 *   (optional) TRUE if this is a file (to print the opening ?php tag). FALSE
 *   (default) if not.
 *
 * @return string
 *   HTML-formatted code, with spans enclosing various PHP elements.
 */
function _api_format_statements(array $statements, $is_drupal = TRUE, $is_file = FALSE) {

  $printer = new ApiPrettyPrinter(array('isDrupal' => $is_drupal));
  if ($is_file) {
    $code = $printer->prettyPrintFile($statements);
  }
  else {
    $code = $printer->prettyPrint($statements);
  }

  // Check for invalid Unicode, which screws everything up.
  if (!drupal_validate_utf8($code)) {
    return t('Non-displayable characters.');
  }

  $connection = Database::getConnection();
  if (!method_exists($connection, 'utf8mb4IsActive') || !$connection->utf8mb4IsActive()) {
    // Replace 4-byte characters not supported by MySQL's utf8 encoding.
    $code = preg_replace('/[^\x{0000}-\x{FFFF}]/u', '�', $code);
  }

  $code = '<pre class="php"><code>' . $code . '</code></pre>';

  return $code;
}

/**
 * Numbers lines in code.
 *
 * @param string $code
 *   Code to number.
 * @param int $number
 *   (optional) Number to start with, if different from 1.
 *
 * @return string
 *   Numbered code. Uses OL list.
 */
function _api_number_lines($code, $number = 1) {
  $start = (is_int($number)) ? $number : 1;
  $lines = explode("\n", $code);
  // If the last line is empty, omit it.
  $last = array_pop($lines);
  if ($last) {
    array_push($lines, $last);
  }
  $output = '<ol class="code-lines" start="' . $start . '"><li>' . implode("\n</li><li>", $lines) . "\n</li></ol>";

  return $output;
}

/**
 * Adds a reference to the {api_reference_storage} table.
 *
 * Since we may parse a file containing a reference before we have parsed the
 * file containing the referenced object, we keep track of the references
 * using a scratch table.
 *
 * @param object $branch
 *   Object representing the branch the reference is in, or TRUE to execute
 *   all of the saved-up inserts.
 * @param string $to_type
 *   Type of object being referenced.
 * @param string $to_name
 *   Name of object being referenced.
 * @param int $from_did
 *   Documentation ID of the object that references this object.
 * @param bool $wait
 *   TRUE to save the insert until _api_reference(TRUE) is called, or FALSE
 *   (default) to do the query immediately.
 */
function _api_reference($branch, $to_type = '', $to_name = '', $from_did = '', $wait = FALSE) {
  static $is_php_function = array();
  static $query_stored = NULL;

  // Is it time to do all the saved queries?
  if ($branch === TRUE) {
    if (!is_null($query_stored)) {
      $query_stored->execute();
      $query_stored = NULL;
    }
    return;
  }

  // Don't make references to built-in PHP functions.
  if ($to_type == 'function' && !isset($is_php_function[$to_name])) {
    $is_php_function[$to_name] = db_select('api_php_documentation', 'd')
      ->fields('d', array('did'))
      ->condition('d.object_name', $to_name)
      ->execute()
      ->fetchField();
  }

  // Avoid trying to save really long object names.
  $to_name = drupal_substr($to_name, 0, 127);

  if ($to_type != 'function' || !$is_php_function[$to_name]) {
    if ($wait) {
      if (is_null($query_stored)) {
        $query_stored = db_insert('api_reference_storage')
          ->fields(array(
            'object_name',
            'branch_id',
            'object_type',
            'from_did',
            'extends_did',
          ));
      }
      $query_stored->values(array(
        'object_name' => $to_name,
        'branch_id' => $branch->branch_id,
        'object_type' => $to_type,
        'from_did' => $from_did,
        'extends_did' => 0,
      ));
    }
    else {
      db_insert('api_reference_storage')
        ->fields(array(
          'object_name' => $to_name,
          'branch_id' => $branch->branch_id,
          'object_type' => $to_type,
          'from_did' => $from_did,
          'extends_did' => 0,
        ))
        ->execute();
    }
  }
}

/**
 * Adds a reference to the {api_namespace} table.
 *
 * @param int $file_did
 *   Documentation ID of the file for this namespace.
 * @param string $type
 *   Type of reference ('namespace' or 'use_alias').
 * @param string $alias
 *   Alias for the class, if $type is 'use_alias'.
 * @param string $class
 *   Full class name for use aliases; namespace name for namespaces.
 * @param bool $wait
 *   TRUE to save the insert until _api_namespace(TRUE) is called, or FALSE
 *   (default) to do the query immediately.
 */
function _api_namespace($file_did, $type = '', $alias = '', $class = '', $wait = FALSE) {
  static $query_stored = NULL;

  // Is it time to do all the saved queries?
  if ($file_did === TRUE) {
    if (!is_null($query_stored)) {
      $query_stored->execute();
      $query_stored = NULL;
    }
    return;
  }

  if ($wait) {
    if (is_null($query_stored)) {
      $query_stored = db_insert('api_namespace')
        ->fields(array('did', 'object_type', 'class_alias', 'class_name'));
    }
    $query_stored->values(array(
      'did' => $file_did,
      'object_type' => $type,
      'class_alias' => $alias,
      'class_name' => $class,
    ));
  }
  else {
    db_insert('api_namespace')
      ->fields(array(
        'did' => $file_did,
        'object_type' => $type,
        'class_alias' => $alias,
        'class_name' => $class,
      ))
      ->execute();
  }
}

/**
 * Registers a shutdown function for cron, making sure to do it just once.
 *
 * @see api_shutdown()
 */
function _api_schedule_shutdown() {
  $scheduled = &drupal_static(__FUNCTION__, FALSE);

  if (!$scheduled) {
    drupal_register_shutdown_function('api_shutdown');
    $scheduled = TRUE;
  }
}

/**
 * Marks a class as "touched" for shutdown, and/or returns the touched list.
 *
 * @param int $class_did
 *   (optional) Documentation ID of the class that has been touched.
 *
 * @return int[]
 *   Array of the touched class IDs.
 */
function _api_touched($class_did = 0) {
  $classes = &drupal_static(__FUNCTION__, array());

  if ($class_did) {
    $classes[$class_did] = $class_did;
  }

  return $classes;
}

/**
 * Keeps track of files that need to be requeued after this cron run.
 *
 * @param int $did
 *   (optional) Documentation ID of a documentation item whose file needs
 *   to be requeued after the run.
 *
 * @return array
 *   Array of all documentation IDs that need to be requeued.
 */
function _api_requeue_after_run($did = NULL) {
  static $dids = array();

  if ($did) {
    $dids[$did] = $did;
  }

  return $dids;
}

/**
 * Keeps track of which branches were updated during this run.
 *
 * @param int $branch_id
 *   (optional) ID of a branch that had some updates.
 *
 * @return int[]
 *   Array of the updated branches.
 */
function _api_branch_touched($branch_id = 0) {
  $branches = &drupal_static(__FUNCTION__, array());

  if ($branch_id) {
    $branches[$branch_id] = $branch_id;
  }

  return $branches;
}

/**
 * Cleans up at the end of the cron job.
 *
 * Figures out class parents, class member lists, class member overrides, and
 * class member references.
 */
function api_shutdown() {
  // Requeue items that were marked for requeue.
  module_load_include('inc', 'api', 'api.db');
  api_mark_items_for_reparse(_api_requeue_after_run());

  // Figure out which classes and interfaces we need to update.
  $classes_touched = _api_touched();

  // For each of these classes, and any that extend/implement them, we need
  // to recompute:
  // - The IDs for extended/implemented classes/interfaces.
  // - IDs for classes that extend/implement this class/interface.
  // - For traits, the traits and classes that use them.
  // - The {api_members} table, which figures out all class members including
  //   inherited members.
  // - The {api_overrides} table, which figures out for a class member if it
  //   is overriding another class's member, and where it is documented.
  // - The "computed-member" information in the {api_reference_storage} table.
  // So, the first step is to recompute class inheritance. This is stored by
  // name in {api_reference_storage}, and we need to find the actual IDs of
  // the classes. Do this one by one, since we need to compute some other
  // stuff anyway as we go.
  $classes_todo = $classes_touched;
  $classes_changed = $classes_touched;
  // Avoid infinite loops by keeping track of which classes we've already
  // checked.
  $classes_added = $classes_touched;
  // Keep track of the IDs of classes that extend/implement others, and the
  // members of each class.
  $class_parents = array();
  $class_members = array();
  while ($did = array_shift($classes_todo)) {
    $doc_object = _api_bare_object_load($did);
    if (!$doc_object) {
      continue;
    }

    // We're going to need the direct members of this class. Since methods,
    // properties, and constants can share names, do each separately.
    $result = db_select('api_documentation', 'd')
      ->fields('d', array('did', 'member_name', 'object_type'))
      ->condition('class_did', $did)
      ->condition('object_type', array('function', 'property', 'constant'))
      ->execute();
    $direct_members = array(
      'function' => array(),
      'property' => array(),
      'constant' => array(),
    );
    foreach ($result as $item) {
      $direct_members[$item->object_type][$item->member_name] = $item->did;
    }
    $class_members[$did] = $direct_members;

    // See if this class extends or inherits anything else, or uses traits,
    // and if so, calculate the IDs of the "parent" classes, traits, or
    // interfaces. Do this in the right order for inheritance: traits
    // trump class extends, and class extends trump interfaces.
    $parents = array();
    $parents_to_check = array();
    $types = array('trait', 'class', 'interface');
    foreach ($types as $type) {
      $results = db_select('api_reference_storage', 'ars')
        ->fields('ars')
        ->condition('ars.object_type', $type)
        ->condition('ars.from_did', $did)
        ->execute();
      if ($results) {
        foreach ($results as $result) {
          $parents_to_check[] = $result;
        }
      }
    }
    foreach ($parents_to_check as $parent) {
      // Figure out the ID of this parent.
      $new_id = _api_best_class_id($parent->object_name, $parent->branch_id);
      $parents[$parent->object_name] = $new_id;
      // See if it's different from what we already had.
      if ($new_id != $parent->extends_did) {
        // Update the reference storage record with this new ID.
        db_update('api_reference_storage')
          ->fields(array('extends_did' => $new_id))
          ->condition('from_did', $did)
          ->condition('object_type', $parent->object_type)
          ->condition('object_name', $parent->object_name)
          ->execute();
        // This class changed.
        if (!in_array($did, $classes_changed)) {
          $classes_changed[] = $did;
        }
      }

      // If this class's parent is already marked as "changed", then this
      // class needs to be marked as "changed" also.
      if (in_array($new_id, $classes_changed) && !in_array($did, $classes_changed)) {
        $classes_changed[] = $did;
      }

      // We also need to get the members of parent classes, so add it to the
      // to do list.
      if (($new_id) && !isset($classes_added[$new_id])) {
        $classes_todo[] = $new_id;
        $classes_added[$new_id] = $new_id;
      }
    }
    $class_parents[$did] = $parents;

    // If this is one of the "changed" classes, see if anything else inherits
    // from this class. If so, we need to check it too. This could be a class
    // that has already computed this as its extends_did, or a class in the
    // same core compatibility that extends a class by this namespaced name
    // but maybe didn't know the DID yet. Don't worry about the possible
    // edge case of a class that had found another one previously and this one
    // would now be better.
    if (in_array($did, $classes_changed)) {
      $or = db_or()
        ->condition('extends_did', $did)
        ->condition('object_name', $doc_object->namespaced_name);
      $query = db_select('api_reference_storage', 'ars')
        ->fields('ars', array('from_did'))
        ->condition('object_type', array('class', 'interface', 'trait'))
        ->condition($or);
      $query->innerJoin('api_branch', 'b', 'b.branch_id = ars.branch_id');
      $query->condition('b.core_compatibility', $doc_object->core_compatibility);
      $results = $query->execute()->fetchCol();

      foreach ($results as $from_did) {
        if (!is_numeric($from_did)) {
          continue;
        }
        $from_did = (int) $from_did;
        if (!isset($classes_added[$from_did])) {
          $classes_todo[] = $from_did;
          $classes_added[$from_did] = $from_did;
          if (!in_array($from_did, $classes_changed)) {
            $classes_changed[] = $from_did;
          }
        }
      }
    }
  }

  // OK, at this point we have a list of all the classes that we need to
  // update member, override, and class parent information for. And we have
  // in hand a list of the parent IDs and the direct members for each one.
  // So go through this list and redo the members and overrides tables.
  foreach ($classes_changed as $did) {
    _api_update_class_reference_info($did, $class_parents, $class_members);
  }

  // Now that all the class members have been updated, calculate
  // computed-member references for member-parent calls in the references
  // table. These are cases where ChildClass::foo() calls parent::bar(), and we
  // need to figure out the full name of the parent member. Do this for
  // all class methods in $classes_changed.
  foreach ($classes_changed as $did) {
    if (isset($class_members[$did]) && count($class_members[$did]['function'])) {
      $direct_methods = array_values($class_members[$did]['function']);
      $select = db_select('api_reference_storage', 'r')
        ->condition('r.object_type', 'member-parent')
        ->condition('r.from_did', $direct_methods);
      // This joins to the documentation record of the calling method.
      $select->innerJoin('api_documentation', 'cd', 'r.from_did = cd.did');
      // This finds the parent (extends) class record.
      $select->innerJoin('api_reference_storage', 'e', 'e.from_did = cd.class_did');
      $select
        ->condition('e.object_type', 'class')
        ->condition('e.extends_did', 0, '>');
      // This finds the members of the parent class.
      $select->innerJoin('api_members', 'pm', 'pm.class_did = e.extends_did');
      // We're looking for a method whose name is r.object_name. It is either:
      // - pm.member_alias (if it came from a trait with an alias)
      // - pm.member_alias is NULL and it's the member name in the member's
      //   documentation record.
      $select->innerJoin('api_documentation', 'dm', 'pm.did = dm.did');
      $and = db_and()
        ->where('dm.member_name = r.object_name')
        ->isNull('pm.member_alias');
      $and2 = db_and()
        ->where('pm.member_alias = r.object_name')
        ->isNotNull('pm.member_alias');
      $or = db_or()
        ->condition($and)
        ->condition($and2);
      $select->condition($or);
      $select->condition('dm.object_type', 'function');

      // Now make up the needed fields for the api_reference_storage records,
      // and insert them into the table.
      $select->distinct();
      $select->addField('dm', 'namespaced_name', 'object_name');
      $select->addField('r', 'branch_id', 'branch_id');
      $select->addField('r', 'from_did', 'from_did');
      // Note: SelectQuery adds expressions to the query at the end of the field
      // list.
      $select->addExpression("'computed-member'", 'object_type');

      db_insert('api_reference_storage')
        ->fields(array('object_name', 'branch_id', 'from_did', 'object_type'))
        ->from($select)
        ->execute();
    }
  }

  // Recalculate reference counts for each touched branch.
  foreach (_api_branch_touched() as $id) {
    _api_calculate_reference_counts($id);
  }

  cache_clear_all();
}

/**
 * Calculates the current best guess as to the ID of a class.
 *
 * @param string $class_name
 *   The namespaced class name to find.
 * @param int $branch_id
 *   The branch ID this name was referenced in.
 *
 * @return int
 *   The documentation ID of a matching class in the current branch, the core
 *   branch for this core compatibility, or another branch in this core
 *   compatibility, or 0 if no match was found. The match has to be unique
 *   within the scope. If multiple matches are found, 0 will be returned.
 */
function _api_best_class_id($class_name, $branch_id) {
  // Check current branch.
  $found = db_select('api_documentation', 'ad')
    ->condition('namespaced_name', $class_name)
    ->condition('branch_id', $branch_id)
    ->fields('ad', array('did'))
    ->execute()
    ->fetchCol();

  if (count($found) == 1) {
    return (int) array_shift($found);
  }
  elseif (count($found) > 1) {
    return 0;
  }

  // Check core branch.
  $branch = api_get_branch_by_id($branch_id);
  $core = api_find_core_branch($branch);
  if ($core) {
    $found = db_select('api_documentation', 'ad')
      ->condition('namespaced_name', $class_name)
      ->condition('branch_id', $core->branch_id)
      ->fields('ad', array('did'))
      ->execute()
      ->fetchCol();
    if (count($found) == 1) {
      return (int) array_shift($found);
    }
    elseif (count($found) > 1) {
      return 0;
    }
  }

  // See if there is a branch with matching core compatibility at least.
  if ($branch) {
    $query = db_select('api_documentation', 'ad');
    $query->innerJoin('api_branch', 'b', 'ad.branch_id = b.branch_id');
    $found = $query
      ->condition('ad.namespaced_name', $class_name)
      ->condition('b.core_compatibility', $branch->core_compatibility)
      ->fields('ad', array('did'))
      ->execute()
      ->fetchCol();
    if (count($found) == 1) {
      return (int) array_shift($found);
    }
  }

  // Didn't find a unique match.
  return 0;
}

/**
 * Updates all of the class reference information for a class.
 *
 * @param int $did
 *   Documentation ID for the class.
 * @param array $parent_info
 *   Array of collected parent information, calculated in api_shutdown().
 * @param array $member_info
 *   Array of collected direct member information, calculated in api_shutdown().
 */
function _api_update_class_reference_info($did, array $parent_info, array $member_info) {
  if (!$did) {
    return;
  }

  // Calculate the full list of class members, including inherited, and figure
  // out what each one is overriding and where each one is documented.
  $members = _api_calc_class_members($did, $parent_info, $member_info);

  // Save the overrides info, and if we are getting documentation from an
  // inherited member, update the summary on the main object. This is only
  // done for the direct members of this class.
  $ao_query = db_insert('api_overrides')
    ->fields(array('did', 'overrides_did', 'documented_did'));
  $new_dids = array();
  foreach ($members as $list) {
    foreach ($list as $member) {
      if ($member['direct_member']) {
        $new_dids[] = $member['did'];
        if ($member['documented_did'] != $member['did']) {
          db_update('api_documentation')
            ->condition('did', $member['did'])
            ->fields(array('summary' => $member['summary']))
            ->execute();
        }
        unset($member['summary']);
        $ao_query->values($member);
      }
    }
  }
  if (count($new_dids)) {
    // Delete the old information and insert new.
    db_delete('api_overrides')
      ->condition('did', $new_dids)
      ->execute();
    $ao_query->execute();
  }

  // Save the member list.
  db_delete('api_members')
    ->condition('class_did', $did)
    ->execute();
  $query = db_insert('api_members')
    ->fields(array('class_did', 'did', 'member_alias'));
  $has_some = FALSE;
  foreach ($members as $list) {
    foreach ($list as $member) {
      if ($member['alias'] != $member['member_name']) {
        $member_alias = $member['alias'];
      }
      else {
        $member_alias = NULL;
      }
      $query->values(array(
        'class_did' => $did,
        'did' => $member['did'],
        'member_alias' => $member_alias,
      ));
      $has_some = TRUE;
    }
  }
  if ($has_some) {
    $query->execute();
  }

  // Update the calculated member reference storage for this class, first
  // deleting any existing computed-member entries. These are for when
  // you have self::foo() or parent::bar() calls inside a method. We will
  // delete all computed-member entries, and recalculate the self:: ones here;
  // the parent:: ones are recalculated in api_shutdown() because we do not
  // necessarily have the parent class member information in the database
  // at this point.
  if (isset($member_info[$did]) && count($member_info[$did]['function'])) {
    // Delete all previous computed-member entries for calls from direct
    // methods of this class.
    $direct_methods = array_values($member_info[$did]['function']);
    db_delete('api_reference_storage')
      ->condition('object_type', 'computed-member')
      ->condition('from_did', $direct_methods)
      ->execute();

    // Calculate computed-member entries for member-self references. We're
    // taking an entry that says something like ThisClass::foo() calls
    // self::bar(), and trying to calculate the fully namespaced name of
    // self::bar(), which might be ThisClass::bar() or SomeParentClass::bar().
    $select = db_select('api_reference_storage', 'r')
      ->condition('r.object_type', 'member-self')
      ->condition('r.from_did', $direct_methods);
    // This joins to the documentation record of the calling method.
    $select->innerJoin('api_documentation', 'd', 'r.from_did = d.did');
    // This links to the now-updated member list of the class the calling
    // method is in.
    $select->innerJoin('api_members', 'm', 'd.class_did = m.class_did');
    // We're looking for a method whose name is r.object_name. It is either:
    // - m.member_alias (if it came from a trait with an alias)
    // - m.member_alias is NULL and it's the member name in the member's
    //   documentation record.
    $select->innerJoin('api_documentation', 'dm', 'm.did = dm.did');
    $and = db_and()
      ->where('dm.member_name = r.object_name')
      ->isNull('m.member_alias');
    $or = db_or()
      ->condition($and)
      ->where('m.member_alias = r.object_name');
    $select->condition($or);
    $select->condition('dm.object_type', 'function');

    // Now make up the needed fields for the api_reference_storage records,
    // and insert them into the table.
    $select->distinct();
    $select->addField('dm', 'namespaced_name', 'object_name');
    $select->addField('r', 'branch_id', 'branch_id');
    $select->addField('r', 'from_did', 'from_did');
    // Note: SelectQuery adds expressions to the query at the end of the field
    // list.
    $select->addExpression("'computed-member'", 'object_type');
    db_insert('api_reference_storage')
      ->fields(array('object_name', 'branch_id', 'from_did', 'object_type'))
      ->from($select)
      ->execute();
  }

}

/**
 * Calculates class member information for a class, including inheritance.
 *
 * @param int $did
 *   Documentation ID of the class to calculate.
 * @param array $parent_info
 *   Array of collected parent information, calculated in api_shutdown().
 * @param array $member_info
 *   Array of collected direct member information, calculated in api_shutdown().
 */
function _api_calc_class_members($did, array $parent_info, array $member_info) {
  // We will most likely encounter a class multiple times while going up
  // the hierarchy, so cache results.
  $cache = &drupal_static(__FUNCTION__, array());

  $members = array(
    'function' => array(),
    'property' => array(),
    'constant' => array(),
  );

  if (!$did || !isset($member_info[$did])) {
    return $members;
  }

  // See if we already calculated members for this class, during another
  // class traversal.
  if (isset($cache[$did])) {
    return $cache[$did];
  }

  // Add this class's direct members to the list, and put it in the
  // cache for now, to avoid loops.
  $members = _api_make_bare_member_list($did, $member_info);
  $cache[$did] = $members;

  // Add in the parents.
  if (!isset($parent_info[$did])) {
    return $members;
  }
  // See if there are any aliases or insteadof statements for trait members.
  $aliases = _api_find_trait_aliases($did);
  $omit = _api_find_trait_precedence($did);

  foreach ($parent_info[$did] as $parent_name => $parent) {
    $parent_members = _api_calc_class_members($parent, $parent_info, $member_info);
    _api_member_list_merge($members, $parent_members, isset($aliases[$parent_name]) ? $aliases[$parent_name] : array(), isset($omit[$parent_name]) ? $omit[$parent_name] : array());
  }

  // Save in the cache and return.
  $cache[$did] = $members;
  return $members;
}

/**
 * Reads trait method aliases from the database.
 */
function _api_find_trait_aliases($did) {
  $results = db_select('api_namespace', 'an')
    ->condition('did', $did)
    ->condition('object_type', array('trait_alias'))
    ->fields('an', array('class_alias', 'class_name'))
    ->execute();

  $return = array();
  foreach ($results as $item) {
    $alias = $item->class_alias;
    $name = $item->class_name;
    $pos = strpos($name, '::');
    if ($pos >= 1) {
      $class = substr($name, 0, $pos);
      $member = substr($name, $pos + 2);
      $return[$class][$member] = $alias;
    }
  }

  return $return;
}

/**
 * Reads trait method precedence (insteadof) statements from the database.
 *
 * @return array
 *   Array whose keys are trait class names, and whose values are a list of
 *   methods that should be excluded from that trait when using it, because
 *   methods from another class is being used instead of them.
 */
function _api_find_trait_precedence($did) {
  $results = db_select('api_namespace', 'an')
    ->condition('did', $did)
    ->condition('object_type', array('trait_precedence'))
    ->fields('an', array('class_alias', 'class_name'))
    ->execute();

  $omit = array();
  foreach ($results as $item) {
    $class = $item->class_alias;
    $method = $item->class_name;
    $omit[$class][] = $method;
  }

  return $omit;
}

/**
 * Calculates a bare list of this class's direct members.
 */
function _api_make_bare_member_list($did, $member_info) {
  // We will most likely encounter a class multiple times while going up
  // the hierarchy, so cache results.
  $cache = &drupal_static(__FUNCTION__, array());

  if (isset($cache[$did])) {
    return $cache[$did];
  }

  $members = array(
    'function' => array(),
    'property' => array(),
    'constant' => array(),
  );

  foreach ($member_info[$did] as $type => $list) {
    foreach ($list as $member_did) {
      $item = _api_bare_object_load($member_did, $type);
      if (!$item) {
        continue;
      }
      $has_docs = _api_has_documentation($item);
      $member_name = $item->member_name;
      $members[$type][$member_name] = array(
        'did' => $item->did,
        'overrides_did' => 0,
        'documented_did' => ($has_docs) ? $item->did : 0,
        'summary' => $item->summary,
        'alias' => $member_name,
        'member_name' => $member_name,
        'direct_member' => TRUE,
      );
    }
  }

  $cache[$did] = $members;
  return $members;
}

/**
 * Merges a member list with parent member list.
 *
 * @param array $members
 *   List of member information, modified by reference.
 * @param array $parent_members
 *   List of parent member information to merge in.
 * @param array $aliases
 *   List of aliases for member names (functions only).
 * @param array $omit
 *   List of methods in parent class to omit due to insteadof statements.
 */
function _api_member_list_merge(array &$members, array $parent_members, array $aliases, array $omit) {
  foreach ($parent_members as $type => $new_type_list) {
    foreach ($new_type_list as $member_name => $info) {
      if (in_array($member_name, $omit)) {
        continue;
      }
      $alias = (isset($aliases[$member_name]) && $type == 'function') ? $aliases[$member_name] : $member_name;
      if (isset($members[$type][$alias])) {
        // We already knew about this member. Save override info, but only
        // for direct members.
        if ($members[$type][$alias]['direct_member']) {
          if (!$members[$type][$alias]['overrides_did']) {
            // We just found what the known member is overriding.
            $members[$type][$alias]['overrides_did'] = $info['did'];
          }
          if (!$members[$type][$alias]['documented_did']) {
            // The old member didn't have documentation, maybe this one does.
            $members[$type][$alias]['documented_did'] = $info['documented_did'];
          }
          if (!$members[$type][$alias]['summary']) {
            // The old member didn't have a summary, maybe this one does.
            $members[$type][$alias]['summary'] = $info['summary'];
          }
        }
      }
      else {
        // It's a new member inherited from a parent, add it in.
        $info['alias'] = $alias;
        $info['direct_member'] = FALSE;
        $members[$type][$alias] = $info;
      }
    }
  }
}

/**
 * Loads a bare documentation object, without overrides.
 *
 * @param int $did
 *   Documentation ID.
 * @param string|array $type
 *   Object type.
 *
 * @return object|null
 *   Documentation object, or NULL if not found.
 */
function _api_bare_object_load($did, $type = '') {
  $cache = &drupal_static(__FUNCTION__, array());

  if (!is_numeric($did)) {
    return NULL;
  }
  $did = (int) $did;
  if (!$did) {
    return NULL;
  }

  if (isset($cache[$did])) {
    return $cache[$did];
  }

  $query = db_select('api_documentation', 'ad');
  $query->fields('ad');
  $query->condition('ad.did', $did);
  if ($type == 'function') {
    $query->leftJoin('api_function', 'afunc', 'afunc.did = ad.did');
    $query->fields('afunc', array('signature', 'parameters', 'return_value'));
  }
  elseif ($type == 'file') {
    $query->leftJoin('api_file', 'afile', 'afile.did = ad.did');
    $query->fields('afile', array('modified', 'queued'));
  }
  $query->leftJoin('api_branch', 'b', 'ad.branch_id = b.branch_id');
  $query->fields('b', array('project', 'core_compatibility'));

  $query->leftJoin('api_documentation', 'adfile', "adfile.file_name = ad.file_name AND adfile.object_type = 'file' AND adfile.branch_id = ad.branch_id");
  $query->addField('adfile', 'did', 'file_did');
  $query = $query->range(0, 1);
  $result = $query->execute()->fetchObject();

  $cache[$did] = $result;

  return $result;
}

/**
 * Determines whether an object contains any documentation.
 *
 * @param object $object
 *   An object to test to see if it contains documentation.
 *
 * @return bool
 *   TRUE if the object has any of the following non-empty properties:
 *   - documentation
 *   - parameters
 *   - return_value
 *   - see
 *   - throws
 *   - var
 */
function _api_has_documentation($object) {
  $members = array(
    'documentation',
    'parameters',
    'return_value',
    'see',
    'deprecated',
    'throws',
    'var',
  );
  foreach ($members as $member) {
    if (!empty($object->$member)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Calculates and stores aggregate reference counts for a branch.
 *
 * @param int $branch_id
 *   ID of the branch to calculate counts for.
 *
 * @see api_find_references()
 */
function _api_calculate_reference_counts($branch_id) {
  // Remove existing reference counts for this branch.
  db_delete('api_reference_counts')
    ->condition('branch_id', $branch_id)
    ->execute();

  // Calculate use counts for classes, with fully-qualified namespaced class
  // name. The entries in {api_namespace} have the namespace declarations,
  // which are missing the initial \ character.
  $select = db_select('api_namespace', 'an');
  $select->leftJoin('api_documentation', 'ad', 'an.did = ad.did');
  $select->addExpression("CONCAT('\\\\', an.class_name)", 'object_name');
  $select->addExpression($branch_id, 'branch_id');
  $select->addExpression("'use'", 'reference_type');
  $select->addExpression('COUNT(*)', 'reference_count');
  $select
    ->condition('an.object_type', 'use_alias')
    ->condition('ad.branch_id', $branch_id)
    ->groupBy('an.class_name');
  db_insert('api_reference_counts')
    ->from($select)
    ->execute();

  // Calculate call counts for functions and methods, including usage of
  // constants, and extend/implements for classes/interfaces. For simple
  // functions, these will not have namespaces on them. For class members and
  // extend/implements of classes, they will have namespaces.
  $select = db_select('api_reference_storage', 'ars');
  $select->addField('ars', 'object_name', 'object_name');
  $select->addExpression($branch_id, 'branch_id');
  $select->addExpression("'call'", 'reference_type');
  $select->addExpression('COUNT(*)', 'reference_count');
  $select
    ->condition('ars.object_type', array(
      'function',
      'constant',
      'member-class',
      'computed-member',
      'class',
      'interface',
    ))
    ->condition('ars.branch_id', $branch_id)
    ->groupBy('ars.object_name');
  db_insert('api_reference_counts')
    ->from($select)
    ->execute();

  // Calculate string reference counts. Some strings have namespaces, and some
  // don't, similar to the 'call' reference count above.
  $select = db_select('api_reference_storage', 'ars');
  $select->addField('ars', 'object_name', 'object_name');
  $select->addExpression($branch_id, 'branch_id');
  $select->addExpression("'string'", 'reference_type');
  $select->addExpression('COUNT(*)', 'reference_count');
  $select
    ->condition('ars.object_type', array(
      'potential callback',
      'potential file',
      'service_class',
      'yaml string',
    ))
    ->condition('ars.branch_id', $branch_id)
    ->groupBy('ars.object_name');
  db_insert('api_reference_counts')
    ->from($select)
    ->execute();

  // Calculate override counts for methods and other class members. For
  // disambiguation, store the namespaced name of the class::member.
  $select = db_select('api_overrides', 'ao');
  $select->leftJoin('api_documentation', 'ad', 'ao.did = ad.did');
  $select->leftJoin('api_documentation', 'ado', 'ao.overrides_did = ado.did');
  // Object name is the name of the overridden method.
  $select->addExpression('ado.namespaced_name', 'object_name');
  $select->addExpression($branch_id, 'branch_id');
  $select->addExpression("'override'", 'reference_type');
  $select->addExpression('COUNT(*)', 'reference_count');
  $select
    // Some records in api_overrides are just about where the documentation is.
    // Filter these out.
    ->condition('ao.overrides_did', 0, '<>')
    // Some overrides may be in other branches. Filter these out too.
    ->condition('ad.branch_id', $branch_id)
    ->condition('ado.branch_id', $branch_id)
    ->groupBy('ado.object_name');
  db_insert('api_reference_counts')
    ->from($select)
    ->execute();
}
