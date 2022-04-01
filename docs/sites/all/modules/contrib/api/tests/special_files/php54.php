<?php

/**
 * @file
 * Tests some PHP 5.4 features.
 */

// Testing that this syntax is parsed and displayed correctly.
require_once __DIR__ . '/something.php';

/**
 * Tests that PHP 5.4 array syntax can be parsed and displayed.
 */
function array_5_4() {
  // New array syntax.
  $foo = [
    'href' => 'bar',
    [
      [
        'nested' => 'valid',
      ],
    ],
  ];

  // Use of [] for array indexing.
  $foo = $my_array[$i + 7]['bar'][some_function_call('foo', 'bar', 'baz')][3];

  // Mixture of old and new array syntax.
  $foo = array(
    'bar' => 'baz',
    'boohoo',
    [
      'mixing' => 'bad',
      'but' => 'legal',
    ],
    array(
      1,
      3,
      5,
    ),
  );

  // Empty array.
  $boo = [];
}

/**
 * A function with a bunch of different code in it, to test formatting.
 *
 * And let's make sure that the <em>comment</em> has some HTML in it.
 */
function formatting_test() {
  // Switch statement. Comment has <em>HTML tags</em> in it.
  switch ($path) {
    case 'admin/help#api':
      return t('
<p>This is an implementation of a subset of the Doxygen documentation generator specification, tuned to produce output that best benefits the Drupal code base. It is designed to assume the code it documents follows Drupal coding conventions, and supports documentation blocks in formats described on !doxygen_link.</p>
<p>Here is another paragraph.</p>');
      break;

    case 'something_else':
      return $foo;
      break;

    default:
      return 'bar';
      break;
  }

  // Casting and some operators.
  $foo = (int) (7 + 3 * ($x + $y));
  $foo++;
  $foo--;
  --$foo;
  $foo += 44;

  // If/else statement.
  if ($foo) {
    // We need a comment here.
    $bar = '3';
  }
  elseif ($foo == 'bar') {
    $bar = '4';
  }
  else {
    $bar = '5';
  }

  // Numeric for loop.
  for ($i = 0; $i < 10; $i++) {
    print "hello " . $i;
    if ($foo) {
      continue;
    }
  }

  // Strings with <<< syntax.
  $foo = <<<EOO
Here is some text with <strong>HTML tags in it</strong>
for this string.
EOO;

  $bar = <<<'EPP'
Here is some text with <strong>HTML tags in it</strong>
for this string.
EPP;

  $baz = <<<'EOT'
 My name is "$name". I am printing some $foo->foo.
Now, I am printing some {$foo->bar[1]}.
This should not print a capital 'A': \x41
EOT;

}
