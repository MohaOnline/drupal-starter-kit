<?php
/**
 * @file
 * Opigno statistics app - Class - General statistics template file
 *
 * @param array $general_statistics
 *  $general_statistics['course_progress_percentage']
 *  $general_statistics['quizz_completion_percentage']
 */
$color_pallete=color_get_palette('platon');
?>
<div class="opigno-statistics-app-widget col col-1-out-of-4 clearfix" id="opigno-statistics-app-class-widget-general-statistics">
  <h2><?php print t('General statistics'); ?></h2>
  <div class="opigno-statistics-app-class-widget-course-progress-percentage">
    <h3><?php print t('Course progress'); ?></h3>
    <div class="circle" id="opigno-statistics-app-class-widget-course-progress-percentage-circle"></div>
  </div>
  <div class="opigno-statistics-app-class-widget-quizz-completion-percentage">
    <h3><?php print t('Quizzes completed'); ?></h3>
    <div class="circle" id="opigno-statistics-app-class-widget-quizz-completion-percentage-circle"></div>
  </div>
  <script type="text/javascript">
    Circles.create({
      id:                  'opigno-statistics-app-class-widget-course-progress-percentage-circle',
      radius:              60,
      value:               <?php print $general_statistics['course_progress_percentage']; ?>,
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

    Circles.create({
      id:                  'opigno-statistics-app-class-widget-quizz-completion-percentage-circle',
      radius:              60,
      value:               <?php print $general_statistics['quizz_completion_percentage']; ?>,
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