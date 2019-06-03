<?php
/**
 * @file
 * Opigno learning locker stats - Dashboard - Active users last week template file
 *
 * @param array $active_users_last_week
 *  $active_users_last_week['percentage']
 */
$color_pallete=color_get_palette('platon');
?>
<div class="opigno-statistics-app-widget clearfix" id="opigno-statistics-app-widget-dashboard-general-statistics">
  <h2><?php print t('Active users last week'); ?></h2>
  <div id="opigno-statistics-app-widget-dashboard-general-statistics-percentage-circle"></div>
  <script type="text/javascript">
    Circles.create({
      id:                  'opigno-statistics-app-widget-dashboard-general-statistics-percentage-circle',
      radius:              60,
      value:               <?php print $active_users_last_week['percentage']; ?>,
      maxValue:            100,
      width:               10,
      text:                function(value){return value + '%';},
      colors:              ['<?php print $color_pallete['light_blue']; ?>', '<?php print $color_pallete['dark_blue']; ?>'],
      duration:            400,
      wrpClass:            'circles-wrp',
      textClass:           'circles-text',
      valueStrokeClass:    'circles-valueStroke',
      maxValueStrokeClass: 'circles-maxValueStroke',
      styleWrapper:        true,
      styleText:           true
    });
  </script>
</div>