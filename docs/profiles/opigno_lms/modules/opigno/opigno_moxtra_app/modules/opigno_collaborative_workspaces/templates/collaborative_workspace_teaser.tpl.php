<?php

$literal_title = $node->title;


$html_participant_list = '';
foreach ($node->field_users_invited[LANGUAGE_NONE] as $participant) {
  $list_user = user_load($participant['target_id']);
  $html_participant_list .= $list_user->name .'<br />';
}


$path_view_node = 'node/' . $node->nid;
$html_view_node_link = '';
if ($view_mode != 'full')
  $html_view_node_link = l(t('Access this collaborative workspace'), $path_view_node, array(
    'attributes' => array(
      'class' => array(
        'read-more',
        'action-element',
        'action-sort-element'
      )
    )
  ));


$path_edit_node = 'node/'. $node->nid .'/edit';
$html_edit_node_link = l(t('Edit this collaborative workspace'), $path_edit_node, array(
  'attributes' => array(
    'class' => array(
      'edit',
      'action-element',
      'action-edit-element'
    )
  )
));

?>


<table>
  <thead>
  <tr>
    <th colspan="3"><?php echo $literal_title; ?></th>
  </tr>
  </thead>
  <tbody>

  <tr>
    <td><?php echo t('Participants'); ?></td>
    <td><?php echo $html_participant_list; ?></td>
    <td class="take-button-cell">
      <?php if (drupal_valid_path($path_view_node)) echo $html_view_node_link; ?>
      <?php if (drupal_valid_path($path_edit_node)) echo $html_edit_node_link; ?>
    </td>
  </tr>

  </tbody>
</table>
