<?php
/**
 * Template file
 * variables:
 * $node -> the current node object
 */ 
?>
<?php
print '"';
print filter_xss($node->title);
print '"';
?>
