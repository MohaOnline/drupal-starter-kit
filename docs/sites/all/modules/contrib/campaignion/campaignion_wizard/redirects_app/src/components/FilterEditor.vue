<docs>
FilterEditor component.
Provides a UI to edit a redirect’s filters.
Use this component with the `.sync` modifier on the `filters` prop.
</docs>

<template lang="html">
  <section class="pra-filter-editor">

    <header>
      <ElDropdown trigger="click" menu-align="start">
        <ElButton>
          {{ text('Add filter') }}<i class="el-icon-caret-bottom el-icon--right"></i>
        </ElButton>
        <ElDropdownMenu slot="dropdown">
          <ElDropdownItem :disabled="optInUsed" @click.native="addFilter('opt-in')">{{ text('opt-in filter') }}</ElDropdownItem>
          <ElDropdownItem :disabled="!fields.length" @click.native="addFilter('submission-field')">{{ text('submission-field filter') }}</ElDropdownItem>
        </ElDropdownMenu>
      </ElDropdown>
    </header>

    <ul class="pra-filters">
      <li v-for="(filter, index) in f" :key="index" :class="['pra-filter', 'pra-filter-' + filter.type]">

        <span v-if="index === 0" class="pra-logical-connective">{{ text('If') }}</span>
        <span v-else class="pra-logical-connective">{{ text('and') }}</span>

        <template v-if="filter.type === 'opt-in'" class="">
          {{ text('supporter') }}
          <ElSelect v-model="filter.value">
            <ElOption :label="text('has')" :value="true"/>
            <ElOption :label="text('has not')" :value="false"/>
          </ElSelect>
          {{ text('opted in') }}
        </template>

        <template v-else>
          <ElSelect v-model="filter.field" class="pra-filter-field">
            <ElOption v-for="field in fields" :key="field.id" :label="field.label" :value="field.id"/>
          </ElSelect>
          <ElSelect v-model="filter.operator" class="pra-filter-operator">
            <ElOption v-for="item in operatorOptions" :key="item.value" :label="item.label" :value="item.value"/>
          </ElSelect>
          <input
            v-model="filter.value"
            type="text"
            autocomplete="off"
            :placeholder="filter.operator.indexOf('regexp') !== -1 ? 'regular expression' : ''"
            class="pra-filter-value field-input field-inline"
          />
        </template>

        <a href="#" @click="removeFilter(index)" class="remove-filter" :title="text('Remove filter')"><span>{{ text('Delete') }}</span></a>

      </li>
    </ul>
  </section>
</template>

<script>
import {find} from 'lodash'

export default {
  data () {
    return {
      f: this.filters /** {Object[]} The redirect’s filters. Internal property. */
    }
  },

  props: {
    fields: Array,         /** {Object[]} Collection of the fields that can be filtered, each having an `id` and a `label`. */
    filters: Array,        /** {Object[]} Collection of the redirect’s filters. */
    operators: {           /** {Object} Dictionary of filter operators, keyed by identifier, each containing a `label` and a `phrase`. **/
      type: Object,
      required: true
    }
  },

  computed: {
    /**
     * Provide the operators in a way that’s understood by the select’s el-option component.
     * @return {Object[]} Collection of options like `{value: '==', label: 'is'}`
     */
    operatorOptions () {
      var arr = []
      Object.keys(this.operators).map(key => {
        arr.push({
          value: key,
          label: this.operators[key].label
        })
      })
      return arr
    },

    /**
     * Searches the filters for a filter with the `opt-in` type.
     * @return {boolean} Has an opt-in filter already been used?
     */
    optInUsed () {
      return !!find(this.f, {type: 'opt-in'})
    }
  },

  watch: {
    // Emit an `update` event for the parent component.
    f (val) {
      this.$emit('update:filters', val)
    },
    // Update the internal filters property.
    filters (val) {
      this.f = this.filters
    }
  },

  methods: {
    text (text) {
      switch (text) {
        case 'Add filter': return Drupal.t('Add filter')
        case 'opt-in filter': return Drupal.t('Opt-in')
        case 'submission-field filter': return Drupal.t('Submission field')
        case 'If': return Drupal.t('If')
        case 'and': return Drupal.t('and')
        case 'supporter': return Drupal.t('supporter')
        case 'has': return Drupal.t('has')
        case 'has not': return Drupal.t('has not')
        case 'opted in': return Drupal.t('opted in')
        case 'regular expression': return Drupal.t('regular expression')
        case 'type to browse values': return Drupal.t('type to browse values')
        case 'Remove filter': return Drupal.t('Remove filter')
        case 'Delete': return Drupal.t('Delete')
      }
    },

    /**
     * Add a new filter to the collection.
     * @param {string} type - The type of the new filter.
     */
    addFilter (type) {
      var filter = {
        id: null,
        type
      }
      switch (type) {
        case 'opt-in':
          filter.value = true
          break
        case 'submission-field':
          filter.field = this.fields[0].id
          filter.operator = '=='
          filter.value = ''
      }
      this.f.push(filter)
    },

    /**
     * Remove a filter from the collection.
     * @param {integer} index - The index of the filter to delete.
     */
    removeFilter (index) {
      this.f.splice(index, 1)
    }
  }
}
</script>

<style lang="css">
</style>
