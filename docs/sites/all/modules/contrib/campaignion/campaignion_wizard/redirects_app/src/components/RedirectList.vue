<docs>
RedirectList component.
Displays all redirects in a draggable list.
</docs>

<template lang="html">
  <Draggable
    v-model="redirects"
    element="ul"
    class="pra-redirects"
    :options="{ handle: '.pra-redirect-handle', forceFallback: true }"
    @start="dragStart"
    @end="dragEnd"
    >
    <li v-for="(redirect, index) in redirects" :key="index" class="pra-redirect">
      <span class="pra-redirect-handle"></span>
      <span class="pra-redirect-info">
        <div v-if="redirect.label" class="pra-redirect-label">{{ redirect.label }}</div>
        <RedirectDescription :redirect="redirect" :index="index" class="pra-redirect-description"/>
        <div class="pra-redirect-destination">➜ {{ redirect.prettyDestination }}</div>
      </span>

      <ElDropdown split-button trigger="click" @click="editRedirect(index)" class="pra-redirect-actions">
        {{ text('Edit') }}
        <ElDropdownMenu slot="dropdown">
          <ElDropdownItem @click.native="duplicateRedirect(index)" class="pra-duplicate-redirect">{{ text('Duplicate') }}</ElDropdownItem>
          <ElDropdownItem @click.native="removeRedirect(index)" class="pra-delete-redirect">{{ text('Delete') }}</ElDropdownItem>
        </ElDropdownMenu>
      </ElDropdown>
    </li>
  </Draggable>
</template>

<script>
import Draggable from 'vuedraggable'
import RedirectDescription from './RedirectDescription'

export default {

  components: {
    RedirectDescription,
    Draggable
  },

  computed: {
    redirects: {
      get () {
        return this.$store.state.redirects
      },
      set (val) {
        this.$store.commit({type: 'updateRedirects', redirects: val})
      }
    }
  },

  methods: {
    /**
    * Emit an `editRedirect` event on the global bus, with the redirect’s index as the payload.
    * @param {integer} index - The index of the redirect to edit.
     */
    editRedirect (index) {
      this.$root.$emit('editRedirect', index)
    },

    /**
    * Emit a `duplicateRedirect` event on the global bus, with the redirect’s index as the payload.
    * @param {integer} index - The index of the redirect to duplicate.
     */
    duplicateRedirect (index) {
      this.$root.$emit('duplicateRedirect', index)
    },

    /**
     * Ask for confirmation, then commit a `removeRedirect` mutation.
     * @param {integer} index - The index of the redirect to remove.
     */
    removeRedirect (index) {
      const title = Drupal.t('Remove redirect?')
      const question = this.redirects[index].label
        ? Drupal.t('Do you really want to remove the redirect "@label"?', {'@label': this.redirects[index].label})
        : Drupal.t('Do you really want to remove the redirect to @url?', {'@url': this.redirects[index].prettyDestination})
      this.$confirm(question, title, {
        confirmButtonText: 'Remove',
        cancelButtonText: 'Cancel',
        type: 'warning'
      }).then(() => {
        this.$store.commit({type: 'removeRedirect', index})
      }, () => {})
    },

    /**
     * draggable handler.
     * Add a `dragging` class to the body.
     */
    dragStart () {
      document.body.classList.add('dragging')
    },

    /**
     * draggable handler.
     * Remove the `dragging` class from the body.
     */
    dragEnd () {
      document.body.classList.remove('dragging')
    },

    text (text) {
      switch (text) {
        case 'Redirect to': return Drupal.t('Redirect to')
        case 'Edit': return Drupal.t('Edit')
        case 'Duplicate': return Drupal.t('Duplicate')
        case 'Delete': return Drupal.t('Delete')
      }
    }
  }
}
</script>

<style lang="css">
.pra-redirect-card {
  border: 1px solid #ccc;
}

.pra-redirect-handle {
  display: inline-block;
  vertical-align: middle;
  width: 0.5rem;
  height: 1.3rem;
  margin-right: 0.5rem;
  border-left: 0.2rem solid #aaa;
  border-right: 0.2rem solid #aaa;
  cursor: move; /* fallback if grab cursor is unsupported */
  cursor: grab;
  cursor: -moz-grab;
  cursor: -webkit-grab;
}

.pra-redirect-info {
  display: inline-block;
  vertical-align: middle;
}

body.dragging, body.dragging * {
  cursor: grabbing;
  cursor: -moz-grabbing;
  cursor: -webkit-grabbing;
}
</style>
