/* global Drupal, jQuery */

import { Client } from './client'
import * as validate from './validate'


const $ = jQuery
let client = null
Drupal.behaviors.campaignion_loqate = {}
Drupal.behaviors.campaignion_loqate.attach = function (context, settings) {
  if (settings.loqate && !client) {
    client = new Client(settings.loqate)
  }
  $(document).bind('clientsideValidationAddCustomRules', function (event) {
    jQuery.validator.addMethod('loqateSortCode', function (value, element, param) {
      const previous = this.previousValue(element)
      if (previous.old === value) {
        return previous.valid
      }
      previous.old = value
      previous.message = this.defaultMessage(element, 'loqateSortCode')
      this.startRequest(element)
      client.validateSortCode(value).then((item) => {
        const valid = !item.Error
        this.stopRequest(element, valid)
        previous.valid = valid
        if (valid) {
          validate.markValid.call(this, element)
        }
        else {
          validate.markInvalid.call(this, element, previous.message)
        }
      })
      return 'pending';
    })
    jQuery.validator.addMethod('loqateAccount', function (value, element, param) {
      const $sortCodeElement = $(param.sortCodeElement)
      if (!$sortCodeElement.is(':filled') || !$sortCodeElement.valid()) {
        return 'dependency-mismatch';
      }
      const previous = this.previousValue(element)
      if (previous.old === value) {
        return previous.valid
      }
      previous.old = value
      previous.message = this.defaultMessage(element, 'loqateAccount')
      this.startRequest(element)
      client.validateAccount($sortCodeElement.val(), value).then((item) => {
        const valid = !item.Error
        this.stopRequest(element, valid)
        previous.valid = valid
        if (valid) {
          validate.markValid.call(this, element)
        }
        else {
          validate.markInvalid.call(this, element, previous.message)
        }
      })
      return 'pending';

    })
  })
}
