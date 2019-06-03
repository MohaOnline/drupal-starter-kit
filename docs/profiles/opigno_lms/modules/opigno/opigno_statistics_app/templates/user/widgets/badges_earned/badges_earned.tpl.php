<?php
/**
 * @file
 * Opigno statistics app - User - badges earned template file
 *
 * @param array $badges_earned
 */
?>

<div class="opigno-statistics-app-widget" id="opigno-statistics-app-user-widget-badges-earned">
  <h2><?php print t('Badges earned'); ?></h2>
  <?php if(count($badges_earned) > 0): ?>
    <?php print theme('opigno_statistics_app_user_widget_badges_earned_list', compact('badges_earned')); ?>
  <?php else: ?>
    <p><?php print t('No badge earned'); ?></p>
  <?php endif ?>
</div>
