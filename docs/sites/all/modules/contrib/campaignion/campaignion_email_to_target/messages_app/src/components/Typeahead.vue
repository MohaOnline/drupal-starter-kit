<docs>
Typeahead component, based on https://github.com/yuche/vue-strap/blob/master/src/Typeahead.vue
Asks a server for results based on the query string entered by the user, or
searches an array, if you pass the `data` prop. Displays a dropdown with the
results for the user to choose from.
Per default, you can use this component with `v-model` to get/set its value.
</docs>

<template>
  <div style="position: relative"
    :class="{
     'typeahead': true,
     'open': showDropdown
    }"
    >
    <input type="text" class="field-input typeahead-input"
      ref="input"
      :placeholder="placeholder"
      autocomplete="off"
      v-model="val"
      @input="update"
      @focus="showCachedOrUpdate"
      @keydown.up="up"
      @keydown.down="down"
      @keyup.enter= "hit"
      @keydown.esc="esc"
      @blur="showDropdown = false"
    />
    <ul v-if="showDropdown" @scroll="scroll" ref="dropdown" class="dropdown-menu">
      <li v-for="(item, index) in items" :class="{'active': isActive(index)}">
        <a class="dropdown-item" @mousedown.prevent="hit" @mousemove="setActive(index)">
          <component :is="templateComp" :item="item" :value="val"></component>
        </a>
      </li>
    </ul>
  </div>
</template>

<script>
const _DELAY_ = 200

/**
 * Prepare a url for appending a GET param key-value pair.
 * @param {string} url - The url to prepare.
 * @return {string} The url with either a ? or a & at the end.
 */
function paramReadyUrl (url) {
  if (!url.match(/\?[^=]+=[^&]*/)) {
    // there’s no parameter. replace trailing ? or / or /? with ?
    return url.replace(/[/?]$|(?:\/)\?$/, '') + '?'
  } else {
    // parameter present in the string. ensure trailing &
    return url.replace(/[&]$/, '') + '&'
  }
}

/**
 * Comply to RFC 3986 when encoding URI components.
 * Encode also !, ', (, ) and *.
 * @param {string} str - The URI component to encode.
 * @return {string} The encoded URI component.
 */
function fixedEncodeURIComponent (str) {
  return encodeURIComponent(str).replace(/[!'()*]/g, c => '%' + c.charCodeAt(0).toString(16))
}

export default {

  created () {
    this.items = this.primitiveData
  },

  props: {
    value: {             /** The component’s value. */
      type: String,
      default: ''
    },
    data: Array,         /** An array of strings to search. Pass this if you want to use the component client-side. */
    count: {             /** Number of items that should be loaded at once. */
      type: Number,
      default: 8
    },
    async: String,       /** An HTTP URL for asynchronous suggestions. Expected to return a JSON object. */
    headers: {           /** HTTP headers to send with the request. */
      type: Object,
      default: {}
    },
    template: String,    /** Used to render suggestion. */
    dataKey: {           /** The key of the suggestions array in the response JSON. If not set, the response itself is expected to by an array of suggestions. */
      type: String,
      default: null
    },
    matchCase: {         /** Match the case when filtering? Client-side only. */
      type: Boolean,
      default: false
    },
    matchStart: {        /** Only suggest items starting with the query string? Client-side only. */
      type: Boolean,
      default: false
    },
    onHit: {             /** Function executed when the user selects a suggestion from the dropdown. */
      type: Function,
      default (item) {
        if (item) {
          this.reset()
          this.$emit('input', item)
        }
      }
    },
    placeholder: String, /** The input’s placeholder text. */
    delay: {             /** Request data from the server after the user stopped typing for this amount of time (milliseconds). */
      type: Number,
      default: _DELAY_
    },
    showDropdownOnFocus: { /** Display the dropdown immediately after the user focused the input or only after typing. */
      type: Boolean,
      default: false
    },
    lazyLoad: {          /** Don’t load huge amounts of data at once. */
      type: Boolean,
      default: false
    },
    pageMode: {          /** Define whether the API uses paging or offset. Allowed values: `page`  or `offset`. */
      type: String,
      default: 'page'
    },
    pageParam: {         /** Query parameter for page or offset. */
      type: String,
      default: 'p'
    },
    firstPage: {         /** The page that pagination starts with. Allowed values: `0` or `1`. */
      type: Number,
      default: 1
    },
    searchParam: {       /** Query parameter for the search term. */
      type: String,
      default: 's'
    },
    countParam: {        /** Query parameter for the number of items or page size. */
      type: String,
      default: 'n'
    }
  },

  data () {
    return {
      val: this.value,         /** {string} Internal value variable bound to the input element. */
      cachedQuery: null,       /** {(string|null)} The string that has been queried in the last request. */
      showDropdown: false,     /** {boolean} The dropdown’s visibility. */
      current: 0,              /** {integer} The index of the currently highlighted item (suggestion). */
      items: [],               /** {string[]} The suggestions. */
      lastLoadedPage: 0,       /** {integer} The number of the page that has been loaded last (in lazyLoad mode). */
      moreItemsLoadable: true, /** {boolean} Is there another page to request? (in lazyLoad mode) */
      queryOnTheWay: false     /** {boolean} Flag indicating that a request has been made and we’re still waiting for a response. */
    }
  },

  computed: {
    /**
     * A vue component that uses the `template` prop and offers a `highlight` method
     * for the template to use.
     * @return {Object} The templateComp component.
     */
    templateComp () {
      return {
        template: typeof this.template === 'string' ? '<span v-html="this.template"></span>' : '<span v-html="highlight(item, value)"></span>',
        props: {
          item: {default: null},
          value: String
        },
        methods: {
          highlight (string, phrase) {
            return (string && phrase && string.replace(new RegExp('(' + phrase + ')', 'gi'), '<strong>$1</strong>')) || string
          }
        }
      }
    },

    /**
     * Client-side mode: return a filtered array of items.
     * HTTP mode: return an empty array.
     * @return {string[]} An array of items matching the query value.
     */
    primitiveData () {
      if (this.data) {
        return this.data.filter(value => {
          value = this.matchCase ? value : value.toLowerCase()
          var query = this.matchCase ? this.value : this.value.toLowerCase()
          return this.matchStart ? value.indexOf(query) === 0 : value.indexOf(query) !== -1
        }).slice(0, this.count)
      } else {
        return []
      }
    },

    /**
     * Prepare the url for appending GET params.
     * @return {string} The API’s url, ending with a ? or &.
     */
    url () {
      return paramReadyUrl(this.async)
    },

    /**
     * Return only allowed values for pageMode. `page` is the default.
     * @return {string} `page` or `offset`, depending on the `pageMode` prop.
     */
    coercedPageMode () {
      return (this.pageMode === 'page' || this.pageMode === 'offset') ? this.pageMode : 'page'
    },

    /**
     * Return only allowed values for firstPage. `1` is the default.
     * @return {integer} `0` or `1`, depending on the `firstPage` prop.
     */
    coercedFirstPage () {
      return (this.firstPage === 0 || this.firstPage === '0') ? 0 : 1
    }
  },

  watch: {
    // Inform the parent component about changes:
    val (val, old) {
      this.$emit('input', val)
    },
    // Update internal data when changes are caused by the parent component:
    value (val) {
      if (this.val !== val) { this.val = val }
    }
  },

  methods: {
    /**
     * Update the list of suggestions.
     * In HTTP mode, trigger a request after the delay.
     * @return {(undefined|false)} TODO: probably returning undefined is enough.
     */
    update () {
      // showDropdownOnFocus means: show the dropdown even if the input is empty.
      // If this option isn’t checked and the input is empty, clear the suggestions list.
      if (!this.showDropdownOnFocus && !this.val) {
        this.reset()
        return false // TODO: probably returning undefined is enough.
      }
      // Client-side mode:
      if (this.data) {
        this.items = this.primitiveData
        this.showDropdown = this.items.length > 0
      }
      // HTTP mode:
      if (this.async) { // TODO: elseif (this.async) would let the data prop determine the mode.
        this.reset()
        var lastVal = this.val
        setTimeout(() => {
          // Only query if the value didn’t change during the delay period.
          if (this.val === lastVal) this.query()
        }, this.delay)
      }
    },

    /**
     * Request data from the server and process the response.
     */
    query () {
      var url = this.url + this.searchParam + '=' + fixedEncodeURIComponent(this.val) + '&' + this.countParam + '=' + this.count
      if (this.lazyLoad) url += '&' + this.pageParam + '=' + (this.coercedPageMode === 'page' ? this.lastLoadedPage + this.coercedFirstPage : this.lastLoadedPage * this.count)
      this.$http.get(url, {
        headers: this.headers
      }).then(response => {
        // get the search term from the url
        const re = new RegExp('[&|?]search=([^&]*)')
        const searchVal = response.config.url.match(re)[1]
        // throw the response away if the typeahead value has changed in the meantime
        if (fixedEncodeURIComponent(this.val) !== searchVal) return

        var data = response.data
        if (this.lazyLoad) {
          var newItems = this.dataKey ? data[this.dataKey] : data
          if (newItems.length) {
            this.items.push(...newItems)
            this.lastLoadedPage++
          }
          this.queryOnTheWay = false
          this.moreItemsLoadable = newItems.length === this.count
        } else {
          this.items = (this.dataKey ? data[this.dataKey] : data).slice(0, this.count)
        }
        this.cachedQuery = searchVal
        this.showDropdown = this.items.length > 0
      })
    },

    /**
     * If the dropdown should be shown on focus, show it – either with the items
     * that are still there or with new ones, depending on whether the list is
     * still appropriate for the current query value.
     */
    showCachedOrUpdate () {
      if (!this.showDropdownOnFocus) {
        return
      }
      if (this.items.length && this.val === this.cachedQuery) {
        this.showDropdown = true
      } else {
        this.update()
      }
    },

    /**
     * Close the dropdown and clear the list of suggestions.
     * Reset variables for a new request.
     */
    reset () {
      this.items = []
      this.cachedQuery = null
      this.current = 0
      this.lastLoadedPage = 0
      this.showDropdown = false
      this.moreItemsLoadable = true
    },

    /**
     * Set `this.current` to the index of item that is being hovered or selected
     * with the cursor keys.
     * @param {integer} index - The active item’s index.
     */
    setActive (index) {
      this.current = index
    },

    /**
     * Check whether the item with a given index is active.
     * @param {integer} index - The index of the item to check.
     * @return {boolean} Is this the item that is currently active?
     */
    isActive (index) {
      return this.current === index
    },

    /**
     * Handle Enter keyups and mousedowns on a suggestion.
     * @param {Event} e - The original event.
     */
    hit (e) {
      if (this.showDropdown) {
        e.preventDefault() // TODO: use Vue event modifiers?
        e.stopPropagation()
        this.onHit(this.items[this.current], this)
      }
    },

    /**
     * Handle keydowns of the 'up' arrow key.
     * Show the dropdown if it’s closed.
     * Move the active item up and scroll it into view, if necessary.
     * @param {Event} e - The original event.
     */
    up (e) {
      e.preventDefault() // TODO: use the .prevent modifier?
      if (!this.showDropdown) {
        this.showCachedOrUpdate()
        return
      }
      if (this.current > 0) {
        this.current--
        const d = this.$refs.dropdown
        const i = d.children[this.current]
        if (i.offsetTop < d.scrollTop) {
          d.scrollTop -= i.clientHeight
        }
      }
    },

    /**
     * Handle keydowns of the 'down' arrow key.
     * Show the dropdown if it’s closed.
     * Move the active item down and scroll it into view, if necessary.
     * @param {Event} e - The original event.
     */
    down (e) {
      e.preventDefault() // TODO: use the .prevent modifier?
      if (!this.showDropdown) {
        this.showCachedOrUpdate()
        return
      }
      if (this.current < this.items.length - 1) {
        this.current++
        const d = this.$refs.dropdown
        const i = d.children[this.current]
        if (i.offsetTop + i.clientHeight > d.scrollTop + d.clientHeight) {
          d.scrollTop += i.clientHeight
        }
      }
    },

    /**
     * Handle esc key keydown events.
     * @param {Event} e - The original event.
     */
    esc (e) {
      if (this.showDropdown) {
        e.stopPropagation() // TODO: use the .stop modifier?
        this.showDropdown = false
      }
    },

    /**
     * Lazy-load more items on scroll.
     */
    scroll () {
      if (!this.lazyLoad || !this.moreItemsLoadable) return
      if (this.$refs.dropdown.scrollHeight - this.$refs.dropdown.scrollTop - 30 < this.$refs.dropdown.clientHeight) {
        if (!this.queryOnTheWay && this.items.length) this.query()
        this.queryOnTheWay = true
      }
    }
  }
}
</script>

<style lang="scss">
.typeahead {
  display: inline-block;

  .dropdown-menu {
    display: none;
    position: absolute;
    top: 100%;
    left: 0;
    min-width: 100%;
    max-height: 12rem;
    overflow-y: auto;
    list-style: none;
    margin: 0;
    padding: 0;
    z-index: 2000;

    & > li {
      width: 100%;

      &.active {
        color: #fff;
        background-color: #aaa;
      }
    }

    & > li > a {
      display: inline-block;
      width: 100%;
      cursor: pointer;
    }
  }

  &.open .dropdown-menu {
    display: block;
  }
}
</style>
