<?php
 $site_name=  variable_get('site_name', 'Drupal');
?>

<div class="wrapper">
    <header class="main-header">
      <!-- Logo -->
      <a href="<?php print $front_page; ?>" class="logo">
        <!-- mini logo for sidebar mini 50x50 pixels -->
        <span class="logo-mini"><?php  print substr($site_name, 0, 3) ;  ?></span>
        <!-- logo for regular state and mobile devices -->
        <span class=" admin logo-lg">
          <?php if($logo) {  ?>
                    <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
            <?php }
                  else {
                   print substr($site_name, 0, 3) ;
                  }?>
        </span>
      </a>
      <!-- Header Navbar: style can be found in header.less -->
      <nav class="navbar navbar-static-top">
        <!-- Sidebar toggle button-->
        <a href="#" class="sidebar-toggle" data-toggle="offcanvas" role="button">
          <span class="sr-only">Toggle navigation</span>
        </a>
      </nav>
    </header>
    <aside class="main-sidebar">
      <!-- sidebar: style can be found in sidebar.less -->
      <section class="sidebar">
        <div class="management-sidebar-menu">
        <?php
            $admin_menu_name = "management";
            $admin_menu = menu_navigation_links($admin_menu_name);
            if ($admin_menu) :

              $link = menu_get_item();
              $tree = menu_tree_all_data($admin_menu_name, $link,3);
              $tree_output = menu_tree_output($tree);

              // Supply a variable for drupal_render to reference.
              $menu = drupal_render($tree_output);

              echo $menu;
          endif; ?>
       <!-- /#admin-menu -->
      </div>
      </section>
      <!-- /.sidebar -->
    </aside>
    <div class="content-wrapper">
        <!-- Content Header (Page header) -->
        <section class="content-header">
           <?php if ($title): ?>
          <h1 class="page-title"><?php print $title; ?></h1>
        <?php endif; ?>
          <?php print $breadcrumb; ?>
        <!-- /.content -->

      <div id="branding" class="clearfix">
          <?php print render($title_prefix); ?>
          <?php print render($title_suffix); ?>
          <?php print render($primary_local_tasks); ?>
      </div>

      <div id="page">
        <?php if ($secondary_local_tasks): ?>
          <div class="tabs-secondary clearfix"><?php print render($secondary_local_tasks); ?></div>
        <?php endif; ?>

        <div id="content" class="clearfix">
          <div class="element-invisible"><a id="main-content"></a></div>
          <?php if ($messages): ?>
            <div id="console" class="clearfix"><?php print $messages; ?></div>
          <?php endif; ?>
          <?php if ($page['help']): ?>
            <div id="help">
              <?php print render($page['help']); ?>
            </div>
          <?php endif; ?>
          <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
          <?php print render($page['content']); ?>
        </div>

        <div id="footer">
          <?php print $feed_icons; ?>
        </div>

      </div>
      </div>
</div>
