/**
 * Basic and default implementation of Swiper plugin
 * 
 * @info Note that you can add more plugin options in the swiper method initiation,
 * to do this, implements a hook_swiper_options_alter in your module,
 * this hook extends the plugin options passed to the swiper method initiation.
 */
window.swiper = {};
(function($) {
  $(document).ready(function() {
    $.each(Drupal.settings.swiper, function(index, swiperSettings) {
      var nodeId = index.replace('nid-', '');
      var swiperContainerElement = '.swiper-container-nid-' + nodeId;

      if ($(swiperContainerElement).length) {
        window.swiper[nodeId] = new Swiper(swiperContainerElement, swiperSettings.options);

        // Clickable Pagination implementation
        $('.pagination-nid-' + nodeId + ' .swiper-pagination-switch').click(function() {
          window.swiper[nodeId].swipeTo($(this).index());
        });
      }
    });
  });
})(jQuery);
