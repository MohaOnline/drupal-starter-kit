/* global suite test setup teardown suiteSetup suiteTeardown */

import { strict as assert } from 'assert'
import sinon from 'sinon'

import { TrackerManager } from '../src/tracker-manager'

suite('TrackerManager', () => {
  test('it is instantiable', () => {
    let tracker = new TrackerManager()
  })

  test('subscribe to topics', () => {
    let tracker = new TrackerManager()
    assert(typeof tracker.topics['my-topic'] === 'undefined')
    tracker.subscribe('my-topic', () => { return null })
    assert(tracker.topics['my-topic'].length === 1)
  })

  test('publish to non-existing topics', () => {
    let tracker = new TrackerManager()
    assert(typeof tracker.topics['my-topic'] === 'undefined')
    tracker.publish('my-topic', {})
  })

  test('handler get\'s called with data on publish', () => {
    let tracker = new TrackerManager()
    let expectation = sinon.expectation.create('handled').exactly(1).withExactArgs({ data: 'foo' })
    tracker.subscribe('my-topic', expectation)
    assert(tracker.topics['my-topic'].length === 1)
    tracker.publish('my-topic', { data: 'foo' })
    expectation.verify()
  })

  test('multiple handlers get called', () => {
    let tracker = new TrackerManager()
    let expectation1 = sinon.expectation.create('handled').exactly(1)
    let expectation2 = sinon.expectation.create('handled').exactly(1)
    tracker.subscribe('my-topic', expectation1)
    tracker.subscribe('my-topic', expectation2)
    tracker.publish('my-topic', { data: 'foo' })
    expectation1.verify()
    expectation2.verify()
  })
})
