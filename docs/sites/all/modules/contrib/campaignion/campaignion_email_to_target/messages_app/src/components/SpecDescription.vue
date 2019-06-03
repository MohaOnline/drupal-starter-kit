<docs>
SpecDescription component.
Shows a sentence describing a spec’s functionality.
</docs>

<template lang="html">
  <span>
    {{ specDescription }}
    <template v-if="spec.filters.length" v-for="(filter, index) in spec.filters">
      <template v-if="index">&nbsp;{{ text('and') }}&nbsp;</template>
      <span v-if="filter.value" class="filter-condition">{{ filterDescription(filter) }}</span>
      <span v-else class="value-missing">[&nbsp;{{ text('missing value') }}&nbsp;]</span>
    </template>
    <span v-if="!spec.filters.length" class="filter-missing">[&nbsp;{{ text('missing filter') }}&nbsp;]</span>
  </span>
</template>

<script>
import {OPERATORS} from '@/utils/defaults'

export default {
  props: {
    spec: Object, /** The spec object. */
    index: Number /** The spec’s index in the specs array. */
  },
  computed: {
    /**
     * Get a description for the spec depending on it’s type and position in the array.
     * @return {string} The description, ending in a way that the filters’ description can be appended.
     */
    specDescription () {
      switch (this.spec.type) {
        case 'message-template':
          return (this.index === 0)
            ? Drupal.t('Send this mail to all targets where ')
            : Drupal.t('Send this mail to all remaining targets where ')
        case 'exclusion':
          return (this.index === 0)
            ? Drupal.t('Exclude all targets where ')
            : Drupal.t('Exclude all remaining targets where ')
      }
    }
  },
  methods: {
    text (text) {
      switch (text) {
        case 'and': return Drupal.t('and')
        case 'missing filter': return Drupal.t('please add a filter')
        case 'missing value': return Drupal.t('please add a filter')
      }
    },

    /**
     * Generate a translated phrase describing a filter.
     * @param {Object} filter - The spec’s filter object.
     * @return {string} The phrase describing the filter.
     */
    filterDescription (filter) {
      return Drupal.t(OPERATORS[filter.operator].phrase, {'@attribute': filter.attributeLabel, '@value': filter.value})
    }
  }
}
</script>

<style lang="css">
</style>
