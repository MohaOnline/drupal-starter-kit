<template>
  <div class="datasets-app">
    <div v-if="livingInWizard" v-loading="showSpinner && !showSelectDialog && !showEditDialog" class="dsa-wizard-step">
      <div class="ae-legend">{{ text('Your targets') }}</div>
      <div class="dsa-intro-text" v-html="introText"></div>
      <el-button type="button" @click="openDialog" :disabled="apiError || showSpinner" class="dsa-select-or-edit-dataset">{{ buttonText }}</el-button>
    </div>
    <div v-if="apiError" class="dsa-has-error">{{ text('api error') }}</div>

    <SelectDatasetDialog />
    <EditDatasetDialog />
  </div>
</template>

<script>
import {mapState} from 'vuex'
import SelectDatasetDialog from '@/components/SelectDatasetDialog'
import EditDatasetDialog from '@/components/EditDatasetDialog'
import {clone} from '@/utils'

export default {
  name: 'app',

  components: {
    SelectDatasetDialog,
    EditDatasetDialog
  },

  data: function () {
    return {
      livingInWizard: !!this.$root.$options.datasetField // The presence of a dataset field tells us whether the app is mounted in the wizard.
    }
  },

  computed: {
    /**
     * @return {string} - A sentence informing the user about the chosen dataset or telling them to choose one.
     */
    introText () {
      return this.selectedDataset
        ? Drupal.t('You have chosen the dataset <strong>"@dataset".</strong> If you would like to edit the dataset or choose a different one click the "edit" button.', {'@dataset': this.selectedDataset.title})
        : Drupal.t('Click the button to choose a dataset.')
    },

    /**
     * @return {string} - Label of the wizard button to choose a dataset.
     */
    buttonText () {
      return this.selectedDataset
        ? Drupal.t('Edit your target dataset')
        : Drupal.t('Choose your target dataset')
    },

    ...mapState([
      'selectedDataset',  /** {(Object|null)} The dataset currently selected in the wizard field. */
      'apiError',         /** {boolean} Was there an error when trying to reach the API? */
      'showSelectDialog', /** {boolean} Is the dialog to select a dataset visible? */
      'showEditDialog',   /** {boolean} Is the dialog to edit a dataset visible? */
      'showSpinner'       /** {boolean} Is the loading spinner visible? */
    ])
  },

  watch: {
    // When another dataset is selected, update the the wizard dataset field.
    selectedDataset (dataset) {
      if (this.livingInWizard && dataset) {
        this.$root.$options.datasetField.value = dataset.key
      }
    },
    // Don’t accidentially submit drupal form while dialogs are open.
    showSelectDialog (val) {
      this.disableDrupalSubmits(val)
    },
    showEditDialog (val) {
      this.disableDrupalSubmits(val)
    }
  },

  methods: {
    /**
     * Handle the select/edit button.
     * If a custom dataset is selected, edit it. If not, open the dialog to select a dataset.
     */
    openDialog () {
      if (this.selectedDataset && this.selectedDataset.is_custom) {
        this.$store.dispatch({type: 'loadContactsAndEdit', dataset: this.selectedDataset})
      } else {
        this.$store.commit('openSelectDialog')
      }
    },

    /**
     * Set the disabled status of the Campaignion wizard buttons.
     * @param {boolean} bool - Set the disabled attribute to this value.
     */
    disableDrupalSubmits (bool) {
      const inputs = document.querySelectorAll('input[type=submit]')
      for (var i = 0, j = inputs.length; i < j; i++) {
        inputs[i].disabled = bool
      }
    },

    text (text) {
      switch (text) {
        case 'api error': return Drupal.t('The email to target API couldn’t be reached. Please reload the page.')
        case 'Your targets': return Drupal.t('Your targets')
      }
    }
  },

  created () {
    // Initialize some properties in the store.
    this.$store.commit({
      type: 'init',
      settings: clone(this.$root.$options.settings)
    })
    // Load all datasets from the server and set the selected dataset (wizard only).
    this.$store.dispatch({
      type: 'loadDatasets',
      selected: this.livingInWizard ? this.$root.$options.datasetField.value : undefined
    })
  }
}
</script>

<style>
li.VuePagination__pagination-item {
    display: inline-block;
    list-style-type: none;
    margin-right: 1rem;
}

td {
  height: 1rem; /* Needed for the contained div with height: 100% */
  white-space: nowrap;
  padding: 0;
}
td.dsa-edited {
  background-color: yellow;
}
td .dsa-contact-field {
  height: 100%; /* Don’t collapse if empty! */
  pointer-events: none; /* So you can click 'through' the div and reach the td... */
  padding: 0 0.7rem;
}
td .dsa-contact-field-invalid {
  background-color: rgba(255, 0, 0, 0.3);
}
td.dsa-edited .dsa-contact-field-invalid {
  background-color: transparent;
}

.dsa-flash {
  -webkit-animation-name: dsa-flash-animation;
  -webkit-animation-duration: 1s;
  animation-name: dsa-flash-animation;
  animation-duration: 1s;
}

@-webkit-keyframes dsa-flash-animation {
  0% { background: transparent; }
  10% { background: #d66540; }
  100% { background: transparent; }
}

@keyframes dsa-flash-animation {
  0% { background: transparent; }
  10% { background: #d66540; }
  100% { background: transparent; }
}

.v-tooltip {
  display: block !important;
  z-index: 10000;
}
.v-tooltip .tooltip-inner {
  background-color: #ddd;
}
</style>
