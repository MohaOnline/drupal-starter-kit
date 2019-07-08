<docs>
SpecDialog component.
The dialog to edit a spec.
</docs>

<template lang="html">
  <el-dialog
    :title="dialogTitle"
    :visible="visible"
    :close-on-click-modal="false"
    :before-close="dialogCancelHandler"
    >

    <section class="spec-label-field">
      <label for="spec-label">{{ text('spec label') }} <small>{{ text('seen only by you') }}</small></label>
      <input type="text" v-model="currentSpec.label" class="field-input" id="spec-label">
    </section>

    <filter-editor
      :fields="targetAttributes"
      :filters.sync="currentSpec.filters"
      :filter-default="{type: 'target-attribute'}"
      :operators="OPERATORS"
      >
    </filter-editor>

    <section v-if="currentSpec.type === 'message-template'" class="spec-message-fields">
      <a href="#" @click="prefillMessage()" class="prefill-message" v-if="currentSpec.type == 'message-template'">{{ text('prefill') }}</a>
      <message-editor v-model="currentSpec.message" :type="currentSpec.type"></message-editor>
    </section>

    <section v-if="currentSpec.type === 'exclusion'" class="spec-message-fields">
      <div class="exclusion-mode-radio">
        <input type="radio" v-model="exclusionMode" value="message" id="exclusion-mode-radio-1">
        <label class="option" for="exclusion-mode-radio-1">{{ text('show message') }}</label>
      </div>
      <message-editor v-if="exclusionMode === 'message'" v-model="currentSpec.message" :type="currentSpec.type"></message-editor>
      <div class="exclusion-mode-radio">
        <input type="radio" v-model="exclusionMode" value="redirect" id="exclusion-mode-radio-2">
        <label class="option" for="exclusion-mode-radio-2">{{ text('redirect user') }}</label>
      </div>
      <div v-if="exclusionMode === 'redirect'" class="exclusion-mode-redirect">
        <label :for="'exclusion-redirect-destination-' + _uid">{{ text('redirect destination') }}</label>
        <DestinationField
        :id="'exclusion-redirect-destination-' + _uid"
        :class="{'field-has-error': destination.value && !destinationIsValid}"
        :value="destination"
        :placeholder="text('Type to search nodes')"
        :maxlength="1023"
        :show-dropdown-on-focus="true"
        data-key="values"
        label-key="label"
        value-key="value"
        :getData="getNodes"
        :url="nodesEndpoint"
        search-param="s"
        :count="20"
        @input="item => {destination = item}"
        />
        <small>{{ text('redirect destination explanation') }}</small>
      </div>
    </section>

    <tokens-list v-if="!(currentSpec.type === 'exclusion' && exclusionMode === 'redirect')" :token-categories="tokenCategories"></tokens-list>

    <section class="exclusion-warning" v-if="currentSpec.type === 'exclusion' && (currentSpecIndex > 0 || (currentSpecIndex === -1 && specs.length))">
      {{ text('exclusion warning') }}
    </section>

    <span slot="footer" :class="{'dialog-footer': true, 'dialog-alert': modalDirty}">
      <span v-if="modalDirty" class="dialog-alert-message">{{ text('unsaved changes') }}</span>
      <el-button @click="cancelButtonHandler()" class="js-modal-cancel">{{ text('Cancel') }}</el-button>
      <el-button type="primary" :disabled="currentSpecIsEmpty" @click="updateSpec" class="js-modal-save">{{ text('Done') }}</el-button>
    </span>

  </el-dialog>
</template>

<script>
import {clone, validateDestination} from '@/utils'
import {OPERATORS, emptySpec} from '@/utils/defaults'
import {mapState} from 'vuex'
import isEqual from 'lodash.isequal'
import omit from 'lodash.omit'
import animatedScrollTo from 'animated-scrollto'
import FilterEditor from './FilterEditor'
import MessageEditor from './MessageEditor'
import TokensList from './TokensList'
import {DestinationField, utils as commonUtils} from 'campaignion_vue'

export default {

  components: {
    FilterEditor,
    MessageEditor,
    TokensList,
    DestinationField
  },

  data () {
    return {
      currentSpec: emptySpec('message-template'), /** {Object} The spec that is currently being edited. This belongs to the component, and must not be a reference to a spec in the store. */
      exclusionMode: 'message', /** {string} `message` or `redirect` */
      nodesEndpoint: Drupal.settings.campaignion_email_to_target.endpoints.nodes, /** {string} URL for retrieving nodes matching a search term. */
      visible: false,    /** {boolean} Show the dialog or not. */
      modalDirty: false, /** {boolean} The user has edited the spec and tried to close the dialog without saving. */
      OPERATORS          /** {Object} Dictionary of filter operators, keyed by identifier, each containing a `label` and a `phrase`. **/
    }
  },

  computed: {
    /**
     * Compute a dialog title depending on whether a new or an existing
     * spec is being edited. If no spec is being edited, return an empty string.
     * @return {string} Translated dialog title.
     */
    dialogTitle () {
      if (this.currentSpecIndex === null) {
        return ''
      } else if (this.currentSpecIndex === -1) {
        switch (this.currentSpec.type) {
          case 'message-template':
            return Drupal.t('Add specific Message')
          case 'exclusion':
            return Drupal.t('Add exclusion')
        }
      } else if (this.currentSpecIndex >= 0) {
        return Drupal.t('Edit @itemName', {'@itemName': this.currentSpec.label})
      }
    },

    /**
     * Check if the spec being edited is empty.
     * If no spec is being edited, return `false`.
     * @return {boolean} Is the current spec empty?
     */
    currentSpecIsEmpty () {
      return this.currentSpecIndex !== null && isEqual(omit(this.currentSpec, ['id', 'errors', 'filterStr']), omit(emptySpec(this.currentSpec.type), ['id', 'errors', 'filterStr']))
    },

    /** Map `url` to `value` and `urlLabel` to `label` for the DestinationField component. */
    destination: {
      get () {
        return {
          value: this.currentSpec.url,
          label: this.currentSpec.urlLabel
        }
      },
      set (val) {
        this.currentSpec.url = val.value
        this.currentSpec.urlLabel = val.label
      }
    },

    /**
     * Validate the current redirect’s destination.
     * @return {boolean} Is it valid?
     */
    destinationIsValid () {
      return validateDestination(this.currentSpec.url)
    },

    ...mapState([
      'specs',            /** {Object[]} Specifications for specific targets, these are either of type `message-template` or `exclusion`. */
      'currentSpecIndex', /** {(integer|null)} The index of the item in the specs array that is currently being edited or `-1` for a new item, or `null`, if no item is being edited. */
      'targetAttributes', /** {Object[]} Collection of objects describing the target attributes: {name: 'contact.email', label: 'Email address', description: ''} */
      'tokenCategories'   /** {Object[]} Collection of categories with a `title`, a `description` and a collection of `tokens`. The `tokens` each have a `title`, a `description` and a `token`. */
    ])
  },

  watch: {
    /**
     * Set dialog visibility depending on the value of currentSpecIndex.
     * @param {(integer|null)} val - The index of the item in the specs array that is currently being edited or `-1` for a new item, or `null`, if no item is being edited.
     */
    currentSpecIndex (val) {
      this.visible = val !== null
    }
  },

  methods: {
    text (text) {
      switch (text) {
        case 'spec label': return this.currentSpec.type === 'message-template' ? Drupal.t('Internal name for this message') : Drupal.t('Internal name for this exclusion')
        case 'seen only by you': return Drupal.t('(seen only by you)')
        case 'show message': return Drupal.t('Write a message')
        case 'redirect user': return Drupal.t('Redirect supporter')
        case 'redirect destination': return Drupal.t('Redirect to:')
        case 'redirect destination explanation': return Drupal.t('Node ID or external URL')
        case 'prefill': return Drupal.t('Prefill from default message')
        case 'exclusion warning': return Drupal.t('Keep in mind that the order of specific messages and exclusions is important. Targets matching this exclusion’s filters could receive specific messages if they also match their filters. Drag this exclusion to the top of the list if you want it to apply under any condition.')
        case 'unsaved changes': return Drupal.t('You have unsaved changes!')
        case 'Cancel': return this.modalDirty ? Drupal.t('Discard my changes') : Drupal.t('Cancel')
        case 'Done': return Drupal.t('Done')
      }
    },

    /**
     * Make a GET request with a parameter.
     * @param {Object} options - We like objects as arguments.
     * @param {string} options.url - The url.
     * @param {string} options.queryParam - The parameter key.
     * @param {string} options.queryString - The parameter value.
     * @param {Object} options.headers - Headers to send along.
     * @return {Promise} A Promise resolving if the request succeeds.
     */
    getNodes ({url, queryParam, queryString, headers}) {
      return this.$http.get(commonUtils.paramReadyUrl(url) + queryParam + '=' + queryString, {headers})
    },

    /**
     * Callback to check if the dialog may be closed.
     * Allow to close it if:
     * - it’s an existing spec and it hasn’t been changed
     * - it’s a new spec and it’s empty
     * - the user has already been warned that changes will be lost and clicks
     *   the Cancel button.
     * In other cases, show the 'unsaved changes' warning and prevent the dialog
     * from  being closed.
     * @param {(Object|undefined)} options - Details about how the user tried to close the dialog.
     * @param {string} options.button - `cancel` if the user has clicked the Cancel button.
     * @return {boolean} May the dialog be closed?
     */
    tryClose (options) {
      // any changes?
      if (this.currentSpecIndex !== -1 && isEqual(this.currentSpec, this.specs[this.currentSpecIndex]) ||
        this.currentSpecIndex === -1 && this.currentSpecIsEmpty ||
        (this.modalDirty && options && options.button === 'cancel')) {
        // no changes, allow to close modal
        return true
      } else {
        // there are unsaved changes, alert!
        this.modalDirty = true
        animatedScrollTo(
          this.$root.$el.querySelector('.el-dialog__wrapper'),
          this.$el.querySelector('.js-modal-cancel').offsetTop,
          400
        )
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
     * Save the edited spec to the store and trigger spec validation.
     */
    updateSpec () {
      // With exclusions, you can choose message or redirect.
      // Clear the irrelevant option:
      if (this.currentSpec.type === 'exclusion') {
        if (this.exclusionMode === 'message') {
          this.currentSpec.url = ''
          this.currentSpec.urlLabel = ''
        } else if (this.exclusionMode === 'redirect') {
          this.currentSpec.message.body = ''
        }
      }
      this.$store.commit({type: 'updateSpec', spec: this.currentSpec})
      this.$store.commit('validateSpecs')
      this.close()
    },

    /**
     * Set `this.exclusionMode` to `message` or `redirect`, depending on
     * whether there is a message or a url in the current spec.
     * Defaults to `message`.
     */
    determineExclusionMode () {
      if (this.currentSpec.url) {
        this.exclusionMode = 'redirect'
      } else {
        this.exclusionMode = 'message'
      }
    },

    /**
     * Stop editing the spec (this will close the dialog via setting
     * `currentSpecIndex` to `null`), reset component data and emit the
     * `closeSpecDialog` event.
     */
    close () {
      this.modalDirty = false
      this.$store.commit('leaveSpec')
      this.$bus.$emit('closeSpecDialog')
    },

    /**
     * Populate the spec’s message object by copying the corresponding fields
     * from the default message. Overwrite only empty fields or fields containing
     * only whitespace.
     */
    prefillMessage () {
      if (!this.currentSpec.message) return
      for (var field in this.currentSpec.message) {
        if (this.currentSpec.message.hasOwnProperty(field)) {
          if (!this.currentSpec.message[field].trim()) {
            this.currentSpec.message[field] = this.$store.state.defaultMessage.message[field]
          }
        }
      }
    }
  },

  mounted () {
    // Listen to events on the global bus, set `this.currentSpec` and commit mutations
    // to set the store’s `currentSpecIndex`, which causes the dialog to open.
    this.$bus.$on('newSpec', type => {
      this.currentSpec = emptySpec(type)
      this.determineExclusionMode()
      this.$store.commit('editNewSpec')
    })
    this.$bus.$on('editSpec', index => {
      this.currentSpec = clone(this.specs[index])
      this.determineExclusionMode()
      this.$store.commit({type: 'editSpec', index})
    })
    this.$bus.$on('duplicateSpec', index => {
      const duplicate = clone(this.specs[index])
      duplicate.id = emptySpec(duplicate.type).id
      duplicate.label = Drupal.t('Copy of @messageName', {'@messageName': duplicate.label})
      this.currentSpec = duplicate
      this.determineExclusionMode()
      this.$store.commit('editNewSpec')
    })
    // If the dialog is visible and it’s not an empty spec, save changes on Enter keyup.
    // If the cursor is inside a textarea, don’t.
    document.addEventListener('keyup', e => {
      if (this.visible && !this.currentSpecIsEmpty && e.keyCode === 13 && document.activeElement.tagName.toLowerCase() !== 'textarea') {
        e.preventDefault()
        this.updateSpec()
      }
    })
  }

}
</script>

<style lang="scss">
.e2tmw {
  .typeahead.field-has-error input {
    border-color: red;
  }

  section {
    margin-bottom: 1rem;

    &.spec-message-fields { margin-bottom: 0; }
  }
}
</style>
