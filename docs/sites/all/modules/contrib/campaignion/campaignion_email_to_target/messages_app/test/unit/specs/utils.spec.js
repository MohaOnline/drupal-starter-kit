import assert from 'assert'

import utils from '@/utils'

describe('utils', function () {
  describe('validateDestination', function () {
    const v = utils.validateDestination
    it('returns true for a string starting with a slash', function () {
      assert.equal(v('/foo'), true)
    })
    it('returns true for a string starting with www.', function () {
      assert.equal(v('www.foo'), true)
    })
    it('returns true for a string starting with http://', function () {
      assert.equal(v('http://foo'), true)
    })
    it('returns true for a string starting with https://', function () {
      assert.equal(v('https://bar'), true)
    })
    it('returns true for a string starting with node/', function () {
      assert.equal(v('node/baz'), true)
    })
    it('returns false for an empty string', function () {
      assert.equal(v(''), false)
    })
    it('returns false for a string containing a space', function () {
      assert.equal(v('node/my node'), false)
    })
    it('returns false for foo', function () {
      assert.equal(v('foo'), false)
    })
  })
})
