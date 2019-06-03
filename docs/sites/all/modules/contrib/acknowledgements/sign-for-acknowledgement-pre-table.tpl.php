<?php
/**
 * Template file
 * variables:
 * $node -> the current node object
 * $expire -> the expiration date  
 */ 
?>
<?php print($expire? t('To be signed within: ') . $expire : ''); ?>