<?php

// If the user can start the meeting, show him the button to start. Else, show that the meet is not started.
if (og_user_access('node', $node->og_group_ref[LANGUAGE_NONE][0]['target_id'], 'start live_meeting'))
{
  echo theme('moxtra_app_js_init');

?>

  <div id="live-meeting-container" style="width: 100%;"></div>
  <h2 id="reloading" style="display: none"><?php print t('Reloading. Please, wait...'); ?></h2>
  <button id="start-meeting" onclick="start_meeting()"><?php print t('Start the live meeting'); ?></button>

  <div id="live-meeting-teaser">
    <?php echo theme('live_meeting_teaser', compact('node', 'live_meeting_info', 'view_mode')); ?>
  </div>


  <script>
    function start_meeting() {
      var $ = jQuery;
      Moxtra.meet({
        schedule_binder_id: '<?php print $node->schedule_binder_id[LANGUAGE_NONE][0]['value']; ?>',
        iframe: true,
        video: true,
        tagid4iframe: 'live-meeting-container',
        iframewidth: '100%',
        start_meet: function() {
          $('button#start-meeting').hide();
          $('div#live-meeting-teaser').hide();
        },
        end_meet: function(){
          $('h2#reloading').show();
          location.reload(true);
        },
        error: function(event){
          if (event.error_code == 409) { // If the meeting is already started in another window
            $('div#live-meeting-container').hide();
            $('h2#reloading').show();
            location.reload(true); // The reload will direct the host to the "join meeting" page.
          }
        }
      });
    }
  </script>

<?php } else { ?>

  <h2><?php echo t('This live meeting has not started yet'); ?></h2>
  <p><?php echo t('Come back later...'); ?></p>

<?php } ?>
