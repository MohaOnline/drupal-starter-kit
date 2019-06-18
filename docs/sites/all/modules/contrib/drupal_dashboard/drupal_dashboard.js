/**
 * @file
 * Js file for the js functionality.
 */

function drupal_dashboard_click_toggle(id) {
  "use strict";
  jQuery("#" + id + " tbody").toggle();
}
