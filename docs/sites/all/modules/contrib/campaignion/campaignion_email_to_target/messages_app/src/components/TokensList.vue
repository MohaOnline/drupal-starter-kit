<docs>
TokensList component.
Displays an expandable table containing the token categories and their corresponding tokens.
</docs>

<template>
  <section class="tokens-list">
      <table v-for="(cat, index) in tokenCategories" class="table table-sm">
        <thead>
          <tr class="token-category">
            <th colspan="2">
              <a href="#" @mousedown.prevent="toggle(index)" @click.prevent>
                <span class="category-expand">{{ expanded[index] ? '–' : '+' }}</span>
                <strong class="category-title" v-html="cat.title"></strong>
              </a>
            </th>
            <th class="category-description" v-html="cat.description"></th>
          </tr>
        </thead>
        <tbody v-if="cat.tokens.length && expanded[index]">
          <tr v-for="token in cat.tokens">
            <td class="token-title" v-html="token.title"></td>
            <td class="token-token">
              <a href="#" @mousedown.prevent="insert(token.token)" @click.prevent title="Insert token at cursor position">{{ token.token }}</a>
            </td>
            <td class="token-description" v-html="token.description"></td>
          </tr>
        </tbody>
      </table>
  </section>
</template>

<script>
import Vue from 'vue'
import {dispatch} from '@/utils'

export default {
  props: {
    tokenCategories: Array /** {Object[]} Collection of categories with a `title`, a `description` and a collection of `tokens`. The `tokens` each have a `title`, a `description` and a `token`. */
  },
  data () {
    return {
      expanded: [] /** {boolean[]} Array of booleans, being true if the token category with the same index is expanded. */
    }
  },
  methods: {
    /**
     * Toggle expanded state of a token category.
     * @param {integer} idx - The index of the token category to toggle.
     */
    toggle (idx) {
      Vue.set(this.expanded, idx, !this.expanded[idx])
    },

    /**
     * Call insertAtCaret for the element with the focus.
     * @param {string} token - The token to insert in the focused element.
     */
    insert (token) {
      if (document.activeElement.hasAttribute('data-token-insertable')) {
        this.insertAtCaret(document.activeElement, token)
      }
    },

    /**
     * Insert a string in a textarea or input at the cursor position.
     * @param {HTMLElement} txtarea - The input or textarea.
     * @param {string} text - The string to insert.
     */
    insertAtCaret (txtarea, text) {
      var strPos = 0
      var range
      const scrollPos = txtarea.scrollTop
      const br = ((txtarea.selectionStart || txtarea.selectionStart === '0')
        ? 'ff' : (document.selection ? 'ie' : false))
      if (br === 'ie') {
        txtarea.focus()
        range = document.selection.createRange()
        range.moveStart('character', -txtarea.value.length)
        strPos = range.text.length
      } else if (br === 'ff') {
        strPos = txtarea.selectionStart
      }

      var front = (txtarea.value).substring(0, strPos)
      var back = (txtarea.value).substring(strPos, txtarea.value.length)
      txtarea.value = front + text + back
      strPos = strPos + text.length
      if (br === 'ie') {
        txtarea.focus()
        range = document.selection.createRange()
        range.moveStart('character', -txtarea.value.length)
        range.moveStart('character', strPos)
        range.moveEnd('character', 0)
        range.select()
      } else if (br === 'ff') {
        txtarea.selectionStart = strPos
        txtarea.selectionEnd = strPos
        txtarea.focus()
      }
      txtarea.scrollTop = scrollPos

      // Trigger input for the MessageEditor component to update.
      dispatch(txtarea, 'input')
    }
  }
}
</script>

<style lang="scss">
.e2tmw th {
  text-align: left;
}
</style>
