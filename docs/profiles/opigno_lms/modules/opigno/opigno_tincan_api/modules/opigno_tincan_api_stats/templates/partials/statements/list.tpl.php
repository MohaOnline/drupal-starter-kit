<table>
  <tr>
    <th><?php print t('Actor'); ?></th>
    <th><?php print t('Verb'); ?></th>
    <th><?php print t('Object'); ?></th>
    <th><?php print t('Date'); ?></th>
  </tr>
  <?php foreach($statements as $statement) print theme('opigno_lrs_stats_statements_list_item', compact('statement')); ?>
</table>