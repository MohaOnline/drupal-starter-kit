(function ($) {
  'use strict';

  const TRACKABLE_NODE_SELECTORS = ['a', 'button', "input[type='submit']"];

  CKEDITOR.plugins.add('fintezaAnalyticsCustomEvents', {
    icons: 'fintezaAnalyticsAddEvent',
    hidpi: true,
    lang: ['en', 'ru', 'zh-cn'],
    init(editor) {
      editor.on('selectionChange', (e, d) => {
        const node = editor.getSelection().getStartElement().$;
        const command = editor.getCommand('fintezaAnalyticsAddEventDialog');

        if (isNodeTrackable(node)) {
          command.setState(CKEDITOR.TRISTATE_OFF);
        }
        else {
          command.setState(CKEDITOR.TRISTATE_DISABLED);
        }
      });

      editor.addCommand(
        'fintezaAnalyticsAddEventDialog',
        new CKEDITOR.dialogCommand('fintezaAnalyticsAddEventDialog', {
          allowedContent: '*[data-fz-event]',
          startDisabled: true
        })
      );

      editor.ui.addButton('fintezaAnalyticsAddEvent', {
        label: editor.lang.fintezaAnalyticsCustomEvents.toolbarButtonLabel,
        command: 'fintezaAnalyticsAddEventDialog',
        toolbar: 'insert'
      });

      CKEDITOR.dialog.add('fintezaAnalyticsAddEventDialog', editor => {
        const dialogDefinition = {
          title: editor.lang.fintezaAnalyticsCustomEvents.windowTitle,
          minWidth: 340,
          minHeight: 80,
          contents: [
            {
              id: 'main',
              padding: 0,
              elements: [
                {
                  id: 'eventName',
                  type: 'text',
                  label: editor.lang.fintezaAnalyticsCustomEvents.fieldLabel,
                  title: editor.lang.fintezaAnalyticsCustomEvents.fieldTitle,
                  default: '',
                  onShow() {
                    const node = editor.getSelection().getStartElement().$;
                    const eventName = getEventName(node);
                    this.setValue(eventName);
                  }
                }
              ]
            }
          ],
          buttons: [CKEDITOR.dialog.okButton, CKEDITOR.dialog.cancelButton],
          onOk() {
            const eventName = this.getContentElement(
              'main',
              'eventName'
            ).getValue();
            const node = editor.getSelection().getStartElement().$;
            setEventName(node, eventName);
          }
        };

        return dialogDefinition;
      });
    }
  });

  function isNodeTrackable(node) {
    for (let index = 0; index < TRACKABLE_NODE_SELECTORS.length; index++) {
      const selector = TRACKABLE_NODE_SELECTORS[index];

      if ($(node).is(selector)) {
        return true;
      }
    }
    return false;
  }

  function getEventName(node) {
    return node.getAttribute('data-fz-event');
  }

  function setEventName(node, name) {
    if (name) {
      node.setAttribute('data-fz-event', name);
    }
    else {
      node.removeAttribute('data-fz-event');
    }
  }
})(jQuery);
