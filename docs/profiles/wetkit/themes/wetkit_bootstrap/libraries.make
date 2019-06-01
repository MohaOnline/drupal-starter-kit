; ##############################################################################
;
; This is a Drush make file that will automatically download the front-end
; libraries used by WxT Bootstrap. Alternatively, you can use Bower (http://bower.io)
; to accomplish this.
;
; Running Drush make in your sub-theme will cause the libraries to be downloaded
; into your theme. If you want to download them into Omega directly to make them
; available to all of your sub-themes (if you have multiple) then you should
; instead run omega.make from the Omega theme directory.
;
; To run this file with 'drush make' you first have to navigate into your theme.
; Normally, this would be 'sites/all/themes/wetkit_bootstrap'.
;
; $ cd sites/all/themes/wetkit_bootstrap
;
; Now you can invoke 'drush make' using the following command:
;
; $ drush make libraries.make --no-core --contrib-destination=.
;
; ##############################################################################

core = 7.x
api = 2

libraries[wet-boew][download][type] = get
libraries[wet-boew][download][url] = https://github.com/wet-boew/wet-boew/releases/download/v4.0.1/wet-boew-dist-4.0.1.zip
