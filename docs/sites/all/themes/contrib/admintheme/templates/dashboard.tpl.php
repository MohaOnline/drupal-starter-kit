<?php
/*
File
This page use for admin dashboard
*/
global $base_url;


?>
<section class="content">

<!-- Count section -->
<div class="row">
        <div class="col-lg-3 col-xs-6">
          <!-- small box -->
          <div class="small-box bg-aqua">
            <div class="inner">
            	<?php 
            	$comment_query = db_select('comment', 'comment')
      					->fields('comment', array('cid'))
      					->execute();
      			$comment_query_count = $comment_query->rowCount(); ?>
              <h3><?php echo $comment_query_count;?></h3>

              <p>ToTal Comment</p>
            </div>
            <div class="icon">
              <i class="fa fa-comment"></i>
            </div>
            <a target="_blank" href="<?php echo $base_url;?>/admin/content/comment" class="small-box-footer">More info <i class="fa fa-arrow-circle-right"></i></a>
          </div>
        </div>
        <!-- ./col -->
        <div class="col-lg-3 col-xs-6">
          <!-- small box -->
          <div class="small-box bg-green">
            <div class="inner">
            	<?php 
            		$users_query = db_select('users', 'users')
      					->fields('users', array('uid'))
      					->execute();
      			$users_query_count = $users_query->rowCount(); ?>
              <h3><?php echo $users_query_count; ?></h3>

              <p>ToTal Users</p>
            </div>
            <div class="icon">
              <i class="fa fa-users"></i>
            </div>
            <a target="_blank" href="<?php echo $base_url;?>/admin/people" class="small-box-footer">More info <i class="fa fa-arrow-circle-right"></i></a>
          </div>
        </div>
        <!-- ./col -->
      </div>

<!-- latest post -->

<div class="row">
	<div class="col-md-4 col-xs-12">
              <!-- USERS LIST -->
              <div class="box box-danger recent-user-list-section">
                <div class="box-header with-border ">
                  <h3 class="box-title">Latest Members</h3>

                  	<?php

                  		$recent_users = db_select('users', 'users')
      					->fields('users', array('uid', 'name', 'created'))
      					->orderBy("created", "DESC")
      					->range(0, 8)
      					->execute()
      					->fetchAll();
      				?>
                  <div class="box-tools pull-right">
                    <span class="label label-danger"><?php echo count($recent_users); ?> New Members</span>
                    <button type="button" class="btn btn-box-tool" data-widget="collapse"><i class="fa fa-minus"></i>
                    </button>
                    <button type="button" class="btn btn-box-tool" data-widget="remove"><i class="fa fa-times"></i>
                    </button>
                  </div>
                </div>
                <!-- /.box-header -->
                <div class="box-body no-padding">
                  <ul class="users-list recent-user-list clearfix">
                  	<?php 
                  	foreach ($recent_users as $key => $recent_users_value) :  
      				//echo"<pre>";print_r($recent_users_value);echo "</pre>"; ?>
      	 		
                    <li>
                      <p class="users-list-name-circle"> <?php echo substr($recent_users_value->name, 0,1); ?></p>
                      <a class="users-list-name" href="<?php echo $base_url . "/user/" . $recent_users_value->uid ."/edit" ?>"><?php echo substr($recent_users_value->name,0,5) . ".."; ?></a>
                      <span class="users-list-date"><?php echo  date("d M", strtotime($recent_users_value->created));?></span>
                    </li>
                	<?php endforeach; ?>
                  </ul>
                  <!-- /.users-list -->
                </div>
                <!-- /.box-body -->
                <div class="box-footer text-center">
                  <a href="<?php echo $base_url . "/admin/people";?>" class="uppercase">View All Users</a>
                </div>
                <!-- /.box-footer -->
              </div>
              <!--/.box -->
    </div>
    <div class="col-xs-4 col-xs-12">
    	<div class="box box-primary recent_node_post">
            <div class="box-header with-border">
              <h3 class="box-title">Recently Added Products</h3>

              <div class="box-tools pull-right">
                <button type="button" class="btn btn-box-tool" data-widget="collapse"><i class="fa fa-minus"></i>
                </button>
                <button type="button" class="btn btn-box-tool" data-widget="remove"><i class="fa fa-times"></i></button>
              </div>
            </div>
            <!-- /.box-header -->
            <div class="box-body" style="">
            	<?php

                  		$recent_nodes = db_select('node', 'node')
      					->fields('node', array('nid', 'title', 'created'))
      					->orderBy("created", "DESC")
      					->range(0, 5)
      					->execute()
      					->fetchAll();
      			?>

              <ul class="products-list product-list-in-box">
              	<?php foreach ($recent_nodes as $key => $recent_nodes_value) : 
              		$node_data = node_load($recent_nodes_value->nid);
 
              	?>  
                <li class="item">
                  <div class="product-img">
                      <p class="node-list-name-circle"> <?php echo substr($recent_nodes_value->title, 0,1); ?></p>
                  </div>
                  <div class="product-info">
                    <a href=<?php echo $base_url . drupal_get_path_alias('/node/' . $recent_nodes_value->nid);?> class="product-title"> <?php echo substr($recent_nodes_value->title, 0,10) . ".."; ?></a>
                    <?php if (!empty($node_data->body)) : ?>
	                    <span class="product-description">
	                          <?php echo substr($node_data->body['und'][0]['value'], 0, 15) . ".."; ?>
	                     </span>
	                <?php endif; ?>
                  </div>
                </li>
            <?php endforeach; ?>
              </ul>
            </div>
            <!-- /.box-body -->
            <div class="box-footer text-center" style="">
              <a href="<?php echo $base_url . "/admin/content"; ?>" class="uppercase">View All Products</a>
            </div>
            <!-- /.box-footer -->
          </div>
    </div>
</div>

</section>