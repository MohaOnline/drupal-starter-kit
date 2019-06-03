<docs>
FilterEditor component.
Provides a UI to edit a spec’s filters.
Use this component with the `.sync` modifier on the `filters` prop.
</docs>

<template lang="html">
  <section class="filter-editor">

    <header>
      <el-dropdown trigger="click" menu-align="start">
        <el-button>
          {{ text('Add filter') }}<i class="el-icon-caret-bottom el-icon--right"></i>
        </el-button>
        <el-dropdown-menu slot="dropdown">
          <el-dropdown-item v-for="field in fields" :key="field.name" @click.native="addFilter(field)">{{ field.label }}</el-dropdown-item>
        </el-dropdown-menu>
      </el-dropdown>
    </header>

    <ul class="filters">
      <li v-for="(filter, index) in f" :key="index" class="filter">

        <span v-if="index === 0" class="logical-connective">{{ text('If') }}</span>
        <span v-else class="logical-connective">{{ text('and') }}</span>

        <span class="attribute-label">{{ filter.attributeLabel }}</span>

        <el-select v-model="filter.operator">
          <el-option v-for="item in operatorOptions" :key="item.value" :label="item.label" :value="item.value"></el-option>
        </el-select>

        <template v-if="filter.operator == 'regexp'">/&nbsp;</template>
        <type-ahead
          v-model="filter.value"
          :placeholder="filter.operator == 'regexp' ? text('regular expression') : text('type to browse values')"
          :show-dropdown-on-focus="true"
          data-key="values"
          :async="e2tApi.url + '/' + e2tApi.dataset + '/attributes/' + filter.attributeName + '/values'"
          search-param="search"
          count-param="count"
          page-param="offset"
          :count="100"
          :lazy-load="true"
          page-mode="offset"
          :headers="{'Authorization': 'JWT ' + e2tApi.token}"
          >
        </type-ahead>
        <template v-if="filter.operator == 'regexp'">&nbsp;/</template>

        <a href="#" @click="removeFilter(index)" class="remove-filter" :title="text('Remove filter')"><span>{{ text('Delete') }}</span></a>

      </li>
    </ul>

  </section>
</template>

<script>
import {clone} from '@/utils'

export default {
  components: {
    typeAhead: require('./Typeahead.vue')
  },

  data () {
    return {
      f: this.filters, /** {Object[]} The spec’s filters. Internal property. */
      e2tApi: clone(Drupal.settings.campaignion_email_to_target.endpoints['e2t-api']) || {} /** {Object} Data needed to call the e2t API. Key: `url`, `dataset`, `token`. */
    }
  },

  props: {
    fields: Array,         /** {Object[]} Collection of the fields that can be filtered, each having a `name`, a `title` and a `description`. */
    filters: Array,        /** {Object[]} Collection of the spec’s filters. */
    filterDefault: Object, /** {Object} A new filter will be generated from this. */
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
        case 'If': return Drupal.t('If')
        case 'and': return Drupal.t('and')
        case 'regular expression': return Drupal.t('regular expression')
        case 'type to browse values': return Drupal.t('type to browse values')
        case 'Remove filter': return Drupal.t('Remove filter')
        case 'Delete': return Drupal.t('Delete')
      }
    },

    /**
     * Add a new filter to the collection.
     * @param {Object} field - The field used by the new filter.
     */
    addFilter (field) {
      var filter = Object.assign({}, this.filterDefault)
      filter.id = null
      filter.attributeName = field.name
      filter.attributeLabel = field.label
      filter.operator = '==' // default
      filter.value = ''
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
