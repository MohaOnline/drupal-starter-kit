import assert from 'assert'

import defaults from '@/utils/defaults'

describe('defaults', function () {
  describe('emptySpec', function () {
    it('returns an empty spec object', function () {
      const s = defaults.emptySpec('message-template')
      assert.equal(s.id, null)
      assert.equal(s.type, 'message-template')
      assert.equal(s.label, '')
      assert.deepEqual(s.filters, [])
      assert.deepEqual(s.message, defaults.messageObj())
      assert.equal(s.url, '')
      assert.equal(s.urlLabel, '')
      assert.deepEqual(s.errors, [])
    })
  })
})
