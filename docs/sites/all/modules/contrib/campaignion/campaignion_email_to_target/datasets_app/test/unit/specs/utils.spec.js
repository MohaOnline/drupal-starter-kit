/* eslint-disable require-jsdoc */

import assert from 'assert'

import {INVALID_CONTACT_STRING, validateContacts} from '@/utils'

describe('utils', function () {
  describe('validateContacts', function () {
    function validContactsFactory () {
      return [
        {id: 6432, first_name: 'Lenette', last_name: 'Barnby', email: 'lbarnby0@ustream.tv', foo: 'bar'},
        {id: 123, first_name: 'Kenn', last_name: 'Weinmann', email: 'kweinmann2@webnode.com', foo: 'baz'},
        {id: 8756, first_name: 'Minny', last_name: 'Goodlett', email: 'm.goodlett@example.com'}
      ]
    }

    const validations = {
      email: '^[^\\s@]+@[^\\s@]+\\.[^\\s@]+$', // backslashes have to be escaped!
      last_name: '\\S+',
      some_random_column_name: '^Hello$'
    }
    const maxFieldLengths = {
      email: 22,
      first_name: 7,
      some_random_column_name: 3
    }

    it('returns true on an array of valid contacts.', function () {
      const contacts = validContactsFactory()
      assert.equal(validateContacts({contacts, validations, maxFieldLengths}), true)
    })

    it('returns false on an array with invalid contacts.', function () {
      const contacts = validContactsFactory()
      contacts[1].email = 'foo'
      assert.equal(validateContacts({contacts, validations, maxFieldLengths}), false)
    })

    it('adds an __error property to invalid contacts.', function () {
      const contacts = validContactsFactory()
      contacts[1].email = 'foo' // Not an email address...
      contacts[2].first_name = 'Ludmilla' // Too long...
      const returnValue = validateContacts({contacts, validations, maxFieldLengths})
      assert.equal(returnValue, false)
      assert.deepEqual(contacts[0], {id: 6432, first_name: 'Lenette', last_name: 'Barnby', email: 'lbarnby0@ustream.tv', foo: 'bar'})
      assert.deepEqual(contacts[1], {id: 123, first_name: 'Kenn', last_name: 'Weinmann', email: 'foo', foo: 'baz', __error: INVALID_CONTACT_STRING})
      assert.deepEqual(contacts[2], {id: 8756, first_name: 'Ludmilla', last_name: 'Goodlett', email: 'm.goodlett@example.com', __error: INVALID_CONTACT_STRING})
    })

    it('validates only one contact if index is passed.', function () {
      const contacts = validContactsFactory()
      contacts[0].email = 'foo' // Not an email address...
      contacts[1].first_name = 'Ludmilla' // Too long...
      contacts[2].last_name = '' // Required by regex...
      const returnValue = validateContacts({contacts, validations, maxFieldLengths, index: 1})
      assert.equal(returnValue, false)
      assert.deepEqual(contacts[0], {id: 6432, first_name: 'Lenette', last_name: 'Barnby', email: 'foo', foo: 'bar'})
      assert.deepEqual(contacts[1], {id: 123, first_name: 'Ludmilla', last_name: 'Weinmann', email: 'kweinmann2@webnode.com', foo: 'baz', __error: INVALID_CONTACT_STRING})
      assert.deepEqual(contacts[2], {id: 8756, first_name: 'Minny', last_name: '', email: 'm.goodlett@example.com'})
    })

    it('removes the __error property if index is passed and the contact is valid.', function () {
      // Some contacts that had been invalid before and have been fixed, but still have the __error property:
      const contacts = [
        {id: 6432, first_name: 'Lenette', last_name: 'Barnby', email: 'lbarnby0@ustream.tv', foo: 'bar'},
        {id: 123, first_name: 'Kenn', last_name: 'Weinmann', email: 'foo@bar.com', foo: 'baz', __error: INVALID_CONTACT_STRING},
        {id: 8756, first_name: 'Lilly', last_name: 'Goodlett', email: 'm.goodlett@example.com', __error: INVALID_CONTACT_STRING}
      ]
      const returnValue = validateContacts({contacts, validations, maxFieldLengths, index: 2})
      assert.equal(returnValue, true)
      assert.deepEqual(contacts[0], {id: 6432, first_name: 'Lenette', last_name: 'Barnby', email: 'lbarnby0@ustream.tv', foo: 'bar'})
      assert.deepEqual(contacts[1], {id: 123, first_name: 'Kenn', last_name: 'Weinmann', email: 'foo@bar.com', foo: 'baz', __error: INVALID_CONTACT_STRING})
      assert.deepEqual(contacts[2], {id: 8756, first_name: 'Lilly', last_name: 'Goodlett', email: 'm.goodlett@example.com'})
    })
  })
})
