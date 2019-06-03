import assert from 'assert'

import App from '@/App.vue'

describe('App', function () {
  it('has a `created` hook.', function () {
    assert(typeof App.created === 'function')
  })
})
