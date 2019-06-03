<docs>
RedirectDescription component.
Display a human readable description of a redirect’s filters.
</docs>

<template lang="html">
  <ul class="pra-redirect-info-filters">
    <li v-for="filter in redirect.filters">
      <template v-if="filter.type === 'opt-in'">{{ filter.value ? text('User has opted in') : text('User has not opted in') }}</template>
      <template v-else>{{ filterDescription(filter) }}</template>
    </li>
  </ul>
</template>

<script>
import {OPERATORS} from '@/utils/defaults'
import {find} from 'lodash'

export default {
  props: {
    redirect: Object,
    index: Number
  },
  methods: {
    text (text) {
      switch (text) {
        case 'User has opted in': return Drupal.t('Supporter has opted in')
        case 'User has not opted in': return Drupal.t('Supporter hasn’t opted in')
      }
    },

    /**
     * Get a phrase describing the filter, like 'First name does not contain foo'.
     * @param {Object} filter - A personalized redirect’s filter.
     * @return {string} The filter operator’s translated phrase.
     */
    filterDescription (filter) {
      const fieldLabel = find(this.$root.$options.settings.fields, {id: filter.field}).label
      return Drupal.t(OPERATORS[filter.operator].phrase, {'@attribute': fieldLabel, '@value': filter.value})
    }
  }
}
</script>

<style lang="css">
</style>
