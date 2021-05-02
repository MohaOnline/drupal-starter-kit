# Campaignion layouts

This module enables themes to have different layout variations. It provides a field that can be added to nodes in order for the user to select which layout variation to use when displaying this node.


## Available layouts

In order for a layout to be active / or selectable the following conditions need to be met:

- The layout must be **declared** by an enabled theme by implementing `hook_campaignion_layout_info()` (this special hook is invoked on all enabled themes, not modules).
- The layout must be **implemented** by the theme by adding it to the `layout[]` property in its `.info`-file (and perhaps adjusting styles and templates). Themes inherit the `layout[]` property from their base themes.
- The layout must be **enabled** for the theme in its theme settings.

The form element on the node shows all layouts that are enabled on any enabled theme. It disables all options that are not currently available for the selected theme.

## Default layout

If a theme specifies a default layout in its info file (using `layout_default = layout_machine_name`) then this layout is set active whenever this theme is active and no other enabled layout is selected. The default layout can’t be disabled.
