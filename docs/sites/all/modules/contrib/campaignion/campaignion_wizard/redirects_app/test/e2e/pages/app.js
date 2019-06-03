var config = require('../../../config')

function destinationField (selector) {
  return {
    destinationInput: selector + ' input',
    destinationDropdown: selector + ' .dropdown-menu'
  }
}

module.exports = {
  url: 'http://localhost:' + (process.env.PORT || config.dev.port),
  elements: [
    destinationField('.pra-default-redirect'),
    {
      app: '[data-interrupt-submit]',
      addRedirect: '.pra-add-redirect',
      // element-ui appends dropdowns to body:
      dropdown: 'body > ul.el-dropdown-menu:last-of-type',
      selectDropdown: 'body > .el-select-dropdown:last-of-type ul',
      // helper links to trigger interrupt-submit.js events
      back: '#trigger-request-leave-page',
      submit: '#trigger-request-submit-page'
    }
  ],
  sections: {
    redirectList: {
      selector: '.pra-redirects',
      sections: {
        redirect: {
          selector: '.pra-redirects li.pra-redirect',
          elements: {
            info: '.pra-redirect-info',
            edit: '.pra-redirect-actions.el-dropdown .el-button-group button:first-of-type',
            openDropdown: '.pra-redirect-actions.el-dropdown .el-button-group button:last-of-type',
            duplicate: '.pra-duplicate-redirect',
            delete: '.pra-delete-redirect'
          }
        }
      }
    },
    dialog: {
      selector: '.el-dialog__wrapper',
      elements: [
        destinationField('.pra-redirect-destination'),
        {
          box: '.el-dialog',
          title: '.el-dialog__title',
          close: '.el-dialog__title',
          label: '.pra-redirect-label input',
          alertMessage: '.pra-dialog-alert-message',
          cancel: '.js-modal-cancel',
          save: '.js-modal-save'
        }
      ],
      sections: {
        filterEditor: {
          selector: '.pra-filter-editor',
          elements: {
            addFilter: 'header .el-dropdown button',
            addOptInFilter: 'header .el-dropdown ul li:nth-of-type(1)',
            addFieldFilter: 'header .el-dropdown ul li:nth-of-type(2)'
          },
          sections: {
            filterList: {
              selector: 'ul.pra-filters',
              sections: {
                optInFilter: {
                  selector: 'li.pra-filter-opt-in',
                  elements: {
                    logicalConnective: '.pra-logical-connective',
                    value: '.el-select input',
                    remove: '.remove-filter'
                  }
                },
                fieldFilter: {
                  selector: 'li.pra-filter-submission-field',
                  elements: {
                    logicalConnective: '.pra-logical-connective',
                    field: '.pra-filter-field input',
                    operator: '.pra-filter-operator input',
                    value: 'input.pra-filter-value',
                    remove: '.remove-filter'
                  }
                }
              }
            }
          }
        }
      }
    },
    messageBox: {
      selector: '.el-message-box__wrapper',
      elements: {
        box: '.el-message-box',
        title: '.el-message-box__title',
        message: '.el-message-box__message',
        cancel: '.el-message-box__btns button:first-of-type',
        ok: '.el-message-box__btns button:last-of-type'
      }
    }
  }
}
