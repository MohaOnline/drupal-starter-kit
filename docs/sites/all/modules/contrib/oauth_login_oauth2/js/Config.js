/*
    jQuery($document).ready(
    function() {
        jQuery('#miniorange_oauth_client_app').change(function() {
            var appname = $document.getElementById("miniorange_oauth_client_app").value();
            if(appname=="Facebook" || appname=="Google" || appname=="Windows Account" || appname=="Custom" || appname=="Strava" || appname=="FitBit" || appname=="Eve Online"){
                
				jQuery("#mo_oauth_custom_app_name_div").show();
				
				jQuery("#test_config_button").show();
				jQuery("#clear_config_button").show();
				jQuery("#mo_oauth_attr_map_div").show();
				jQuery("#mo_oauth_attr_mapping_div").show();
				jQuery("#mo_oauth_custom_app_name").attr('required','true');
                jQuery("#callbackurl").val("<?php echo JURI::root();?>");
                jQuery("#mo_oauth_authorizeurl").attr('required','true');
			    jQuery("#mo_oauth_accesstokenurl").attr('required','true');
                jQuery("#mo_oauth_resourceownerdetailsurl").attr('required','true');
                
                if(appname != "Eve Online")
                {
				    jQuery("#mo_oauth_authorizeurl_div").show();
				    jQuery("#mo_oauth_accesstokenurl_div").show();
				    jQuery("#mo_oauth_resourceownerdetailsurl_div").show();
                }
    
                if(appname=="Facebook"){
					$document.getElementById('mo_oauth_authorizeurl').value()='https://www.facebook.com/dialog/oauth';
					$document.getElementById('mo_oauth_accesstokenurl').value()='https://graph.facebook.com/v2.8/oauth/access_token';
					$document.getElementById('mo_oauth_resourceownerdetailsurl').value()='https://graph.facebook.com/me/?fields=id,name,email,age_range,first_name,gender,last_name,link&access_token=';
				}else if(appname=="Google"){
					$document.getElementById('mo_oauth_authorizeurl').value()="https://accounts.google.com/o/oauth2/auth";
					$document.getElementById('mo_oauth_accesstokenurl').value()="https://www.googleapis.com/oauth2/v3/token";
					$document.getElementById('mo_oauth_resourceownerdetailsurl').value()="https://www.googleapis.com/plus/v1/people/me";
				}else if(appname=="Windows Account"){
					$document.getElementById('mo_oauth_authorizeurl').value()="https://login.live.com/oauth20_authorize.srf";
					$document.getElementById('mo_oauth_accesstokenurl').value()="https://login.live.com/oauth20_token.srf";
					$document.getElementById('mo_oauth_resourceownerdetailsurl').value()="https://apis.live.net/v5.0/me";
				}else if(appname=="Custom"){
					$document.getElementById('mo_oauth_authorizeurl').value()="";
					$document.getElementById('mo_oauth_accesstokenurl').value()="";
					$document.getElementById('mo_oauth_resourceownerdetailsurl').value()="";
                }
                if(appname=="Strava"){
					$document.getElementById('mo_oauth_authorizeurl').value()='https://www.strava.com/oauth/authorize';
					$document.getElementById('mo_oauth_accesstokenurl').value()='https://www.strava.com/oauth/token';
					$document.getElementById('mo_oauth_resourceownerdetailsurl').value()='https://www.strava.com/api/v3/athlete';
				}else if(appname=="FitBit"){
					$document.getElementById('mo_oauth_authorizeurl').value()="https://www.fitbit.com/oauth2/authorize";
					$document.getElementById('mo_oauth_accesstokenurl').value()="https://api.fitbit.com/oauth2/token";
					$document.getElementById('mo_oauth_resourceownerdetailsurl').value()="https://www.fitbit.com/1/user";
				}
			}
          
        })
    }
);

<form id="oauth_config_form" name="oauth_config_form" method="post" action="<?php echo JRoute::_('index.php?option=com_miniorange_oauth&view=accountsetup&task=accountsetup.saveConfig'); ?>">
		<table class="mo_settings_table"/>
			
			<tr><td><strong><?php echo JText::_('COM_MINIORANGE_OAUTH_CALLBACK_URL');?></strong></td>
				<td><input class="mo_table_textbox" id="callbackurl"  type="text" style="width: 140%;" readonly="true" value='<?php echo $redirecturi;?>'/></td>
		   </tr>

			<tr id="mo_oauth_custom_app_name_div">
				<td><strong><font color="#FF0000">*</font><?php echo JText::_('COM_MINIORANGE_OAUTH_CUSTOM_APP_NAME');?></strong></td>
				<td><input class="mo_table_textbox" type="text" style="width: 140%;" id="mo_oauth_custom_app_name" name="mo_oauth_custom_app_name" value='<?php echo $custom_app;?>'/></td>
			</tr>
			<tr>
				<td><strong><font color="#FF0000">*</font><?php echo JText::_('COM_MINIORANGE_OAUTH_CLIENT_ID');?></strong></td>
				<td><input class="mo_table_textbox" required="" type="text" style="width: 140%;" name="mo_oauth_client_id" id="mo_oauth_client_id" value='<?php echo $client_id;?>'/></td>
			</tr>
			<tr>
				<td><strong><font color="#FF0000">*</font><?php echo JText::_('COM_MINIORANGE_OAUTH_CLIENT_SECRET');?></strong></td>
				<td><input class="mo_table_textbox" required="" type="text" style="width: 140%;" name="mo_oauth_client_secret" value='<?php echo $client_secret;?>'/></td>
			</tr>
			<tr>
				<td><strong><?php echo JText::_('COM_MINIORANGE_OAUTH_APP_SCOPE');?></strong></td>
				<td><input class="mo_table_textbox" type="text" style="width: 140%;" name="mo_oauth_scope" value='<?php echo $app_scope;?>'/></td>
			</tr>
			<tr id="mo_oauth_authorizeurl_div">
				<td><strong><font color='#FF0000'>*</font><?php echo JText::_('COM_MINIORANGE_OAUTH_AUTHORIZE_ENDPOINT');?></strong></td>
				<td><input class="mo_table_textbox" required="" type="text" style="width: 140%;" id="mo_oauth_authorizeurl" name="mo_oauth_authorizeurl" value='<?php echo $authorize_endpoint;?>'/></td>
			</tr>
			<tr id="mo_oauth_accesstokenurl_div">
				<td><strong><font color="#FF0000">*</font><?php echo JText::_('COM_MINIORANGE_OAUTH_TOKEN_ENDPOINT');?></strong></td>
				<td><input class="mo_table_textbox" required="" type="text" style="width: 140%;" id="mo_oauth_accesstokenurl" name="mo_oauth_accesstokenurl" value='<?php echo $access_token_endpoint;?>'/></td>
			</tr>
			<tr id="mo_oauth_resourceownerdetailsurl_div">
				<td><strong><font color="#FF0000">*</font><?php echo JText::_('COM_MINIORANGE_OAUTH_INFO_ENDPOINT');?></strong></td>
				<td><input class="mo_table_textbox" required="" type="text" style="width: 140%;" id="mo_oauth_resourceownerdetailsurl" name="mo_oauth_resourceownerdetailsurl" value='<?php echo $user_info_endpoint;?>'/></td>
			</tr>
			<tr style="height: 30px !important; background-color: #FFFFFF;"> 
				<td colspan="3"></td>
			</tr>
			<tr>
				<td><input type="submit" name="send_query" id="send_query" value='<?php echo JText::_("COM_MINIORANGE_OAUTH_SAVE_SETTINGS_BUTTON");?>' style="margin-bottom:3%;" class="btn btn-medium btn-success" />
				<input type="button" id="test_config_button" title='<?php echo JText::_("COM_MINIORANGE_OAUTH_TEST_CONFIGURATION_MESSAGE");?>' style="margin-bottom:3%; margin-left:10px;" class="btn btn-primary" value='<?php echo JText::_("COM_MINIORANGE_OAUTH_TEST_CONFIGURATION_BUTTON");?>' onclick="testConfiguration()"/></td>
				<td><a href="index.php?option=com_miniorange_oauth&view=accountsetup&task=accountsetup.clearConfig" id="clear_config_button" style="margin-bottom:3%; margin-right:-100px; float:right;" class="btn btn-danger" /><?php echo JText::_('COM_MINIORANGE_OAUTH_CLEAR_SETTINGS_BUTTON');?></td>
			</tr>
		</table>
	</form>
	*/