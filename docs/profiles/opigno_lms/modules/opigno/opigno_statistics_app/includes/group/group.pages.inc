<?php

require_once(__DIR__ . '/course/course.pages.inc');
require_once(__DIR__ . '/class/class.pages.inc');

/*
 * Implements the title callback for menu entry node/%/opigno-statistics
 * @see opigno_statistics_app_menu
 */
function opigno_statistics_app_group_statistics_page_title_callback($nid) {
  $node_title = db_query('SELECT group_title FROM {opigno_statistics_group} WHERE group_nid = :nid LIMIT 1', array(':nid' => $nid))->fetchCol();
  $node_title=$node_title[0];
  return $node_title . ' - ' . t("Statistics");
}

/*
 * Render group statistic page
 * Switch display based on node type
 *
 * @param $nid
 */
function opigno_statistics_app_group_statistics_page($nid){
  $node_type = db_query('SELECT group_type FROM {opigno_statistics_group} WHERE group_nid = :nid LIMIT 1', array(':nid' => $nid))->fetchCol();
  $node_type=$node_type[0];
  switch($node_type) {
    case 'course': return opigno_statistics_app_course_statistics_page($nid);
    case 'class' : return opigno_statistics_app_class_statistics_page($nid);
    default: return MENU_NOT_FOUND;
  }
}
