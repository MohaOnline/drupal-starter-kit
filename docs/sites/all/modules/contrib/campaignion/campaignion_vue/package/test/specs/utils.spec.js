import assert from 'assert'

import utils from '../../src/utils.js'

describe('fixedEncodeURIComponent', function () {
  it('encodes common special characters covered by encodeURIComponent.', function () {
    const str = utils.fixedEncodeURIComponent('^°"§ $%&/=?´`[]{}²³+#,;:<>|äöüß')
    assert.equal(str, '%5E%C2%B0%22%C2%A7%20%24%25%26%2F%3D%3F%C2%B4%60%5B%5D%7B%7D%C2%B2%C2%B3%2B%23%2C%3B%3A%3C%3E%7C%C3%A4%C3%B6%C3%BC%C3%9F')
  })
  it('encodes additional special characters.', function () {
    const str = utils.fixedEncodeURIComponent('!\'()*')
    assert.equal(str.toLowerCase(), '%21%27%28%29%2a')
  })
  it('doesn’t encode a-z, A-Z and 0-9.', function () {
    const str = utils.fixedEncodeURIComponent('abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789')
    assert.equal(str, 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789')
  })
  it('doesn’t encode allowed special characters.', function () {
    const str = utils.fixedEncodeURIComponent('-_.~')
    assert.equal(str, '-_.~')
  })
})

describe('paramReadyUrl', function () {
  it('appends a ? if there is no parameter', function () {
    assert.equal(utils.paramReadyUrl('/foo/bar'), '/foo/bar?')
  })
  it('deals with trailing slashes', function () {
    assert.equal(utils.paramReadyUrl('/foo/bar/'), '/foo/bar?')
  })
  it('prevents double question marks', function () {
    assert.equal(utils.paramReadyUrl('/foo/bar?'), '/foo/bar?')
    assert.equal(utils.paramReadyUrl('/foo/bar/?'), '/foo/bar?')
  })
  it('appends a & if there is one', function () {
    assert.equal(utils.paramReadyUrl('/foo/bar?color=red'), '/foo/bar?color=red&')
  })
  it('appends a & if there are two', function () {
    assert.equal(utils.paramReadyUrl('/foo/bar?color=red&x=y'), '/foo/bar?color=red&x=y&')
  })
})

describe('escapeRegExp', function () {
  it('escapes characters that mean something in regular expressions.', function () {
    const str = utils.escapeRegExp('-[]/{}()*+?.\\^$|')
    assert.equal(str, '\\-\\[\\]\\/\\{\\}\\(\\)\\*\\+\\?\\.\\\\\\^\\$\\|') // eslint-disable-line no-useless-escape
  })
})
