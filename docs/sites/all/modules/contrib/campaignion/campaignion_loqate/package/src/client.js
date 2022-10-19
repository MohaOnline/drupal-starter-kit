/* global jQuery */

var $ = jQuery

/**
 * API client for the loqate JSON endpoints.
 */
class Client {
  constructor (settings) {
    this.settings = settings
  }
  validateSortCode (sortCode) {
    return new Promise((resolve, reject) => {
      $.post(this.settings.sortCodeEndpoint, {
        Key: this.settings.key,
        SortCode: sortCode,
      }).done((data) => {
        if (data.Items.length === 1) {
          let item = data.Items[0]
          if (typeof item.Error !== 'undefined') {
            item.Error = parseInt(item.Error)
            if (item.Error < 1000) {
              // Validation errors have codes >1000. Smaller error codes seem to
              // be setup issues.
              reject(Error('Configuration error'))
            }
          }
          resolve(item)
        }
        else {
          // No bank / branch with this sort code found.
          resolve({
            Error: -101,
            Description: 'No matching branch found.',
          })
        }
      }).fail(function (xhr) {
        reject(Error('Request error'))
      })
    })
  }
  validateAccount (sortCode, account) {
    return new Promise((resolve, reject) => {
      $.post(this.settings.accountEndpoint, {
        Key: this.settings.key,
        AccountNumber: account,
        SortCode: sortCode,
      }).done((data) => {
        if (data.Items.length === 1) {
          let item = data.Items[0]
          if (typeof item.Error !== 'undefined') {
            item.Error = parseInt(item.Error)
            if (item.Error < 1000) {
              // Validation errors have codes >1000. Smaller error codes seem to
              // be setup issues.
              reject(Error('Configuration error'))
            }
          }
          if (!item.Error && !item.IsDirectDebitCapable) {
            resolve({
              Error: -100,
              Description: 'Account is not able to handle direct debit.',
            })
          }
          resolve(item)
        }
        else {
          // No account with this sort code found.
          resolve({
            Error: -101,
            Description: 'No matching account found.',
          })
        }
      }).fail((xhr) => {
        reject(Error('Request error'))
      })
    })
  }
}

export { Client }
