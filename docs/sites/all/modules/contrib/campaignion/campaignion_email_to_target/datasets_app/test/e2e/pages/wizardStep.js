var config = require('../../../config')

module.exports = {
  url: 'http://localhost:' + (process.env.PORT || config.dev.port),
  elements: {
    chosenDataset: 'input.datasets-app-selected-dataset',
    introText: '.dsa-intro-text',
    selectOrEditDataset: '.dsa-select-or-edit-dataset'
  },
  sections: {
    selectDialog: {
      selector: '.dsa-select-dataset-dialog',
      elements: {
        box: '.el-dialog',
        title: '.el-dialog__title',
        addNewDataset: '.dsa-add-new-dataset',
        close: '.el-dialog__headerbtn'
      },
      sections: {
        datasetList: {
          selector: '.dsa-dataset-list',
          elements: {
            filter: 'input',
            datasets: 'ul.dsa-datasets',
            dataset: 'ul.dsa-datasets > li'
          }
        }
      }
    },
    editDialog: {
      selector: '.dsa-edit-dataset-dialog',
      elements: {
        box: '.el-dialog',
        title: '.el-dialog__title',
        close: '.el-dialog__headerbtn',
        datasetTitle: 'input#dsa-dataset-title',
        datasetDescription: 'input#dsa-dataset-description',
        filter: '.VueTables__search input',
        table: '.dsa-contacts-table table',
        tableHeader: '.dsa-contacts-table table > thead',
        tableBody: '.dsa-contacts-table table > tbody',
        deleteContact: '.dsa-delete-contact',
        invalidContactMark: '.dsa-invalid-contact',
        pageLink1: {
          selector: '//*[@class="VuePagination"]//a[@class="page-link" and text()="1"]',
          locateStrategy: 'xpath'
        },
        pageLink2: {
          selector: '//*[@class="VuePagination"]//a[@class="page-link" and text()="2"]',
          locateStrategy: 'xpath'
        },
        addContact: '.dsa-add-contact',
        chooseDataset: '.dsa-choose-dataset',
        alertMessage: '.dialog-alert-message',
        cancel: '.js-modal-cancel',
        save: '.js-modal-save'
      },
      sections: {
        popup: {
          selector: '.dsa-edit-value-popup',
          elements: {
            input: 'input.dsa-edit-value-input',
            error: '.dsa-edit-value-error',
            save: '.dsa-edit-value-save',
            cancel: '.dsa-edit-value-cancel'
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
