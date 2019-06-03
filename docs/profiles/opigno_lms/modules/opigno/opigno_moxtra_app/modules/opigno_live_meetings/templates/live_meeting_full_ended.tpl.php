<?php
  $mox_binder_id = $node->schedule_binder_id[LANGUAGE_NONE][0]['value'];
  $files = opigno_live_meetings_api_get_files_list($node->uid, $mox_binder_id);
  $recordings = opigno_live_meetings_api_get_recording_info($node->uid, $mox_binder_id);
  $access_token = opigno_moxtra_app_api_opigno_get_access_token($node->uid);
?>
<?php echo theme('live_meeting_teaser', compact('node', 'live_meeting_info', 'view_mode')); ?>

<?php if($files->data->files): ?>
  <div class="field">
    <div class="field-label"><?php print t('Files'); ?>:&nbsp;</div>
    <div class="field-items">
    <?php foreach($files->data->files as $file): ?>
      <div class="field-item">
        <a title="<?php print $file->name; ?>" href="<?php print opigno_live_meetings_api_get_file_info($node->uid, $mox_binder_id, $file->file_id)->data->url; ?>"><?php print $file->name?: t('Unnamed'); ?></a>
      </div>
    <?php endforeach; ?>
    </div>
  </div>
<?php endif; ?>

<?php if ($recordings->data->recordings): ?>
  <div class="field">
    <div class="field-label"><?php print t('Recordings'); ?>:&nbsp;</div>
    <div class="field-items">
      <?php foreach($recordings->data->recordings as $recording): ?>
        <?php $down_get_separator = (empty(parse_url($recording->download_url)['query']) ? '?' : '&') ?>
        <?php $play_get_separator = (empty(parse_url($recording->playback_url)['query']) ? '?' : '&') ?>

        <div class="field-item">
          <a title="Download" href="<?php print $recording->download_url . $down_get_separator .'access_token='. $access_token; ?>"><?php print t('Download'); ?></a> / <a title="Playback" href="<?php print $recording->playback_url . $play_get_separator .'access_token='. $access_token; ?>"><?php print t('Playback'); ?></a>
        </div>
      <?php endforeach; ?>
    </div>
  </div>
<?php endif; ?>
