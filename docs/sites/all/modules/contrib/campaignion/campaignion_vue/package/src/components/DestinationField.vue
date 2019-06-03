<docs>
DestinationField component, based on https://github.com/yuche/vue-strap/blob/master/src/Typeahead.vue
Asks a server for results based on the query string entered by the user and displays
a dropdown with the result labels for the user to choose from. Every result has a
value and a label. Apart from suggestions the user can enter custom data, then the
component sets both value and label to this string.
You can use this component with `v-model` to get/set its value.
</docs>

<template lang="html">
  <div :class="{
     'typeahead': true,
     'open': showDropdown
    }">
    <input type="text" class="field-input typeahead-input"
      ref="input"
      :placeholder="placeholder"
      :maxlength="maxlength"
      autocomplete="off"
      v-model="val"
      @input="update"
      @focus="focus"
      @keydown.up.prevent="up"
      @keydown.down.prevent="down"
      @keyup.enter.stop.prevent= "hit"
      @keydown.esc="esc"
      @blur="showDropdown = false"
    />
    <ul v-if="showDropdown" ref="dropdown" class="dropdown-menu">
      <li v-for="(item, index) in items" :class="{'active': isActive(index)}">
        <a class="dropdown-item" @mousedown.stop.prevent="hit" @mousemove="setActive(index)">
          <component :is="templateComp" :item="item" :value="val"></component>
        </a>
      </li>
    </ul>
  </div>
</template>

<script>
import {fixedEncodeURIComponent, escapeRegExp} from '../utils'

const _DELAY_ = 200

export default {
  props: {
    value: {             /** The component’s value. */
      type: Object,
      default () {
        // When the user types a custom value into the field, value and label are
        // identical. They only differ if a suggestion has been selected.
        return {
          value: '',
          label: ''
        }
      }
    },
    count: {             /** Max number of items that should be displayed in the dropdown. */
      type: Number,
      default: 8
    },
    template: String,    /** Used to render a suggestion. */
    dataKey: {           /** The key of the suggestions array in the response JSON. If not set, the response itself is expected to by an array of suggestions. */
      type: String,
      default: null
    },
    labelKey: {          /** The key indicating the label in a suggestion object. */
      type: String,
      default: 'label'
    },
    valueKey: {          /** The key indicating the value in a suggestion object. */
      type: String,
      default: 'value'
    },
    placeholder: String, /** The input’s placeholder text. */
    maxlength: {
      type: Number,
      default: -1
    },
    delay: {             /** Request data from the server after the user stopped typing for this amount of time (milliseconds). */
      type: Number,
      default: _DELAY_
    },
    getData: {           /** Function that GETs data from the server. Should return a promise. */
      type: Function,
      required: true
    },
    url: {               /** The URL to query. */
      type: String,
      required: true
    },
    headers: {           /** HTTP headers to send with the request. */
      type: Object,
      default () {
        return {}
      }
    },
    queryParam: {       /** Query parameter for the search term. */
      type: String,
      default: 's'
    },
    showDropdownOnFocus: { /** Display the dropdown immediately after the user focused the input. */
      type: Boolean,
      default: false
    }
  },

  data () {
    return {
      val: this.value.label, /** {string} Internal value variable bound to the input element. */
      showDropdown: false,   /** {boolean} The dropdown’s visibility. */
      current: 0,            /** {integer} The index of the currently highlighted item (suggestion). */
      items: []              /** {Object[]} Collection of suggestions, each having a value and a label property. */
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
        template: typeof this.template === 'string' ? '<span v-html="this.template"></span>' : '<span v-html="highlight(item.' + this.labelKey + ', value)"></span>',
        props: {
          item: {default: null},
          value: String
        },
        methods: {
          highlight (string, phrase) {
            return (string && phrase && string.replace(new RegExp('(' + escapeRegExp(phrase) + ')', 'gi'), '<strong>$1</strong>')) || string
          }
        }
      }
    },

    /**
     * Guess whether the user entered a url or a path.
     * @return {boolean} `true` if the user probably entered a url or a path.
     */
    urlMode () {
      return !!this.val.match(/^(ww|ht|\/)/i)
    }
  },

  watch: {
    // Update internal data when changes are caused by the parent component:
    value: {
      handler (val) {
        if (this.val !== val.label) {
          this.val = val.label
        }
      },
      deep: true
    }
  },

  methods: {
    /**
     * Handle the input getting the focus.
     */
    focus () {
      if (!this.val && this.showDropdownOnFocus) {
        // Show suggestions when field is blank.
        this.update()
      }
    },

    /**
     * Update the list of suggestions.
     */
    update () {
      // If a suggestion has been selected, value and label are not identical.
      // We clear the field if a suggestion had been selected, so the user gets
      // feedback that they deselected the suggestion by typing something else.
      if (this.value.label !== this.value.value) {
        this.val = ''
      }
      // Inform the parent component about the changes.
      this.$emit('input', {
        value: this.val,
        label: this.val
      })
      // Close the dropdown and clear the list of suggestions.
      this.reset()
      // No need to query anything if the user entered a url.
      if (this.urlMode) {
        return
      }
      var lastVal = this.val
      setTimeout(() => {
        // Only query if the value didn’t change during the delay period.
        if (this.val === lastVal) {
          this.query()
        }
      }, this.delay)
    },

    /**
     * Request data from the server and process the response.
     */
    query () {
      this.getData({
        url: this.url,
        headers: this.headers,
        queryParam: this.queryParam,
        queryString: fixedEncodeURIComponent(this.val)
      }).then(response => {
        // get the search term from the url
        const re = new RegExp('[&|?]' + this.queryParam + '=([^&]*)')
        var searchVal
        try {
          searchVal = response.config.url.match(re)[1]
        } catch (error) {
          return
        }
        // Throw the response away if the typeahead value has changed in the meantime.
        if (fixedEncodeURIComponent(this.val) !== searchVal) return

        var data = response.data
        this.items = (this.dataKey ? data[this.dataKey] : data).slice(0, this.count)
        this.showDropdown = this.items.length > 0
      })
    },

    /**
     * Close the dropdown and clear the list of suggestions.
     */
    reset () {
      this.items = []
      this.current = 0
      this.showDropdown = false
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
     */
    hit () {
      if (this.showDropdown) {
        this.val = this.items[this.current][this.labelKey]
        this.$emit('input', {
          value: this.items[this.current][this.valueKey],
          label: this.items[this.current][this.labelKey]
        })
        this.reset()
      }
    },

    /**
     * Handle keydowns of the 'up' arrow key.
     * Move the active item up and scroll it into view, if necessary.
     */
    up () {
      if (!this.showDropdown) {
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
     * Move the active item down and scroll it into view, if necessary.
     */
    down () {
      if (!this.showDropdown) {
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
        e.stopPropagation()
        this.showDropdown = false
      }
    }
  }
}
</script>

<style lang="css">
.typeahead {
  display: inline-block;
  position: relative;
}

.typeahead .dropdown-menu {
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
}

.typeahead.open .dropdown-menu {
  display: block;
}

.typeahead .dropdown-menu > li {
  width: 100%;
}

.typeahead .dropdown-menu > li.active {
  color: #fff;
  background-color: #aaa;
}

.typeahead .dropdown-menu > li > a {
  display: inline-block;
  width: 100%;
  cursor: pointer;
}
</style>
