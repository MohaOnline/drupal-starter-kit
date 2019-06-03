<?php echo theme('moxtra_app_js_init'); ?>

<div id="live-meeting-container" style="width: 100%;"></div>
<h2 id="reloading" style="display: none"><?php print t('Reloading. Please, wait...'); ?></h2>
<h2 id="max_reached" style="display: none"><?php print t('The maximum number of users for this meeting is reached.'); ?></h2>
<h2 id="left" style="display: none;"><?php echo t('You left successfully'); ?></h2>

<script>
  var $ = jQuery;
  Moxtra.joinMeet({
    session_key: '<?php echo $live_meeting_info->data->session_key; ?>',
    iframe: true,
    video: true,
    tagid4iframe: 'live-meeting-container',
    iframewidth: '100%',
    error: function(event) {
      if (event.error_code == 412) { // Error code when the meeting has already ended
        $('div#live-meeting-container').hide();
        location.reload(true);
      }
    },
    end_meet: function(){
      $('h2#reloading').show();
      location.reload(true);
    },
    exit_meet: function() {
      $('h2#left').show();
    },
    reach_limit: function() {
      $('div#live-meeting-container').hide();
      $('h2#max_reached').show()
    }
  });
</script>
