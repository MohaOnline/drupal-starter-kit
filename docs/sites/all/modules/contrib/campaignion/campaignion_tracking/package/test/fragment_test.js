/* global suite test setup teardown suiteSetup suiteTeardown */

import { strict as assert } from 'assert'
import sinon from 'sinon'

import {
  parseLocationHash,
  consumeLocationHashForPrefixes,
  parseLocationHashForPrefixes,
  parsePartsWithPrefix,
  parsePart
} from '../src/fragment'

suite('parse location hash', () => {
  test('empty hash returns empty list', () => {
    const locationHash = ''
    const parsed = parseLocationHash(locationHash)
    sinon.assert.match(parsed, [])
  })

  test('delimit parts with semicolon', () => {
    const locationHash = 'key1=a;key2=b'
    const parsed = parseLocationHash(locationHash)
    assert(Array.isArray(parsed))
    assert(parsed.length === 2)
  })

  test('return list of match objects', () => {
    const locationHash = 'key1=a;key2=b,c'
    const parsed = parseLocationHash(locationHash)
    assert(parsed[0].id === 'key1')
    sinon.assert.match(parsed[0].codes, ['a'])
    assert(parsed[1].id === 'key2')
    sinon.assert.match(parsed[1].codes, ['b', 'c'])
  })

  test('return nothing if no prefix is whitelisted', () => {
    const locationHash = 'key1=a;key2=b,c;prefix1:key3=d'
    const parsed = parseLocationHashForPrefixes([], locationHash)
    assert(parsed.length === 0)
  })

  test('return matches for whitelisted prefixes', () => {
    const locationHash = 'key1=a;prefix1:key2=b,c;prefix3:key3=d'
    const parsed = parseLocationHashForPrefixes(['prefix1', 'prefix3'], locationHash)
    // matches nested objects
    sinon.assert.match(parsed[0], { prefix: 'prefix1' })
    sinon.assert.match(parsed[1], { prefix: 'prefix3' })
  })

  test('return matches for whitelisted prefixes in order of location hash', () => {
    const locationHash = 'key1=a;prefix1:key2=b,c;prefix3:key3=d'
    const parsed = parseLocationHashForPrefixes(['prefix3', 'prefix1'], locationHash)
    // matches nested objects
    sinon.assert.match(parsed[0], { prefix: 'prefix1' })
    sinon.assert.match(parsed[1], { prefix: 'prefix3' })
  })

  test('consume only prefixed parts of a location hash', () => {
    const locationHash = 'key1=a;key2=b,c;prefix1:key3=d'
    const parsed = consumeLocationHashForPrefixes(['prefix1'], locationHash)
    // matches nested objects
    sinon.assert.match(parsed.items[0], { prefix: 'prefix1' })
  })
})

suite('parse location parts', () => {
  test('return a match object', () => {
    const part = 'key1=a'
    const parsed = parsePart(part)
    sinon.assert.match(parsed, { id: 'key1', codes: ['a'] })
  })

  test('urlencoded values are decoded', () => {
    const part = 'key1=test%20%26%20stuff'
    const parsed = parsePart(part)
    sinon.assert.match(parsed, { id: 'key1', codes: ['test & stuff'] })
  })

  test('empty keys', () => {
    const part = '=a'
    const parsed = parsePart(part)
    sinon.assert.match(parsed, { id: '', codes: ['a'] })
  })

  test('no key', () => {
    const part = 'a'
    const parsed = parsePart(part)
    sinon.assert.match(parsed, { id: '', codes: ['a'] })
  })

  test('a match object includes the original string too', () => {
    const part = 'key1=a'
    const parsed = parsePart(part)
    sinon.assert.match(parsed, {
      id: 'key1',
      codes: ['a'],
      origPart: 'key1=a'
    })
  })

  test('multiple parts with ampersand', () => {
    const parts = 'key1=a&key2=b'
    const parsed = parsePartsWithPrefix(parts)
    assert(parsed.length === 2)
    sinon.assert.match(parsed, [{
      prefix: '',
      id: 'key1',
      codes: ['a'],
      origPart: 'key1=a'
    }, {
      prefix: '',
      id: 'key2',
      codes: ['b'],
      origPart: 'key2=b'
    }])
  })

  test('parts with prefix', () => {
    const parts = 'key1=a'
    const parsed = parsePartsWithPrefix(parts, 'prefix1')
    sinon.assert.match(parsed, [{
      prefix: 'prefix1',
      id: 'key1',
      codes: ['a'],
      origPart: 'key1=a'
    }])
  })

  test('same keys parsed as different items', () => {
    const parts = 'key1=a&key1=b'
    const parsed = parsePartsWithPrefix(parts)
    sinon.assert.match(parsed, [{
      prefix: '',
      id: 'key1',
      codes: ['a'],
      origPart: 'key1=a'
    }, {
      prefix: '',
      id: 'key1',
      codes: ['b'],
      origPart: 'key1=b'
    }])
  })
})
