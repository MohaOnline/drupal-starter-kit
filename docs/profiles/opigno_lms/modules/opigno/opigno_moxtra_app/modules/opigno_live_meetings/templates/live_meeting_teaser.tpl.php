<?php

$literal_title = $node->title;

$literal_status = '';
switch($live_meeting_info->data->status) {
  case 'SESSION_STARTED':
    $literal_status = t('Started');
    break;

  case 'SESSION_SCHEDULED':
    $literal_status = t('Scheduled');
    break;

  case 'SESSION_ENDED':
    $literal_status = t('Ended');
    break;
}

$literal_score_saved = '';
$path_register_score = 'node/' . $node->nid . '/live-meeting-score';
$html_register_score_link = '';
if ($live_meeting_info->data->status == 'SESSION_ENDED')
{
  $html_register_score_link = l(t('Register presences'), $path_register_score, array(
    'attributes' => array(
      'class' => array(
        'results',
        'action-element',
        'action-results-element'
      )
    )
  ));

  $literal_score_saved = opigno_live_meetings_score_is_registered_db($node->nid) ? t('Presences are already registered') : t('Presences not yet registered');
}

$path_show_meeting = 'node/' . $node->nid;
$html_show_meeting_link = '';
if ($view_mode != 'full')
  $html_show_meeting_link = l(t('Access this live meeting'), $path_show_meeting, array(
    'attributes' => array(
      'class' => array(
        'read-more',
        'action-element',
        'action-sort-element'
      )
    )
  ));

$path_edit_meeting = 'node/'. $node->nid .'/edit';
$html_edit_meeting_link = l(t('Edit this live meeting'), $path_edit_meeting, array(
  'attributes' => array(
    'class' => array(
      'edit',
      'action-element',
      'action-edit-element'
    )
  )
));

$literal_start_date = format_date(strtotime($node->opigno_calendar_date[LANGUAGE_NONE][0]['value']), 'long');
$literal_end_date = format_date(strtotime($node->opigno_calendar_date[LANGUAGE_NONE][0]['value2']), 'long');

$html_participants_list = '';
if(count($participants = $live_meeting_info->data->participants) > 0) {
  // Unify each participant (because they can appear several times in the participant list from Moxtra)
  $already_shown_ids = array();
  foreach($participants as $participant)
    if (!in_array($participant->unique_id, $already_shown_ids)) {
      $html_participants_list .= $participant->name .'<br />';
      $already_shown_ids[] = $participant->unique_id;
    }
}

?>

<table>
  <thead>
  <tr>
    <th colspan="3"><?php echo $literal_title; ?></th>
  </tr>
  </thead>
  <tbody>

  <tr>
    <td><?php echo t('Status'); ?></td>
    <td><?php echo $literal_status; ?></td>
    <td rowspan="5" class="take-button-cell">
      <?php if (drupal_valid_path($path_show_meeting)) echo $html_show_meeting_link; ?>
      <?php if (drupal_valid_path($path_register_score)) echo $html_register_score_link; ?>
      <?php if (drupal_valid_path($path_edit_meeting)) echo $html_edit_meeting_link; ?>
    </td>
  </tr>

  <?php if (drupal_valid_path($path_register_score) && !empty($literal_score_saved)) { ?>
    <tr>
      <td><?php echo t('Presences'); ?></td>
      <td><?php echo $literal_score_saved; ?></td>
    </tr>
  <?php } ?>

  <tr>
    <td><?php echo t('Start date'); ?></td>
    <td><?php echo $literal_start_date; ?></td>
  </tr>

  <tr>
    <td><?php echo t('End date'); ?></td>
    <td><?php echo $literal_end_date; ?></td>
  </tr>

  <tr>
    <td><?php echo t('Participants'); ?></td>
    <td><?php echo $html_participants_list; ?></td>
  </tr>

  </tbody>
</table>
