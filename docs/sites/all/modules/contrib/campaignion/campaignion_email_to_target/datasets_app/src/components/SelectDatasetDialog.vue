<template lang="html">
  <el-dialog
    :title="text('Choose your dataset')"
    :visible="showSelectDialog"
    :close-on-click-modal="false"
    size="medium"
    :before-close="dialogCancelHandler"
    class="dsa-select-dataset-dialog"
    >

    <template slot="title">
      <span class="el-dialog__title">{{ text('Choose your dataset') }}</span>
      <el-button type="button" @click="editNewDataset" class="dsa-add-new-dataset">{{ text('Add new dataset') }}</el-button>
    </template>

    <p class="dsa-hint">{{ text('hint') }}</p>

    <DatasetList />

  </el-dialog>
</template>

<script>
import {mapState} from 'vuex'
import DatasetList from '@/components/DatasetList'

export default {
  components: {
    DatasetList
  },

  computed: {
    ...mapState([
      'showSelectDialog' /** {boolean} Visibility of the dialog. */
    ])
  },

  methods: {
    /**
     * Close the select dialog and start editing a new dataset.
     */
    editNewDataset () {
      this.$store.commit('closeSelectDialog')
      this.$store.commit('editNewDataset')
    },

    /**
     * Handle cancelling of the element-ui dialog.
     * Close the select dialog and call done().
     * @param {function} done - Passed by element-ui dialog. Call done() to finish closing the dialog.
     */
    dialogCancelHandler (done) {
      this.$store.commit('closeSelectDialog')
      done()
    },

    text (text) {
      switch (text) {
        case 'Choose your dataset': return Drupal.t('Choose your dataset')
        case 'Add new dataset': return Drupal.t('Add new dataset')
        case 'hint': return Drupal.t('Click on the dataset you would like to choose for this action')
      }
    }
  }
}
</script>

<style lang="css">
</style>
