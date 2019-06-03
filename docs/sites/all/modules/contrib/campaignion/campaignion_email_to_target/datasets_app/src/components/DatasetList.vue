<template lang="html">
  <section class="dsa-dataset-list">
    <input type="text" v-model="filter" :placeholder="text('filter placeholder')" class="field-input">
    <ul class="dsa-datasets">
      <li v-for="dataset in filteredDatasets" :key="dataset.key" @click="select(dataset)" v-tooltip.top="{content: dataset.description}">
        {{ dataset.title }}
      </li>
    </ul>
  </section>
</template>

<script>
import {mapState} from 'vuex'

export default {
  data () {
    return {
      filter: '' // Value of the filter input.
    }
  },

  computed: {
    /** @return {Object[]} Array of datasets with titles matching the filter string. */
    filteredDatasets () {
      return this.datasets.filter(dataset => {
        return dataset.title.toLowerCase().indexOf(this.filter.toLowerCase()) > -1
      })
    },
    ...mapState([
      'datasets',        /** {Object[]} Array of all available datasets. */
      'showSelectDialog' /** {boolean} Visibility of the select dataset dialog. */
    ])
  },

  watch: {
    showSelectDialog (val) {
      // Clear filter when dialog is shown.
      if (val) {
        this.filter = ''
      }
    }
  },

  methods: {
    /**
     * Select a dataset. Close the select dialog.
     * If it’s a custom dataset, load its contacts and edit it.
     * @param {Object} dataset - The selected dataset.
     */
    select (dataset) {
      this.$store.commit('closeSelectDialog')
      if (dataset.is_custom) {
        this.$store.dispatch({
          type: 'loadContactsAndEdit',
          dataset
        })
      } else {
        this.$store.commit({
          type: 'setSelectedDataset',
          key: dataset.key
        })
      }
    },

    text (text) {
      switch (text) {
        case 'filter placeholder': return Drupal.t('Type to filter the list of datasets')
      }
    }
  }
}
</script>

<style lang="css">
</style>
