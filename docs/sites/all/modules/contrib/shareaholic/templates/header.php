<?php ShareaholicAdmin::include_css_js_assets(); ?>

<script>
  window.first_part_of_url = "<?php echo $settings['base_link']; ?>";
  window.verification_key = "<?php echo $settings['verification_key']; ?>";
  window.shareaholic_api_key = "<?php echo $settings['api_key']; ?>";
  window.SHAREAHOLIC_PLUGIN_VERSION = '<?php echo ShareaholicUtilities::get_version(); ?>';
</script>

<script>
  window.ShareaholicConfig = {
    apiKey: "<?php echo $settings['api_key']; ?>",
    assetFolders: true,
    origin: "drupal",
    language: "<?php if (isset($GLOBALS['language']->language)) { echo $GLOBALS['language']->language;} ?>"
  };
</script>
  
<!-- Header - start -->
<div id="shr-header-container"></div>
<script class="shr-app-loader shr-app-loader__header" src="<?php echo ShareaholicUtilities::asset_url('ui-header/loader.js') ?>"></script>
<!-- Header - end -->

<script>
  // override default jquery with sQuery (from jquery_custom) on shareaholic admin pages only
  $ = jQuery = sQuery;

  (function($) {
    function formatUniversalHeader() {
      var $header = $('.shr-header-content');
      var $target = $('.region.region-content');
      
      // move the header to the target and show it
      $target.before($header);
      $header.show();
    }

    $(function() {
      formatUniversalHeader();
    });

  })(jQuery);
</script>