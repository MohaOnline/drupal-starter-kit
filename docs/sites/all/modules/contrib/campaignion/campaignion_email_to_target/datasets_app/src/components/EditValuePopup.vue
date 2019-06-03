<template lang="html">
  <div v-if="editValue" :class="{
    'dsa-edit-value-popup': true,
    'dsa-has-error': showError && !valid
  }">
    <div v-if="showError && !valid" class="dsa-edit-value-error">
      {{ errorMessage }}
    </div>
    <div v-else class="dsa-edit-value-label">
      {{ label }}
    </div>
    <input type="text" v-model="value" @keydown.enter.stop="save" @keydown.esc.stop="cancel" ref="input" class="dsa-edit-value-input field-input"/>
    <el-button type="button" @click="save" class="dsa-edit-value-save">{{ text('save') }}</el-button>
    <el-button type="button" @click="cancel" class="dsa-edit-value-cancel">{{ text('cancel') }}</el-button>
  </div>
</template>

<script>
import {mapState} from 'vuex'
import Popper from 'popper.js/dist/umd/popper'
import {dispatch} from '@/utils'
import {find} from 'lodash'

var popper = {}

export default {
  data: function () {
    return {
      value: '',                 /** {string} The value of the editing field. */
      validator: new RegExp(''), /** {RegExp} The regular expression to test the value against, depeding on the column being edited. */
      showError: false           /** {boolean} Visibility of the validation error message. */
    }
  },

  computed: {
    /** @return {boolean} Does the editing field’s value differ from the selected cell’s value? */
    changed () {
      return !!this.editValue && (this.value !== this.editValue.row[this.editValue.col])
    },

    /** @return {boolean} Does the editing field’s value pass the validation for the edited column? */
    valid () {
      if (typeof this.maxFieldLengths[this.editValue.col] !== 'undefined') {
        const maxlength = this.maxFieldLengths[this.editValue.col]
        return this.validator.test(this.value) && this.value.length <= maxlength
      } else {
        return this.validator.test(this.value)
      }
    },

    /** @return {string} A specific error message if maxlength is exceeded, or else a generic one. */
    errorMessage () {
      const maxlength = this.maxFieldLengths[this.editValue.col]
      if (typeof maxlength !== 'undefined' && this.value.length > maxlength) {
        return Drupal.t('Make sure that this field is not longer than @maxlength characters.', {'@maxlength': maxlength})
      } else {
        return Drupal.t('Please enter a valid @fieldName', {'@fieldName': this.label.toLowerCase()})
      }
    },

    /** @return {string} The title of the edited column (or if falsey, the column key). */
    label () {
      return find(this.columns, {key: this.editValue.col}).title || this.editValue.col
    },

    ...mapState([
      'editValue',      /** {(Object|null)} Object describing the table cell being edited. */
      'columns',        /** {Object[]} Array of objects describing each column: {key: 'foo', title: 'Foo', description: 'The foo column.'} */
      'validations',    /** {Object} Validations for each column: strings containing regular expressions, keyed by columns key. */
      'maxFieldLengths' /** {Object} Maximum characters for each column. Dictionary of integers, keyed by column name. */
    ])
  },

  watch: {
    editValue (val) {
      if (val) {
        // Set initial values.
        this.showError = false
        this.validator = new RegExp(this.validations[val.col])
        this.value = val.row[val.col]
        this.highlightCell(true)
        // Open popup.
        this.$nextTick(() => {
          popper = new Popper(val.el, this.$el, {
            placement: 'top',
            modifiers: {
              preventOverflow: {
                boundariesElement: 'viewport'
              }
            }
          })
          // Give popper time before focusing the input to prevent random auto-scrolling.
          setTimeout(() => {
            if (this.$refs.input) {
              this.$refs.input.focus()
            }
          }, 30)
        })
      } else {
        // Close popup.
        popper.destroy()
      }
    }
  },

  mounted () {
    document.addEventListener('click', this.clickHandler)
  },

  beforeDestroy () {
    document.removeEventListener('click', this.clickHandler)
  },

  methods: {
    /**
     * Handle clicks that bubbled all the way up to document.
     * @param {Event} e - The click event.
     */
    clickHandler (e) {
      // Close the editing popup if the user clicks somewhere else.
      // Do nothing if no value is being edited.
      if (!this.editValue) return
      // Do nothing if the user clicked inside the popup.
      if (this.$el.tagName && this.$el.contains(e.target)) return
      // Do nothing if user clicked on the cell that’s being edited.
      if (e.target.classList.contains('dsa-edited')) return
      // Remove cell highlighting.
      this.highlightCell(false)
      // Flash the cell to indicate that the changes are being dismissed.
      if (this.changed) this.flashCell()
      // Stop editing the cell, causing the popup to close.
      this.$store.commit({ type: 'leaveValue' })
    },

    /**
     * Validate the edited value and save it to the store.
     * If the next cell is empty (as when editing a cell in a new row), edit the next cell.
     */
    save () {
      if (this.valid) {
        const nextCell = this.editValue.el.nextSibling
        // Remove cell highlighting.
        this.highlightCell(false)
        // Update the value in the store.
        this.$store.commit({
          type: 'updateValue',
          value: this.value
        })
        // If the next field is blank, edit it.
        this.$nextTick(() => {
          if (nextCell && !nextCell.textContent && !(nextCell.children[0] && nextCell.children[0].classList.contains('dsa-delete-contact'))) {
            // Cause the click handler to return and leave the popup open.
            nextCell.classList.add('dsa-edited')
            // Click the next cell to open the popup.
            dispatch(nextCell, 'click')
          }
        })
      } else {
        this.showError = true
      }
    },

    /**
     * Cancel editing a cell.
     */
    cancel () {
      this.highlightCell(false)
      this.$store.commit({ type: 'leaveValue' })
    },

    /**
     * Set cell highlighting by adding or removing the dsa-edited class.
     * @param {boolean} highlight - To highlight or not to highlight.
     */
    highlightCell (highlight) {
      if (highlight) {
        this.editValue.el.classList.add('dsa-edited')
      } else {
        this.editValue.el.classList.remove('dsa-edited')
      }
    },

    /**
     * Trigger a flash effect by adding the dsa-flash class for 1 second.
     */
    flashCell () {
      const el = this.editValue.el
      el.classList.add('dsa-flash')
      setTimeout(function () {
        el.classList.remove('dsa-flash')
      }, 1000)
    },

    text (text) {
      switch (text) {
        case 'save': return Drupal.t('Save')
        case 'cancel': return Drupal.t('Cancel')
      }
    }
  }
}
</script>

<style lang="css">
.dsa-edit-value-popup {
  background-color: #ccc;
  padding: 0.5rem;
}

.dsa-edit-value-input {
  display: block;
  width: 12rem;
}

.dsa-edit-value-save.el-button {
  width: calc(50% - 0.25rem);
  margin: 0;
}

.dsa-edit-value-cancel.el-button {
  width: calc(50% - 0.25rem);
  margin: 0;
  float: right;
}
</style>
