<?php
  global $base_path;
  $opigno_statistics_ml = menu_get_item('admin/opigno-statistics/opigno-statistics-app');
  $opigno_statistics_dashboard_ml = menu_get_item('admin/opigno-statistics/opigno-statistics-app/dashboard');

  $lls_ml = menu_get_item('admin/opigno-statistics/learning-locker-statistics');
  $lls_dashboard_ml = menu_get_item('admin/opigno-statistics/learning-locker-statistics/dashboard');
  $lls_course_content_ml = menu_get_item('admin/opigno-statistics/learning-locker-statistics/course-content');
  $lls_quizzes_ml = menu_get_item('admin/opigno-statistics/learning-locker-statistics/quizzes');
?>
<div class="admin clearfix">
  <div class="left clearfix">
    <div class="admin-panel">
      <h3><?php print $opigno_statistics_ml['title']; ?></h3>
      <div class="body">
        <dl class="admin-list"><dt><a href="<?php print $base_path.$opigno_statistics_dashboard_ml['href'];?>"><?php print $opigno_statistics_dashboard_ml['title']; ?></a></dt>
          <dd ><?php print $opigno_statistics_dashboard_ml['description']; ?></dd>
        </dl>
      </div>
    </div>
  </div>
  <div class="right clearfix">
    <div class="admin-panel">
      <h3><?php print $lls_ml['title']; ?></h3>
      <div class="body">
        <dl class="admin-list"><dt ><a href="<?php print $base_path.$lls_dashboard_ml['href'];?>"><?php print $lls_dashboard_ml['title']; ?></a></dt>
          <dd><?php print $lls_dashboard_ml['description']; ?></dd><dt ><a href="<?php print $base_path.$lls_course_content_ml['href'];?>"><?php print $lls_course_content_ml['title']; ?></a></dt>
          <dd><?php print $lls_course_content_ml['description']; ?></dd><dt ><a href="<?php print $base_path.$lls_quizzes_ml['href'];?>"><?php print $lls_quizzes_ml['title']; ?></a></dt>
          <dd><?php print $lls_quizzes_ml['description']; ?></dd>
        </dl>
      </div>
    </div>
  </div>
</div>