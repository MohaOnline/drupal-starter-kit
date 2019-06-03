hostingSiteBackupManagerRefreshList = function() {
  if (!Drupal.settings.hostingSiteBackupManager.nid) {
    return null;
  }

  var hostingSiteBackupManagerCallback = function(data, responseText) {
      $("#hosting-site-backup-manager-backupstable").html(data.markup);
      setTimeout("hostingSiteBackupManagerRefreshList()", 30000);
  }

  hostingTaskAddOverlay('#hosting-site-backup-manager-backupstable');
  $.get(Drupal.settings.basePath + 'node/' + Drupal.settings.hostingSiteBackupManager.nid + '/ajax/backups', null, hostingSiteBackupManagerCallback , 'json');
}

$(document).ready(function() {
  if("#hosting-site-backup-manager-backupstable") {
    setTimeout("hostingSiteBackupManagerRefreshList()", 30000);
  }
});
