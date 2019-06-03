<template>
  <div class="email-to-target-messages-widget e2tmw" data-interrupt-submit :data-has-unsaved-changes="unsavedChanges">
    <el-button @click="newSpec('message-template')">{{ text('Create message') }}</el-button>
    <el-button @click="newSpec('exclusion')">{{ text('Create exclusion') }}</el-button>
    <div class="e2t-col"><p>{{ text('messages help') }}</p></div>
    <spec-list></spec-list>
    <section class="default-message">
      <message-editor :value="defaultMessage.message" @input="updateDefaultMessage" type="message-template">
        <legend slot="legend">{{ specs.length ? text('message to remaining targets') : text('default message') }}</legend>
      </message-editor>
      <ul class="spec-errors">
        <li v-for="error in defaultMessageErrors" class="spec-error">{{ error.message }}</li>
      </ul>
      <tokens-list :token-categories="tokenCategories"></tokens-list>
    </section>

    <spec-dialog></spec-dialog>
  </div>
</template>

<script>
import {mapState} from 'vuex'
import {clone, dispatch, isEmptyMessage} from '@/utils'
import isEqual from 'lodash.isequal'
import omit from 'lodash.omit'
import SpecList from './components/SpecList'
import MessageEditor from './components/MessageEditor'
import TokensList from './components/TokensList'
import SpecDialog from './components/SpecDialog'

export default {

  name: 'app',

  components: {
    SpecList,
    MessageEditor,
    TokensList,
    SpecDialog
  },

  computed: {
    /**
     * Get an errors collection for the default message in case it’s empty.
     * @return {(Object[]|undefined)} An errors collection with an error object for the default message.
     */
    defaultMessageErrors () {
      if (isEmptyMessage(this.defaultMessage.message)) {
        return [{type: 'message', message: 'Message is empty'}]
      }
    },

    /**
     * Compare the specs and the default message with initial state.
     * @return {boolean} Is there a difference to initial state?
     */
    unsavedChanges () {
      for (let i = 0, j = this.$store.state.specs.length; i < j; i++) {
        if (!isEqual(omit(this.$store.state.specs[i], ['errors', 'filterStr']), omit(this.$store.state.initialData.specs[i], ['errors', 'filterStr']))) {
          return true
        }
      }
      if (!isEqual(omit(this.$store.state.defaultMessage, ['errors', 'filterStr']), omit(this.$store.state.initialData.defaultMessage, ['errors', 'filterStr']))) {
        return true
      }
      return false
    },

    ...mapState([
      'specs',          /** {Object[]} Specifications for specific targets, these are either of type `message-template` or `exclusion`. */
      'defaultMessage', /** {Object} The spec object representing the message sent to all remaining targets. */
      'tokenCategories' /** {Object[]} Collection of categories with a `title`, a `description` and a collection of `tokens`. The `tokens` each have a `title`, a `description` and a `token`. */
    ])
  },

  methods: {
    text (text) {
      switch (text) {
        case 'messages help': return Drupal.t('Order your messages and exceptions below with the most specific messages at the top. The target will receive the first message that they meet the criteria for.')
        case 'Create message': return Drupal.t('Add specific message')
        case 'Create exclusion': return Drupal.t('Add exclusion')
        case 'message to remaining targets': return Drupal.t('Message to all remaining targets')
        case 'default message': return Drupal.t('Default message that will be sent to target(s)')
        case 'service unavailable title': return Drupal.t('Service unavailable')
        case 'service unavailable': return Drupal.t('The service is temporarily unavailable.\rYour messages could not be saved.\rPlease try again or contact support if the issue persists.')
        case 'unsaved changes title': return Drupal.t('Unsaved changes')
        case 'unsaved changes': return Drupal.t('You have unsaved changes!\rYou will lose your changes if you go back.')
        case 'invalid data title': return Drupal.t('Invalid data')
        case 'invalid data': return Drupal.t('There are validation errors (see error notices).\rYour campaign might not work as you intended.')
        case 'OK': return Drupal.t('OK')
        case 'Cancel': return Drupal.t('Cancel')
        case 'Go back anyway': return Drupal.t('Go back anyway')
        case 'Stay on page': return Drupal.t('Stay on page')
        case 'Save anyway': return Drupal.t('Save anyway')
      }
    },

    /**
     * Emit a `newSpec` event on the global bus, with the spec type as the payload.
     * @param {string} type - The type of the spec to create.
     */
    newSpec (type) {
      this.$bus.$emit('newSpec', type)
    },

    /**
     * Set the default message.
     * @param {Object} val - The message object to store.
     */
    updateDefaultMessage (val) {
      this.$store.commit({type: 'updateDefaultMessage', message: val})
    }
  },

  created () {
    this.$store.commit('initializeData', Drupal.settings.campaignion_email_to_target)
    this.$store.commit('validateSpecs')
  },

  mounted () {
    const listener = e => {
      /** Answer back to interrupt-submit.js, it’s ok to leave the page. */
      const leavePage = () => {
        dispatch(this.$el, 'resume-leave-page')
      }

      /** Answer back to interrupt-submit.js, don’t leave the page. */
      const stayOnPage = () => {
        dispatch(this.$el, 'cancel-leave-page')
      }

      /** Persist the data to the server. */
      const putData = () => {
        // Append the default message to the specs array.
        const messages = clone(this.$store.state.specs)
        messages.push(clone(this.$store.state.defaultMessage))
        const data = JSON.stringify({
          messageSelection: messages
        })
        this.$http.put(Drupal.settings.campaignion_email_to_target.endpoints.messages, data).then((response) => {
          // success
          leavePage()
        }, (response) => {
          // error
          stayOnPage()
          this.$alert(this.text('service unavailable'), this.text('service unavailable title'), {
            confirmButtonText: this.text('OK')
          })
        })
      }

      // Does the user want to leave the page without saving?
      if (e.type === 'request-leave-page') {
        if (this.unsavedChanges) {
          this.$confirm(this.text('unsaved changes'), this.text('unsaved changes title'), {
            confirmButtonText: this.text('Go back anyway'),
            cancelButtonText: this.text('Stay on page'),
            type: 'warning'
          }).then(() => { leavePage() }, () => { stayOnPage() })
        } else {
          leavePage()
        }
        // By now, we’ve established that it’s ok to leave the page or or the user decided to stay,
        // so we don’t proceed.
        return
      }

      // Should we warn the user before saving invalid data?
      if (this.$store.state.hardValidation) {
        var validationFailed = false
        // Check each of the specs for errors.
        for (let i = 0, j = this.specs.length; i < j; i++) {
          if (this.$store.state.specs[i].errors && this.$store.state.specs[i].errors.length) {
            validationFailed = true
            break
          }
        }
        // Check the default message for errors.
        if (this.defaultMessageErrors && this.defaultMessageErrors.length) {
          validationFailed = true
        }
        if (validationFailed) {
          this.$confirm(this.text('invalid data'), this.text('invalid data title'), {
            confirmButtonText: this.text('Save anyway'),
            cancelButtonText: this.text('Cancel'),
            type: 'warning'
          }).then(() => { putData() }, () => { stayOnPage() })
          return
        }
      }

      // This is the standard procedure for unpublished actions. The user has clicked
      // 'Save as draft' or 'Next', and we persist the data even if there are errors.
      putData()
    }

    // Listen to events dispatched by campaignion_vue’s interrupt-submit.js
    this.$el.addEventListener('request-submit-page', listener)
    this.$el.addEventListener('request-leave-page', listener)
  }

}
</script>

<style lang="scss">
.e2tmw {
  *, *:before, *:after {
    box-sizing: border-box;
  }

  ul, li {
    list-style: none;
    padding: 0;
    margin: 0;
  }
  .e2t-col {
    margin-top: 1rem;
  } 
}
</style>
