<?php
function miniorange_oauth_client_reports($form,$form_state)
{
    drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    drupal_add_css( drupal_get_path('module', 'oauth_login_oauth2'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));

    global $base_url;

    $form['mo_header_style'] = array('#markup' => '<div class="mo_oauth_table_layout_1">');

    $form['mo_top_div'] = array(
        '#markup' => '<div class="mo_oauth_table_layout mo_oauth_container">',
    );

    $form['miniorange_oauth_client_report'] = array(
        '#disabled' => true,
        '#markup' => " 
                  <table id='login_reports' class='display' cellspacing='0' width='100%'>
		            <tr>
		                <th>IP Address </th>
						<th>Username </th>
						<th>Status </th>
		                <th>TimeStamp </th>   
                    </tr>
                    <tr>
                        <td>This is a Enterprise feature. <a style='color: red' href= $base_url/admin/config/people/oauth_login_oauth2/licensing>Click Here</a> to see our full list of Enterprise Features</td>
                        <td></td><td></td><td></td>
                    </tr>
		    </table>
		"
    );

    $form['mo_header_style_end'] = array('#markup' => '</div></div>');

    return $form;
}
 ?>