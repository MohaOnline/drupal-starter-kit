<?php

$literal_title = $node->title;


$path_view_node = 'node/' . $node->nid;
$html_view_node_link = '';
if ($view_mode != 'full')
  $html_view_node_link = l(t('Access'), $path_view_node, array(
    'attributes' => array(
      'class' => array(
        'read-more',
        'action-element',
        'action-sort-element'
      ),
      'onclick' => 'event.preventDefault(); openCollaborativeWorkspace("'. htmlentities($node->field_collaborative_workspace_id[LANGUAGE_NONE][0]['value']) .'", "'. htmlentities($node->title) .'");'
    )
  ));


$path_edit_node = 'node/'. $node->nid .'/edit';
$html_edit_node_link = l(t('Edit'), $path_edit_node, array(
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
    <th colspan="2"><?php echo $literal_title; ?></th>
  </tr>
  </thead>
  <tbody>

  <tr>
    <td class="take-button-cell">
      <?php if (drupal_valid_path($path_view_node)) echo $html_view_node_link; ?>
    </td>

    <?php if (drupal_valid_path($path_edit_node)) { ?>
    <td class="take-button-cell">
      <?php echo $html_edit_node_link; ?>
    </td>
    <?php } ?>

  </tr>

  </tbody>
</table>
