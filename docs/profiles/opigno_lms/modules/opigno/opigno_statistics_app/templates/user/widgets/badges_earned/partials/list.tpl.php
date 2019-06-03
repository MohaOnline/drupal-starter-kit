<?php
/**
 * @file
 * Opigno statistics app - User - Badges earned list template file
 *
 * @param array $badges_earned
 */
?>
<div>
    <?php foreach($badges_earned as $badge_earned) print theme('opigno_statistics_app_user_widget_badges_earned_list_item', compact('badge_earned')); ?>
</div>