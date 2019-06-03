<?php
/**
 * @file
 * Opigno statistics app - course - Total number of page view template file
 *
 * @param $total_number_of_page_view
 *  $total_number_of_page_view['graph_config']
 */
?>
<div class="opigno-statistics-app-widget col col-3-out-of-4 clearfix" id="opigno-statistics-app-course-widget-number-of-interactions">
  <h2><?php print t('Number of interactions') . ' / ' . t('Score'); ?></h2>
  <div id="opigno-statistics-app-course-widget-number-of-interactions-chart" style="height: 250px;"></div>
  <script type="text/javascript">
    (function(){
      var graph_config = <?php print json_encode($number_of_interactions['graph_config']); ?>;
      graph_config['hoverCallback'] = function(index, options, content) {
        var data = options.data[index];
        return '<b>' + data.username + '</b>' +
          '<br/><?php print str_replace('\'','\\\'',t('Number of interactions')); ?>: ' + data.number_of_interactions +
          '<br/><?php print t('Score') ?>: ' + data.score;
      };
      new Morris.Line(graph_config);
    })();
  </script>
</div>