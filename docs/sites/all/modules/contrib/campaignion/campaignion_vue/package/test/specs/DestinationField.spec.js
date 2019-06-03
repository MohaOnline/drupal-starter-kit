import assert from 'assert'
import sinon from 'sinon'

import c from '../../src/components/DestinationField.vue'

afterEach(() => {
  sinon.restore()
})

const dropdownItemHeight = 20

/**
 * Just a POJO dummy for dropdown height calculations.
 * @param {integer} dropdownHeight - The clientHeight of the dropdown list.
 * @return {Object} The fake dropdown element.
 */
function fakeDropdown (dropdownHeight) {
  const items = []
  for (var i = 0; i < 20; i++) {
    items.push({
      offsetTop: dropdownItemHeight * i,
      clientHeight: dropdownItemHeight
    })
  }
  return {
    clientHeight: dropdownHeight,
    scrollTop: 0,
    children: items
  }
}

describe('DestinationField.vue', function () {
  describe('data', function () {
    it('is a function.', function () {
      assert.equal(typeof c.data, 'function')
    })
    it('returns correct init values for attributes.', function () {
      assert.deepEqual(c.data.call({
        value: {value: 'node/2', label: 'My node'}
      }), {
        val: 'My node',
        showDropdown: false,
        current: 0,
        items: []
      })
    })
  })

  describe('computed', function () {
    describe('templateComp', function () {
      const context = {
        labelKey: 'foo'
      }
      it('returns a vue component.', function () {
        const templateComp = c.computed.templateComp.call(context)
        assert.equal(templateComp.template, '<span v-html="highlight(item.foo, value)"></span>')
        assert.deepEqual(templateComp.props, {
          item: {default: null},
          value: String
        })
        assert.equal(typeof templateComp.methods.highlight, 'function')
      })
      describe('methods.highlight', function () {
        const highlight = c.computed.templateComp.call(context).methods.highlight
        it('encloses the phrase in <strong> tags.', function () {
          assert.equal(highlight('Hello world!', 'or'), 'Hello w<strong>or</strong>ld!')
        })
        it('returns the string if the phrase was not found.', function () {
          assert.equal(highlight('Hello world!', 'foo'), 'Hello world!')
        })
      })
    })

    describe('urlMode', function () {
      const urlMode = c.computed.urlMode
      it('returns true if this.val starts with ww', function () {
        assert(urlMode.call({val: 'wwfoo'}) === true)
      })
      it('returns true if this.val starts with ht', function () {
        assert(urlMode.call({val: 'htbla'}) === true)
      })
      it('returns true if this.val starts with a slash', function () {
        assert(urlMode.call({val: '/foo'}) === true)
      })
      it('returns false if this.val starts with something else.', function () {
        assert(urlMode.call({val: 'foo'}) === false)
      })
    })
  })

  describe('watch', function () {
    describe('watcher for this.value', function () {
      it('watches deeply', function () {
        assert(c.watch.value.deep === true)
      })
      it('updates this.val.label', function () {
        const context = {val: 'foo'}
        c.watch.value.handler.call(context, {value: 'some value', label: 'bar'})
        assert(context.val === 'bar')
      })
      // TODO test if branch?
    })
  })

  describe('methods', function () {
    describe('focus', function () {
      it('calls this.update if this.val is falsey and this.showDropdownOnFocus is truthy.', function () {
        const context = {
          val: '',
          showDropdownOnFocus: true,
          update: sinon.fake()
        }
        c.methods.focus.call(context)
        assert(context.update.callCount === 1)
      })
    })

    describe('update', function () {
      const update = c.methods.update
      var context, clock

      beforeEach(function () {
        // State: a suggestion is selected.
        context = {
          value: {
            value: 'node/2',
            label: 'My node'
          },
          val: 'My node',
          urlMode: false,
          delay: c.props.delay.default,
          $emit: sinon.spy(),
          reset: sinon.spy(),
          query: sinon.spy()
        }
        clock = sinon.useFakeTimers()
      })

      afterEach(function () {
        clock.restore()
      })

      it('resets this.val if this.value.value and this.value.label differ.', function () {
        update.call(context)
        assert(context.val === '')
      })
      it('emits an input event, passing the resetted value.', function () {
        update.call(context)
        assert(context.$emit.calledOnce)
        assert.equal(context.$emit.firstCall.args[0], 'input')
        assert.deepEqual(context.$emit.firstCall.args[1], {
          value: '',
          label: ''
        })
      })
      it('calls this.reset', function () {
        update.call(context)
        assert(context.reset.calledOnce)
      })
      it('calls this.query after the delay time.', function () {
        update.call(context)

        clock.tick(context.delay - 1)
        assert(context.query.notCalled)

        clock.tick(1)
        assert(context.query.calledOnce)
      })
      it('doesn’t call this.query if this.val changed during the delay time.', function () {
        update.call(context)
        context.val = 'foo'

        clock.tick(context.delay)
        assert(context.query.notCalled)
      })
      it('doesn’t call this.query if this.urlMode is true.', function () {
        context.urlMode = true
        update.call(context)

        clock.tick(context.delay)
        assert(context.query.notCalled)
      })
    })

    describe('query', function () {
      const query = c.methods.query
      var context
      const responseData = {
        myItems: [
          {value: '1', label: 'One'},
          {value: '2', label: 'Two'},
          {value: '3', label: 'Three'},
          {value: '4', label: 'Four'}
        ]
      }

      beforeEach(function () {
        context = {
          items: [],
          dataKey: 'myItems',
          queryParam: 'search',
          url: 'http://foo.bar.com',
          headers: {'Foo': 'bar'},
          count: 3,
          getData: sinon.spy(function () {
            return Promise.resolve({
              config: {
                url: 'https://some.url.com?search=bar'
              },
              data: responseData
            })
          })
        }
      })

      it('calls this.getData', function () {
        context.val = 'bar'
        query.call(context)
        assert(context.getData.calledOnce)
        assert.deepEqual(context.getData.firstCall.args[0], {
          url: 'http://foo.bar.com',
          headers: {'Foo': 'bar'},
          queryParam: context.queryParam,
          queryString: 'bar'
        })
      })
      it('encodes special characters in the query', function () {
        context.val = '&?'
        query.call(context)
        assert(context.getData.calledOnce)
        assert.equal(context.getData.firstCall.args[0].queryString, '%26%3F')
      })
      it('sets this.items if the query string matches this.val', function (done) {
        context.val = 'bar' // The spy returns the response for the search term 'bar'.
        query.call(context)
        setTimeout(() => {
          assert.equal(context.items.length, context.count)
          assert.deepEqual(context.items, responseData.myItems.filter((val, idx) => idx < 3))
          done()
        }, 0)
      })
      it('throws the response away if the response query string doesn’t match this.val', function (done) {
        context.val = 'barista' // The spy returns the response for the search term 'bar'.
        query.call(context)
        setTimeout(() => {
          assert.equal(context.items.length, 0)
          done()
        }, 0)
      })
      it('shows the dropdown if there are items', function (done) {
        context.val = 'bar' // The spy returns the response for the search term 'bar'.
        query.call(context)
        setTimeout(() => {
          assert.equal(context.items.length, context.count)
          assert.equal(context.showDropdown, true)
          done()
        }, 0)
      })
    })

    describe('reset', function () {
      it('clears the items, resets the current item and hides the dropdown.', function () {
        const context = {
          items: [
            {value: '1', label: 'One'},
            {value: '2', label: 'Two'}
          ],
          current: 1,
          showDropdown: true
        }
        c.methods.reset.call(context)
        assert.equal(context.items.length, 0)
        assert.equal(context.current, 0)
        assert.equal(context.showDropdown, false)
      })
    })

    describe('setActive', function () {
      it('sets the current item.', function () {
        const context = {current: 0}
        c.methods.setActive.call(context, 5)
        assert.equal(context.current, 5)
      })
    })

    describe('isActive', function () {
      const context = {current: 7}
      it('returns true if the specified item is active.', function () {
        assert.equal(c.methods.isActive.call(context, 7), true)
      })
      it('returns false if the specified item is not active.', function () {
        assert.equal(c.methods.isActive.call(context, 5), false)
      })
    })

    describe('hit', function () {
      const hit = c.methods.hit
      var context

      beforeEach(function () {
        context = {
          items: [
            {val: '1', text: 'One'},
            {val: '2', text: 'Two'}
          ],
          val: 'foo', // current value of the input
          current: 1, // the second dropdown item is active
          showDropdown: true,
          valueKey: 'val',
          labelKey: 'text',
          $emit: sinon.fake(),
          reset: sinon.fake()
        }
      })

      it('sets this.val to the value of the current item.', function () {
        hit.call(context)
        assert.equal(context.val, 'Two')
      })
      it('emits an input event for the parent component.', function () {
        hit.call(context)
        assert(context.$emit.calledOnce)
        assert.equal(context.$emit.firstCall.args[0], 'input')
        assert.deepEqual(context.$emit.firstCall.args[1], {
          value: '2',
          label: 'Two'
        })
      })
      it('calls this.reset', function () {
        hit.call(context)
        assert(context.reset.calledOnce)
      })
      it('does nothing if the dropdown is closed.', function () {
        context.showDropdown = false
        hit.call(context)
        assert.equal(context.val, 'foo')
        assert(context.$emit.notCalled)
        assert(context.reset.notCalled)
      })
    })

    describe('up', function () {
      const up = c.methods.up
      var context

      beforeEach(function () {
        context = {
          showDropdown: true,
          items: new Array(20),
          current: 3,
          $refs: {
            dropdown: fakeDropdown(5 * dropdownItemHeight)
          }
        }
      })

      it('decrements this.current', function () {
        up.call(context)
        assert.equal(context.current, 2)
      })
      it('doesn’t decrement if the beginning of the array is reached.', function () {
        context.current = 0
        up.call(context)
        assert.equal(context.current, 0)
      })
      it('scrolls the now active item into view.', function () {
        // Scroll the third item partly out of view:
        context.$refs.dropdown.scrollTop = 2 * dropdownItemHeight + 1
        up.call(context)
        // Assert that the third item is completely visible:
        assert(context.$refs.dropdown.scrollTop < 2 * dropdownItemHeight)
      })
      it('doesn’t scroll if the item is in view.', function () {
        context.$refs.dropdown.scrollTop = 2 * dropdownItemHeight
        up.call(context)
        assert.equal(context.$refs.dropdown.scrollTop, 2 * dropdownItemHeight)
      })
      it('doesn’t decrement if the dropdown is closed.', function () {
        context.showDropdown = false
        up.call(context)
        assert.equal(context.current, 3)
      })
    })

    describe('down', function () {
      const down = c.methods.down
      var context

      beforeEach(function () {
        context = {
          showDropdown: true,
          items: new Array(20),
          current: 4,
          $refs: {
            dropdown: fakeDropdown(5 * dropdownItemHeight)
          }
        }
      })

      it('increments this.current', function () {
        down.call(context)
        assert.equal(context.current, 5)
      })
      it('doesn’t increment if the end of the array is reached.', function () {
        context.current = context.items.length - 1
        down.call(context)
        assert.equal(context.current, context.items.length - 1)
      })
      it('scrolls the now active item into view.', function () {
        // Scroll the sixth item partly out of view:
        context.$refs.dropdown.scrollTop = dropdownItemHeight - 1
        down.call(context)
        // Assert that the sixth item is completely visible:
        assert(context.$refs.dropdown.scrollTop >= dropdownItemHeight)
      })
      it('doesn’t scroll if the item is in view.', function () {
        context.$refs.dropdown.scrollTop = dropdownItemHeight
        down.call(context)
        assert.equal(context.$refs.dropdown.scrollTop, dropdownItemHeight)
      })
      it('doesn’t decrement if the dropdown is closed.', function () {
        context.showDropdown = false
        down.call(context)
        assert.equal(context.current, 4)
      })
    })

    describe('esc', function () {
      const esc = c.methods.esc
      var context, e

      beforeEach(function () {
        context = {showDropdown: true}
        e = {stopPropagation: sinon.fake()}
      })

      it('stops event propagation if the dropdown is open.', function () {
        esc.call(context, e)
        assert(e.stopPropagation.calledOnce)
      })
      it('closes the dropdown.', function () {
        esc.call(context, e)
        assert.equal(context.showDropdown, false)
      })
      it('lets the event bubble if the dropdown is closed.', function () {
        context.showDropdown = false
        esc.call(context, e)
        assert(e.stopPropagation.notCalled)
      })
    })
  })
})
