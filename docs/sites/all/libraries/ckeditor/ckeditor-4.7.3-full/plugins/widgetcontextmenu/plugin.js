/*
 * Widget Context Menu
 *
 * @author CX Builders - Bastiaan van der Kooij (cxbuilders.com)
 * @version 1.13
 */

(function() {
    CKEDITOR.plugins.add('widgetcontextmenu', {
        lang: 'en',
        hidpi: true,
        init: function(editor) {
            var self = this;
            self.TargetWidget = null;
            self.CanEdit = editor.config.widgetcontextmenu_edit != false;
            self.CanCopy = editor.config.widgetcontextmenu_copy != false;
            self.CanRemove = editor.config.widgetcontextmenu_remove != false;

            if (!self.CanEdit && !self.CanCopy && !self.CanRemove) {
                return;
            }

            editor.addCommand('editwidget', {
                modes: { wysiwyg: 1, source: 0 },
                exec: function(editor) {
                    if (self.TargetWidget != null) {
                        self.TargetWidget.edit();
                    }
                },
                canUndo: true
            });

            editor.addCommand('copywidget', {
                modes: { wysiwyg: 1, source: 0 },
                exec: function(editor) {
                    if (self.TargetWidget != null) {
                        self.TargetWidget.focus();
                        editor.execCommand( 'copy' );
                    }
                },
                canUndo: true
            });

            editor.addCommand('removewidget', {
                modes: { wysiwyg: 1, source: 0 },
                exec: function(editor) {
                    if (self.TargetWidget != null) {
                        var Element = self.TargetWidget.element;
                        self.TargetWidget.destroy();
                        Element.remove();
                    }
                },
                canUndo: true
            });

            if (editor.addMenuItems) {
                editor.addMenuGroup('widget');

                var IconPath = this.path + 'icons/' + (CKEDITOR.env.hidpi ? 'hidpi/' : '');
                CKEDITOR.skin.addIcon('widget', IconPath + 'widget.png');
                CKEDITOR.skin.addIcon('editwidget', IconPath + 'editwidget.png');
                CKEDITOR.skin.addIcon('copywidget', IconPath + 'copywidget.png');
                CKEDITOR.skin.addIcon('removewidget', IconPath + 'removewidget.png');

                editor.addMenuItems({
                    widget: {
                        label: 'Widget',
                        group: 'widget',
                        getItems: function() {
                            var MenuItems = {};
                            if (self.CanEdit) { MenuItems.editwidget = CKEDITOR.TRISTATE_OFF; }
                            if (self.CanCopy) { MenuItems.copywidget = CKEDITOR.TRISTATE_OFF; }
                            if (self.CanRemove) { MenuItems.removewidget = CKEDITOR.TRISTATE_OFF; }
                            return MenuItems;
                        }
                    },
                    editwidget: {
                        label: editor.lang[this.name].edit,
                        icon: 'editwidget',
                        group: 'widget',
                        command: 'editwidget'
                    },
                    copywidget: {
                        label: editor.lang[this.name].copy,
                        icon: 'copywidget',
                        group: 'widget',
                        command: 'copywidget'
                    },
                    removewidget: {
                        label: editor.lang[this.name].remove,
                        icon: 'removewidget',
                        group: 'widget',
                        command: 'removewidget'
                    }
                });
            }


            if (editor.contextMenu) {
                editor.contextMenu.addListener(function(element, selection) {
                    self.TargetWidget = editor.widgets.widgetHoldingFocusedEditable || editor.widgets.focused;
                    if (self.TargetWidget != null) {
                        var WidgetMenuItem = editor.getMenuItem('widget');
                        WidgetMenuItem.label = self.TargetWidget.element.getAttribute( 'title' ) || self.TargetWidget.name;
                        return {
                            widget: CKEDITOR.TRISTATE_OFF
                        };
                    }
                });
            }
        }
    });
})();