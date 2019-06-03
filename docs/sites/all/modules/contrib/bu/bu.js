(function ($) {
  Drupal.behaviors.bu = {
    attach: function (context) {
      if (context == document) {
        var e = document.createElement("script");
        e.setAttribute("type", "text/javascript");
        $buoop = {
        vs: {
          i:Drupal.settings.bu['ie'],
          f:Drupal.settings.bu['firefox'],
          o:Drupal.settings.bu['opera'],
          s:Drupal.settings.bu['safari'],
          c:Drupal.settings.bu['chrome'],
        },
        insecure:Drupal.settings.bu['insecure'],
        unsupported:Drupal.settings.bu['unsupported'],
        mobile:Drupal.settings.bu['mobile'],
        style:Drupal.settings.bu['position'],
        text: Drupal.settings.bu['text'],
        reminder: Drupal.settings.bu['reminder'],
        reminderClosed: Drupal.settings.bu['reminder_closed'],
        test: Drupal.settings.bu['debug'],
        newwindow: Drupal.settings.bu['blank'],
        noclose: Drupal.settings.bu['hide_ignore'],
        }
        e.setAttribute("src", Drupal.settings.bu['source']);
        document.body.appendChild(e);
      }
    }
  }
})(jQuery);
