import assert from 'assert'
import sinon from 'sinon'

import c from '@/components/SpecDialog'

import {OPERATORS, emptySpec} from '@/utils/defaults'

afterEach(() => {
  sinon.restore()
})

describe('SpecDialog', function () {
  describe('data', function () {
    it('returns init data', function () {
      const data = c.data()
      assert.deepEqual(data.currentSpec, emptySpec('message-template'))
      assert.equal(data.exclusionMode, 'message')
      assert.equal(data.visible, false)
      assert.equal(data.modalDirty, false)
      assert.deepEqual(data.OPERATORS, OPERATORS)
    })
  })

  describe('computed', function () {
    describe('destination', function () {
      it('gets url and urlLabel.', function () {
        const destination = c.computed.destination.get.call({
          currentSpec: {
            url: 'node/23',
            urlLabel: 'My awesome node'
          }
        })
        assert.deepEqual(destination, {
          value: 'node/23',
          label: 'My awesome node'
        })
      })
      it('sets url and urlLabel.', function () {
        const context = {currentSpec: {}}
        c.computed.destination.set.call(context, {
          value: 'node/27',
          label: 'Your terrific node'
        })
        assert.equal(context.currentSpec.url, 'node/27')
        assert.equal(context.currentSpec.urlLabel, 'Your terrific node')
      })
    })

    describe('destinationIsValid', function () {
      it('validates this.url', function () {
        // There doesn’t seem to be a simple way to stub utils.validateDestination
        // with babel. See this issue: https://github.com/sinonjs/sinon/issues/1711
        assert.equal(c.computed.destinationIsValid.call({
          currentSpec: {url: 'foo'}
        }), false)
        assert.equal(c.computed.destinationIsValid.call({
          currentSpec: {url: '/node/foo'}
        }), true)
      })
    })
  })

  describe('methods', function () {
    describe('updateSpec', function () {
      const updateSpec = c.methods.updateSpec
      var context

      beforeEach(function () {
        context = {
          currentSpec: emptySpec('exclusion'),
          exclusionMode: 'message',
          $store: {commit: sinon.fake()},
          close: sinon.fake()
        }
      })

      it('commits the updateSpec mutation.', function () {
        updateSpec.call(context)
        assert(context.$store.commit.called)
        assert.deepEqual(context.$store.commit.firstCall.args[0], {
          type: 'updateSpec',
          spec: context.currentSpec
        })
      })
      it('for exclusions, it makes sure that url is empty if message is selected.', function () {
        context.exclusionMode = 'message'
        context.currentSpec.message.body = 'foo'
        context.currentSpec.url = 'node/13'
        context.currentSpec.urlLabel = 'My node'
        updateSpec.call(context)
        assert.equal(context.currentSpec.message.body, 'foo')
        assert.equal(context.currentSpec.url, '')
        assert.equal(context.currentSpec.urlLabel, '')
      })
      it('for exclusions, it makes sure that message.body is empty if redirect is selected.', function () {
        context.exclusionMode = 'redirect'
        context.currentSpec.message.body = 'foo'
        context.currentSpec.url = 'node/13'
        context.currentSpec.urlLabel = 'My node'
        updateSpec.call(context)
        assert.equal(context.currentSpec.message.body, '')
        assert.equal(context.currentSpec.url, 'node/13')
        assert.equal(context.currentSpec.urlLabel, 'My node')
      })
    })

    describe('determineExclusionMode', function () {
      const d = c.methods.determineExclusionMode
      var context

      beforeEach(function () {
        context = {
          currentSpec: emptySpec('exclusion')
        }
      })

      it('sets exclusionMode to message if message and url are empty (new specs)', function () {
        d.call(context)
        assert.equal(context.exclusionMode, 'message')
      })
      it('sets exclusionMode to redirect if there is a url', function () {
        context.currentSpec.url = '/foo'
        d.call(context)
        assert.equal(context.exclusionMode, 'redirect')
      })
      it('sets exclusionMode to message if there is a message.body', function () {
        context.currentSpec.message.body = 'foo'
        d.call(context)
        assert.equal(context.exclusionMode, 'message')
      })
    })
  })
})
