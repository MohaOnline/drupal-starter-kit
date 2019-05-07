<?php
/**
 * @file
 * Default theme implementation for Ultimenu block (the UL list or container).
 * If you need to customize the actual Flyout, use region.tpl.php instead, or
 * @see theme_ultimenu_region().
 *
 * Available variables:
 * - $content: The renderable array containing the menu.
 * - $classes: A string containing the CSS classes for the SECTION tag:
 *   - ultimenu ORIENTATION ultimenu-MENU-NAME SKIN-NAME.
 *   - ultimenu horizontal ultimenu-htb ultimenu-main-menu ultimenu-htb-blue
 * - $classes_array: An array containing each of the CSS classes.
 *
 * The following variables are provided for contextual information.
 * - $delta: (string) The ultimenu's block delta.
 * - $config: An array of the block's configuration settings. Includes
 *   - menu_name: main-menu
 *   - menu_name_truncated: main
 *   - skin: sites/all/modules/custom/ultimenu/skins/ultimenu-htb-tabs-blue.css
 *   - skin_name:  ultimenu-htb-tabs-blue (based on safe CSS file name)
 *   - orientation: 
 *     - ultimenu-htb: horizontal to bottom
 *     - ultimenu-htt: horizontal to top
 *     - ultimenu-vtr: vertical to right
 *     - ultimenu-vtl: vertical to left
 *
 * @see template_preprocess_ultimenu()
 */
?>
<ul class="<?php print $classes; ?>">
  <?php print render($content); ?>
</ul>
