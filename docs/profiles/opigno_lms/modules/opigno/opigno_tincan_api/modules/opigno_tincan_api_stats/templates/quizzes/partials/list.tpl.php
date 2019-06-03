<table id="lrs-stats-quizzes-table">
  <thead>
    <tr>
      <th><?php print t('Names'); ?></th>
      <th class="center"><?php print t('Number of views'); ?></th>
      <th class="center"><?php print t('Number of users'); ?></th>
      <th class="center"><?php print t('Number of attempts'); ?></th>
      <th class="center"><?php print t('Average scores'); ?></th>
    </tr>
  </thead>
  <tbody>
    <?php foreach($quizzes as $quiz_id => $quiz) print theme('opigno_lrs_stats_quizzes_list_item', compact('quiz_id', 'quiz')); ?>
  </tbody>
</table>