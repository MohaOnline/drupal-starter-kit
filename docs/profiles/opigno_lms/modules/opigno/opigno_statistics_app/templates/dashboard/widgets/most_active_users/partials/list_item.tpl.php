<?php
/**
 * @file
 * Opigno statistics app - Dashboard - Most active users list item template file
 *
 * @param array $active_user
 *  $active_user['gid'];
 *  $active_user['last_visit']
 */
  $user = user_load($active_user['uid']);
  global $base_path;
  $default_image_path = $base_path . drupal_get_path('module', 'opigno_statistics_app') . '/img/default_user_picture.jpg';
?>
<a href="<?php print url("user/{$active_user['uid']}/opigno-statistics"); ?>">
  <div class="opigno-statistics-app-widget-dashboard-most-active-users-list-item clearfix">
    <div class="pull-right">
      <?php print ((isset($user->picture) && !empty($user->picture))? theme('image', array('path' => image_style_url('thumbnail', $user->picture->uri))) : "<img src=\"{$default_image_path}\"/>"); ?>
    </div>
    <b><?php print $active_user['user_name']; ?></b>
    <p><?php print t('Last visit') ?>:<br/> <?php print DateTime::createFromFormat('U',$active_user['last_visit'])->format('Y-m-d'); ?></p>
  </div>
</a>