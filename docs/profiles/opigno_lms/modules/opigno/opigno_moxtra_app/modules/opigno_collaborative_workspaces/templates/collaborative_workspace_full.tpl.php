<?php
global $user;

echo theme('moxtra_app_js_init');

$collaborative_workspace_id = $node->field_collaborative_workspace_id[LANGUAGE_NONE][0]['value'];

$collaborative_workspaces_ids = opigno_collaborative_workspaces_db_get_collaborative_workspaces_ids_from_user_id($user->uid);
$collaborative_workspace_list_html = '';
foreach($collaborative_workspaces_ids as $foreach_collaborative_workspace_id) {
  $collaborative_workspace_node = node_load($foreach_collaborative_workspace_id);
  $collaborative_workspace_list_html .= theme('collaborative_workspace_teaser_small', array('node' => $collaborative_workspace_node, 'view_mode' => 'small'));
}

?>

<div id="collaborative_workspaces_wrapper" style="height: auto; overflow: hidden;">

  <div id="collaborative_workspaces_list" style="width: 250px; float: left;">
    <?php echo $collaborative_workspace_list_html; ?>
  </div>


  <div id="collaborative_workspace_container" style="float: none; width: auto; overflow: hidden;"></div>
  <script>
    var $ = jQuery;

    function openCollaborativeWorkspace(collabWorkspaceId, collabWorkspaceTitle) {
      Moxtra.chat({
        binder_id: collabWorkspaceId,
        iframe: true,
        tagid4iframe: 'collaborative_workspace_container',
        iframewidth: '100%',
        autostart_meet: true,
        video: true,
        invite_members: true,
        produce_feeds: true
      });
      $('div#title-wrapper').find('h1').html(collabWorkspaceTitle);
    }

    $( document ).ready(function() {
      openCollaborativeWorkspace("<?php echo htmlentities($collaborative_workspace_id); ?>", "<?php echo htmlentities($node->title); ?>");
    });
  </script>

</div>