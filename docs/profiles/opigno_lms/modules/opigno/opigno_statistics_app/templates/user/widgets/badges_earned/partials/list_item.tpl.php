<?php
/**
 * @file
 * Opigno statistics app - User - Badges earned list item template file
 *
 * @param array $badge_earned
 *  $badge_earned['title']
 *  $badge_earned['image']
 */
?>
<?php print theme('image', array('title' => $badge_earned['title'], 'path' => file_create_url($badge_earned['image']), 'width' => 50)); ?>