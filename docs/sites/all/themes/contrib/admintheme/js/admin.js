jQuery(document).ready(function() {
      //jQuery(#management-menu .menu-1.expanded a").hide();
     /*
		admin menu show hide
     */
  	//jQuery("#management-menu .menu-1.expanded ul li.expanded a").attr('href','javascript:;');
  	  jQuery("#management-menu .menu-1.expanded ul li.expanded").each(function(event) {
        var target = jQuery( event.target );
		jQuery(this).children().attr('href','javascript:;');
		var menu_item_text = jQuery(this).children().text();
		 jQuery(this).children().append('<span class="pull-right-container "><i class="fa fa-angle-left pull-right"></i></span>');
    });

  	/*jQuery("#management-menu .menu-1.expanded ul li.expanded a").append('<span class="pull-right-container"><i class="fa fa-angle-left pull-right"></i></span>');*/
    jQuery("#management-menu .menu-1.expanded ul li.expanded ul").hide();
  	jQuery("#management-menu .menu-1.expanded ul li.expanded a").click(function() {
  		 jQuery(this).parent().toggleClass("menu-open");
  		 jQuery(this).parent().find("ul").slideToggle( "slow" );

    });

 	/****** select2 *****/
 	jQuery('select.form-select').select2();
  if (jQuery(".submenu-active").length > 0) {
    jQuery(".submenu-active").parent().addClass("parent-active");
    jQuery(".submenu-active").parent().toggleClass("menu-open");
      jQuery(".submenu-active").parent().show();
     jQuery(".submenu-active").parent().parent().addClass("active menu-open");
  }

});
