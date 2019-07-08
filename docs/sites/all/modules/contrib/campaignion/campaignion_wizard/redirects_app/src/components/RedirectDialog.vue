<docs>
RedirectDialog component.
The dialog to edit a redirect.
</docs>

<template lang="html">
  <ElDialog
    :title="dialogTitle"
    :visible="visible"
    :close-on-click-modal="false"
    :before-close="dialogCancelHandler"
    >

    <section class="pra-redirect-label">
      <label :for="'pra-redirect-label-' + _uid">{{ text('Redirect label') }} <small>{{ text('seen only by you') }}</small></label>
      <input type="text" v-model="currentRedirect.label" class="field-input" :id="'pra-redirect-label-' + _uid">
    </section>

    <FilterEditor
      :fields="$root.$options.settings.fields"
      :filters.sync="currentRedirect.filters"
      :operators="OPERATORS"
    />

    <section class="pra-redirect-destination">
      <label :for="'pra-redirect-destination-' + _uid">{{ text('Redirect destination') }} <small>{{ text('type a node title or ID or paste a URL') }}</small></label>
      <DestinationField
      :id="'pra-redirect-destination-' + _uid"
      :class="{'pra-has-error': showErrors && !destinationIsValid}"
      :value="destination"
      :placeholder="text('Type to search nodes')"
      :show-dropdown-on-focus="true"
      data-key="values"
      label-key="label"
      :getData="getNodes"
      :url="$root.$options.settings.endpoints.nodes"
      search-param="s"
      :count="20"
      @input="item => {destination = item}"
      />
      <div v-if="showErrors && !destinationIsValid" class="pra-error-message">{{ text('destination error') }}</div>
    </section>

    <span slot="footer" :class="{'pra-dialog-footer': true, 'pra-dialog-alert': modalDirty}">
      <span v-if="modalDirty" class="pra-dialog-alert-message">{{ text('unsaved changes') }}</span>
      <el-button @click="cancelButtonHandler()" class="js-modal-cancel">{{ text('Cancel') }}</el-button>
      <el-button type="primary" :disabled="currentRedirectIsEmpty" @click="updateRedirect" class="js-modal-save">{{ text('Done') }}</el-button>
    </span>

  </ElDialog>
</template>

<script>
import {clone, validateDestination} from '@/utils'
import {OPERATORS, emptyRedirect} from '@/utils/defaults'
import api from '@/utils/api'
import {mapState} from 'vuex'
import isEqual from 'lodash/isEqual'
import omit from 'lodash/omit'
import {DestinationField} from 'campaignion_vue'
import FilterEditor from './FilterEditor'

export default {
  components: {
    DestinationField,
    FilterEditor
  },

  data () {
    return {
      currentRedirect: emptyRedirect(), /** {Object} The redirect that is currently being edited. This belongs to the component, and must not be a reference to a redirect in the store. */
      modalDirty: false,                /** {boolean} The user has edited the redirect and tried to close the dialog without saving. */
      showErrors: false,                /** {boolean} Show destination field validation errors (only after the first attemt to save the redirect). */
      OPERATORS                         /** {Object} Dictionary of filter operators, keyed by identifier, each containing a `label` and a `phrase`. **/
    }
  },

  computed: {
    /**
     * Compute a dialog title depending on whether a new or an existing redirect is
     * being edited.
     * @return {(string|undefined)} Translated dialog title.
     */
    dialogTitle () {
      if (this.currentRedirectIndex === -1) {
        return Drupal.t('Add personalized redirect')
      } else if (this.currentRedirectIndex >= 0) {
        if (this.currentRedirect.label) {
          return Drupal.t('Edit @itemName', {'@itemName': this.currentRedirect.label})
        } else {
          return Drupal.t('Edit personalized redirect')
        }
      }
    },

    /**
     * Check if the redirect being edited is empty.
     * If no redirect is being edited, return `false`.
     * @return {boolean} Is the current redirect empty?
     */
    currentRedirectIsEmpty () {
      return this.currentRedirectIndex !== null && isEqual(omit(this.currentRedirect, ['id', 'prettyDestination']), omit(emptyRedirect(), ['id', 'prettyDestination']))
    },

    /**
     * Controls the edit dialog visibility.
     * @return {boolean} Is a redirect being edited?
     */
    visible () {
      return this.currentRedirectIndex !== null
    },

    /** Map `destination` to `value` and `prettyDestination` to `label` for the DestinationField component. */
    destination: {
      get () {
        return {
          value: this.currentRedirect.destination,
          label: this.currentRedirect.prettyDestination
        }
      },
      set (val) {
        this.currentRedirect.destination = val.value
        this.currentRedirect.prettyDestination = val.label
      }
    },

    /**
     * Validate the current redirect’s destination.
     * @return {boolean} Is it valid?
     */
    destinationIsValid () {
      return validateDestination(this.currentRedirect.destination)
    },

    ...mapState([
      'redirects',           /** {Object[]} All the personalized redirects. */
      'currentRedirectIndex' /** The index of the item in the redirects array that is currently being edited or `-1` for a new item. `null` means that no item is being edited. */
    ])
  },

  methods: {
    text (text) {
      switch (text) {
        case 'Redirect label': return Drupal.t('Redirect label')
        case 'seen only by you': return Drupal.t('(seen only by you)')
        case 'Redirect destination': return Drupal.t('Redirect destination')
        case 'type a node title or ID or paste a URL': return Drupal.t('type a node title or ID or paste a URL')
        case 'Type to search nodes': return Drupal.t('Type to search nodes')
        case 'destination error': return Drupal.t('Please enter a valid URL or choose a node.')
        case 'unsaved changes': return Drupal.t('You have unsaved changes!')
        case 'Cancel': return this.modalDirty ? Drupal.t('Discard my changes') : Drupal.t('Cancel')
        case 'Done': return Drupal.t('Done')
      }
    },

    getNodes: api.getNodes,

    /**
     * Callback to check if the dialog may be closed.
     * Allow to close it if:
     * - it’s an existing redirect and it hasn’t been changed
     * - it’s a new redirect and it’s empty
     * - the user has already been warned that changes will be lost and clicks
     *   the Cancel button.
     * In other cases, show the 'unsaved changes' warning and prevent the dialog
     * from  being closed.
     * @param {(Object|undefined)} options - Details about how the user tried to close the dialog.
     * @param {string} options.button - `cancel` if the user has clicked the Cancel button.
     * @return {boolean} May the dialog be closed?
     */
    tryClose (options) {
      // Any changes?
      if (this.currentRedirectIndex !== -1 && isEqual(this.currentRedirect, this.redirects[this.currentRedirectIndex]) ||
        this.currentRedirectIndex === -1 && this.currentRedirectIsEmpty ||
        (this.modalDirty && options && options.button === 'cancel')) {
        // No changes or force close via cancel button: allow to close modal.
        return true
      } else {
        // There are unsaved changes, alert!
        this.modalDirty = true
        return false
      }
    },

    /**
     * Handle dialog closing via the x button or ESC key.
     * @param {Function} done - el-dialog’s callback function.
     */
    dialogCancelHandler (done) {
      if (this.tryClose()) {
        this.close()
        done()
      }
    },

    /**
     * Handle dialog closing via the Cancel button.
     */
    cancelButtonHandler () {
      if (this.tryClose({button: 'cancel'})) {
        this.close()
      }
    },

    /**
     * Validate the redirect and save it to the store.
     */
    updateRedirect () {
      if (!this.destinationIsValid) {
        this.showErrors = true
        return
      }
      this.$store.commit({type: 'updateRedirect', redirect: this.currentRedirect})
      this.close()
    },

    /**
     * Stop editing the redirect (this will close the dialog via setting
     * `currentRedirectIndex` to `null`), reset component data and emit the
     * `closeRedirectDialog` event.
     */
    close () {
      this.modalDirty = false
      this.showErrors = false
      this.$store.commit('leaveRedirect')
      this.$root.$emit('closeRedirectDialog')
    }
  },

  mounted () {
    // Listen to events on the global bus, set `this.currentRedirect` and commit mutations
    // to set the store’s `currentRedirectIndex`, which causes the dialog to open.
    this.$root.$on('newRedirect', () => {
      this.currentRedirect = emptyRedirect()
      this.$store.commit('editNewRedirect')
    })
    this.$root.$on('editRedirect', index => {
      this.currentRedirect = clone(this.redirects[index])
      this.$store.commit({type: 'editRedirect', index})
    })
    this.$root.$on('duplicateRedirect', index => {
      const duplicate = clone(this.redirects[index])
      duplicate.id = emptyRedirect().id
      duplicate.label = Drupal.t('Copy of @redirectLabel', {'@redirectLabel': duplicate.label})
      this.currentRedirect = duplicate
      this.$store.commit('editNewRedirect')
    })
    // If the dialog is visible and it’s not an empty redirect, save changes on Enter keyup.
    // If the cursor is inside a textarea, don’t.
    document.addEventListener('keyup', e => {
      if (this.visible && !this.currentRedirectIsEmpty && e.keyCode === 13 && document.activeElement.tagName.toLowerCase() !== 'textarea') {
        e.preventDefault()
        this.updateRedirect()
      }
    })
  }
}
</script>

<style lang="css">
</style>
