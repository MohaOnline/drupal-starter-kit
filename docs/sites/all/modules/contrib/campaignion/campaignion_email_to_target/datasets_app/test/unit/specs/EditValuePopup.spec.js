import assert from 'assert'

import EditValuePopup from '@/components/EditValuePopup'

describe('EditValuePopup', function () {
  describe('computed', function () {
    describe('valid', function () {
      const valid = EditValuePopup.computed.valid
      const thisFactory = function (val) {
        return {
          value: val,
          editValue: {col: 'first_name'},
          validator: new RegExp('Hein'),
          maxFieldLengths: {first_name: 6}
        }
      }
      it('returns true for a name that matches the regex and fits the max length.', function () {
        assert(valid.call(thisFactory('Heiner')) === true)
      })
      it('returns false for a name that doesn’t match the regex.', function () {
        assert(valid.call(thisFactory('Susi')) === false)
      })
      it('returns false for a name that’s too long.', function () {
        assert(valid.call(thisFactory('Heinrich')) === false)
      })
    })

    describe('errorMessage', function () {
      const errorMessage = EditValuePopup.computed.errorMessage
      const context = {
        editValue: {col: 'first_name'},
        validator: new RegExp('Hein'),
        maxFieldLengths: {first_name: 8},
        label: 'First name'
      }
      it('returns an error message if the value is too long.', function () {
        const msg = errorMessage.call(Object.assign({}, context, {value: 'Marianne-Antonia'}))
        assert(msg === 'Make sure that this field is not longer than 8 characters.')
      })
      it('returns another error message if the regex doesn’t match.', function () {
        const msg = errorMessage.call(Object.assign({}, context, {value: 'Marianne'}))
        assert(msg === 'Please enter a valid first name')
      })
    })
  })
})
