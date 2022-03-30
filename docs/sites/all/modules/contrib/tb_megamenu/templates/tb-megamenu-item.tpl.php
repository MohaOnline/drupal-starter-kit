<?php if(isset($item['link']['href'])) : ?>
  <li <?php print $attributes;?> class="<?php print $classes;?>">
    <?php if ($item['link']['href'] == "<nolink>"): ?>
      <a href="#" class="tb_nolink <?php if (isset($item['link']['localized_options']['attributes']['class'])) { print check_plain(implode(' ', $item['link']['localized_options']['attributes']['class'])); } ?>">
        <?php if(!empty($item_config['xicon'])) : ?>
          <i class="<?php print $item_config['xicon'];?>"></i>
        <?php endif;?>
        <?php print t(check_plain($item['link']['link_title']));?>
      </a>
    <?php elseif ($item['link']['href'] == "<separator>"): ?>
      <div class="tb_separator <?php if (isset($item['link']['localized_options']['attributes']['class'])) { print check_plain(implode(' ', $item['link']['localized_options']['attributes']['class'])); } ?>"></div>
    <?php else: ?>
      <a href="<?php print check_url(url($item['link']['href'], $item['link']['options']));?>" <?php echo drupal_attributes($item['link']['#attributes']); ?>>
        <?php if(!empty($item_config['xicon'])) : ?>
          <i class="<?php print $item_config['xicon'];?>"></i>
        <?php endif;?>
        <?php print t(check_plain($item['link']['title']));?>
        <?php if($submenu && $block_config['auto-arrow']) :?>
          <span class="caret"></span>
        <?php endif;?>
        <?php if(!empty($item_config['caption'])) : ?>
          <span class="mega-caption"><?php print t($item_config['caption']);?></span>
        <?php endif;?>
      </a>
    <?php endif; ?>
    <?php print $submenu ? $submenu : "";?>
  </li>
<?php endif;?>