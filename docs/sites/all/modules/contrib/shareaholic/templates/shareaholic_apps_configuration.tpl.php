<div id='app_settings'>
  <?php $page_types = ShareaholicUtilities::page_types(); ?>
  <?php $settings = ShareaholicUtilities::get_settings(); ?>

  <fieldset class="app">
    <legend><h2><i class="icon icon-share_buttons"></i><?php print t('In-Page Share Buttons'); ?></h2></legend>

    <span class="helper"><i class="icon-star"></i> <?php print t('Pick where you want your in-page share buttons to be displayed. Click "customize" to customize look & feel, themes, share counters, alignment, and more.'); ?></span>

    <?php foreach($page_types as $key => $page_type) { ?>
    <fieldset id='sharebuttons'>
      <legend><?php echo ucwords($page_type->name) ?></legend>
      <?php foreach(array('above', 'below') as $position) { ?>
        <?php if (isset($settings['location_name_ids']['share_buttons']["{$page_type->type}_{$position}_content"])) { ?>
          <?php $location_id = $settings['location_name_ids']['share_buttons']["{$page_type->type}_{$position}_content"] ?>
        <?php } else { $location_id = ''; } ?>
          <div>
            <input type="checkbox" id="share_buttons_<?php echo "{$page_type->type}_{$position}_content" ?>" name="share_buttons[<?php echo "{$page_type->type}_{$position}_content" ?>]" class="check"
            <?php if (isset($settings['share_buttons']["{$page_type->type}_{$position}_content"])) { ?>
              <?php echo ($settings['share_buttons']["{$page_type->type}_{$position}_content"] == 'on' ? 'checked' : '') ?>
            <?php } ?>>
            <input type="hidden" id="share_buttons_<?php echo "{$page_type->type}_{$position}_content_location_id" ?>" name="share_buttons[<?php echo "{$page_type->type}_{$position}_content_location_id" ?>]" value="<?php echo $location_id ?>"/>
            <label for="share_buttons_<?php echo "{$page_type->type}_{$position}_content" ?>"><?php echo ucfirst($position) ?> Content</label>
            <button data-app='share_buttons'
                    data-location_id='<?php echo $location_id ?>'
                    data-href='share_buttons/locations/{{id}}/edit'
                    class="mll btn btn-success">
            <?php print t('Customize'); ?></button>
          </div>
      <?php } ?>
    </fieldset>
    <?php } ?>
    <div class='fieldset-footer'>
      <span class="helper"><i class="icon-star"></i> Brand your shares with your @Twitterhandle, pick your favorite URL shortener, share buttons for images, etc.</span>
      <button class='app_wide_settings btn' data-href='share_buttons/edit'><?php echo t('Edit Settings'); ?></button>
    </div>
  </fieldset>
  
  <div class='clear'></div>
  
  <fieldset class="app">
    <legend><h2><i class="icon icon-recommendations"></i><?php print t('Related Content'); ?></h2></legend>

    <span class="helper"><i class="icon-star"></i> <?php print t('Pick where you want the app to be displayed. Click "Customize" to customize look & feel, themes, block lists, etc.'); ?></span>
    <?php foreach($page_types as $key => $page_type) { ?>
      <?php if (isset($settings['location_name_ids']['recommendations']["{$page_type->type}_below_content"])) { ?>
        <?php $location_id = $settings['location_name_ids']['recommendations']["{$page_type->type}_below_content"] ?>
      <?php } else { $location_id = ''; } ?>
      <fieldset id='recommendations'>
        <legend><?php echo ucwords($page_type->name) ?></legend>
          <div>
            <input type="checkbox" id="recommendations_<?php echo "{$page_type->type}_below_content" ?>" name="recommendations[<?php echo "{$page_type->type}_below_content" ?>]" class="check"
            <?php if (isset($settings['recommendations']["{$page_type->type}_below_content"])) { ?>
              <?php echo ($settings['recommendations']["{$page_type->type}_below_content"] == 'on' ? 'checked' : '') ?>
            <?php } ?>>
            <input type="hidden" id="recommendations_<?php echo "{$page_type->type}_below_content_location_id" ?>" name="recommendations[<?php echo "{$page_type->type}_below_content_location_id" ?>]" value="<?php echo $location_id ?>"/>
            <label for="recommendations_<?php echo "{$page_type->type}_below_content" ?>">Below Content</label>
            <button data-app='recommendations'
                    data-location_id='<?php echo $location_id ?>'
                    data-href="recommendations/locations/{{id}}/edit"
                    class="mll btn btn-success">
            <?php print t('Customize'); ?></button>
          </div>
      </fieldset>
    <?php } ?>

    <div class="fieldset-footer">
      <span class="helper"><i class="icon-star"></i> Re-sync your content, exclude pages from being recommended, etc.</span>
      <button class='app_wide_settings btn' data-href='recommendations/edit'><?php print t('Edit Settings'); ?></button>
    </div>
  </fieldset>

  <div class='clear'></div>

  <fieldset class="app">
    <legend><h2><i class="icon icon-affiliate"></i><?php echo t('Monetization'); ?></h2></legend>
    <span class="helper"><i class="icon-star"></i> <?php echo t('Configure Promoted Content, Affiliate Links, Banner Ads, etc. Check your earnings at any time.'); ?></span>
    <button class='app_wide_settings btn' data-href='monetizations/edit'><?php echo t('Edit Settings'); ?></button>
  </fieldset>
</div>

<div class='clear'></div>

<div class="row" style="padding-top:20px; padding-bottom:35px;">
  <div class="span2">
    <?php print $variables['shareaholic_apps_configuration']['hidden'] ?>
    <?php print $variables['shareaholic_apps_configuration']['submit'] ?>
  </div>
</div>
