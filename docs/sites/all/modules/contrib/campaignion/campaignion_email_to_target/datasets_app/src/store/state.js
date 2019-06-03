const state = {
  datasets: [],            /** {Object[]} Array of all available datasets. */
  currentDataset: null,    /** {(Object|null)} The dataset being edited. */
  selectedDataset: null,   /** {(Object|null)} The dataset currently selected in the wizard field. */
  contacts: [],            /** {Object[]} Array of contacts belonging to the current dataset. */
  contactPrefix: '',       /** {string} A prefix used to identify a contact attribute in a dataset’s attributes list. */
  columns: [],             /** {Object[]} Array of objects describing each column in the current dataset: {key: 'foo', title: 'Foo', description: 'The foo column.'} */
  tableColumns: [],        /** {string[]} Array of column identifiers for the columns that are shown in the table. */
  standardColumns: [],     /** {Object[]} Array of objects describing the columns that have to be present in every dataset. */
  validations: {},         /** {Object} Validations for each column. Dictionary of regex strings, keyed by column name. */
  maxFieldLengths: {},     /** {Object} Maximum characters for each column. Dictionary of integers, keyed by column name. */
  editValue: null,         /** {(Object|null)} While a contact is being edited: { id: contact.id, row: Object, col: key for row, el: td element } */
  showSelectDialog: false, /** {boolean} Is the dialog to select a dataset visible? */
  showEditDialog: false,   /** {boolean} Is the dialog to edit a dataset visible? */
  showSpinner: false,      /** {boolean} Is the loading spinner in the App component visible? */
  datasetChanged: false,   /** {boolean} True if the user has made changes on the current dataset. */
  apiError: false          /** {boolean} Was there an error when trying to reach the API? */
}

export default state
