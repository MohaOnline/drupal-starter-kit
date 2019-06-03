import Vue from 'vue'
import {emptyDataset} from '@/utils/defaults'
import {clone, elementIndex, validateContacts} from '@/utils'
import {findIndex} from 'lodash'

var idCounter = 0

/**
 * Generate dummy ids for new contacts, so they can be identified when editing their fields.
 * These dummy ids are stripped before persisting the contacts to the server, so the server
 * can identify a contact as new by its missing id.
 * @return {string} An id prefixed with 'new', unique during the lifecycle of the app.
 */
function newId () {
  return 'new' + idCounter++
}

/**
 * Generate an array with column keys for the contacts table.
 * Take all the keys from the array of column objects and add an `__error` key at
 * the beginning and eventually a `__delete` key at the end.
 * @param {Object[]} columns - Array of objects describing columns: {key: 'email', title: 'Email address', description: ''}
 * @param {boolean} isCustom - If the dataset is custom and thereby editable, add a __delete column.
 * @return {string[]} Array of column keys for the contacts table.
 */
function filterTableColumns (columns, isCustom) {
  const cols = columns.map(col => col.key)
  cols.splice(0, 0, '__error')
  if (isCustom) {
    cols.push('__delete')
  }
  return cols
}

export default {
  /**
   * Initialize the store.
   * @param {Object} state - vuex state.
   * @param {Object} payload - The mutation’s payload.
   * @param {Object} payload.settings - A settings object with the following properties:
   * @param {string} payload.settings.contactPrefix - A prefix used to identify a contact attribute in a dataset’s attributes list. Defaults to `contact.`.
   * @param {Object[]} payload.settings.standardColumns - An array of objects describing the columns that have to be present in every dataset.
   * @param {Object} payload.settings.validations - Validations for each column: strings containing regular expressions, keyed by columns key.
   * @param {Object} payload.settings.maxFieldLengths - Maximum characters for each column. Dictionary of integers, keyed by column name.
   */
  init (state, {settings}) {
    state.contactPrefix = settings.contactPrefix || 'contact.'
    state.standardColumns = settings.standardColumns || []
    state.validations = settings.validations || {}
    state.maxFieldLengths = settings.maxFieldLengths || {}
  },

  /**
   * Set the list of available datasets.
   * @param {Object} state - vuex state.
   * @param {Object[]} datasets - Array of all available datasets.
   */
  setDatasets (state, datasets) {
    state.datasets = datasets
  },

  /**
   * Update one of the datasets by replacing it with the edited version or append a new
   * dataset to the list.
   * @param {Object} state - vuex state.
   * @param {Object} payload - The mutation’s payload.
   * @param {Object} payload.dataset - The dataset to update or add.
   */
  updateOrAddDataset (state, {dataset}) {
    const i = findIndex(state.datasets, {key: dataset.key})
    if (i > -1) {
      Vue.set(state.datasets, i, dataset)
    } else {
      state.datasets.push(dataset)
    }
  },

  /**
   * Set `selectedDataset` to a clone of a dataset in the list.
   * @param {Object} state - vuex state.
   * @param {Object} payload - The mutation’s payload.
   * @param {string} payload.key - The key of the dataset to select.
   */
  setSelectedDataset (state, {key}) {
    const i = findIndex(state.datasets, {key})
    state.selectedDataset = clone(state.datasets[i])
  },

  /**
   * Set `showSelectDialog` to `true`.
   * @param {Object} state - vuex state.
   */
  openSelectDialog (state) {
    state.showSelectDialog = true
  },

  /**
   * Set `showSelectDialog` to `false`.
   * @param {Object} state - vuex state.
   */
  closeSelectDialog (state) {
    state.showSelectDialog = false
  },

  /**
   * Start editing a clone of a given dataset with a given list of contacts.
   * @param {Object} state - vuex state.
   * @param {Object} payload - The mutation’s payload.
   * @param {Object} payload.dataset - The dataset to edit.
   * @param {Object[]} payload.contacts - The contacts belonging to this dataset.
   */
  editDataset (state, {dataset, contacts}) {
    // Get the columns from the dataset: only columns prefixed with 'contact.'
    const columns = []
    var attribute
    for (var i = 0, j = dataset.attributes.length; i < j; i++) {
      if (dataset.attributes[i].key.indexOf(state.contactPrefix) === 0) {
        attribute = clone(dataset.attributes[i])
        attribute.key = attribute.key.substr(state.contactPrefix.length)
        columns.push(attribute)
      }
    }
    state.currentDataset = clone(dataset)
    state.columns = columns
    state.tableColumns.splice(0, state.tableColumns.length, ...filterTableColumns(state.columns, state.currentDataset.is_custom)) // don’t replace with a new array to keep table binding
    state.contacts.splice(0, state.contacts.length, ...contacts) // don’t replace with a new array to keep table binding
    state.datasetChanged = false
    state.showEditDialog = true
  },

  /**
   * Start editing a new dataset, providing the standard columns and no contacts in the contacts table.
   * @param {Object} state - vuex state.
   */
  editNewDataset (state) {
    state.currentDataset = emptyDataset(state)
    state.columns = clone(state.standardColumns)
    state.tableColumns.splice(0, state.tableColumns.length, ...filterTableColumns(state.columns, state.currentDataset.is_custom)) // don’t replace with a new array to keep table binding
    state.contacts.splice(0, state.contacts.length) // don’t replace with a new array to keep table binding
    state.datasetChanged = false
    state.showEditDialog = true
  },

  /**
   * Set `showEditDialog` to `false`.
   * @param {Object} state - vuex state.
   */
  closeEditDialog (state) {
    state.showEditDialog = false
  },

  /**
   * Set the visibility of the spinner in the App component.
   * @param {Object} state - vuex state.
   * @param {boolean} val - `true` shows the spinner, `false` hides it.
   */
  showSpinner (state, val) {
    state.showSpinner = !!val
  },

  /**
   * Set the `apiError` flag to indicate that the API couldn’t be reached.
   * @param {Object} state - vuex state.
   * @param {boolean} val - `true` indicates an error.
   */
  setApiError (state, val) {
    state.apiError = !!val
  },

  /**
   * Add a new contact to the list.
   * @param {Object} state - vuex state.
   */
  addContact (state) {
    if (!state.currentDataset.is_custom) return
    const newContact = {
      id: newId() // We need ids to identify rows when they are clicked. These dummy ids are removed before POSTing.
    }
    for (var i = 0, j = state.columns.length; i < j; i++) {
      newContact[state.columns[i].key] = ''
    }
    // Maybe the contact is invalid in an empty state... Let’s check:
    validateContacts({
      contacts: [newContact],
      validations: state.validations,
      maxFieldLengths: state.maxFieldLengths
    })
    state.contacts.push(newContact)
    state.datasetChanged = true
  },

  /**
   * Remove a contact from the list.
   * @param {Object} state - vuex state.
   * @param {string} id - The id of the contact to remove.
   */
  deleteContact (state, id) {
    const i = findIndex(state.contacts, {id})
    state.contacts.splice(i, 1)
    state.datasetChanged = true
  },

  /**
   * Handle a click on a contact table row.
   * @param {Object} state - vuex state.
   * @param {Object} payload - The mutation’s payload.
   * @param {Object} payload.row - The contact that has been clicked.
   * @param {Event} payload.event - The native click event.
   */
  'contactsTable/ROW_CLICK' (state, {row, event}) {
    // Don’t open the popup on another cell if it’s already open.
    if (state.editValue) return
    const el = event.target
    // Return if it’s the __delete cell (but not the link).
    if (el.children[0] && el.children[0].classList.contains('dsa-delete-contact')) return
    const cellIndex = elementIndex(el)
    const col = state.contactsTable.columns[cellIndex]
    // Return if user clicked the error cell.
    if (cellIndex === 0 && state.contactsTable.columns[0] === '__error') return
    // TODO v2: in non-custom datasets, check if column may be edited
    state.editValue = {
      id: row.id,
      row,
      col,
      el
    }
  },

  /**
   * Stop editing a contact.
   * @param {Object} state - vuex state.
   */
  leaveValue (state) {
    state.editValue = null
  },

  /**
   * Update a field of the contact currently being edited.
   * @param {Object} state - vuex state.
   * @param {Object} payload - The mutation’s payload.
   * @param {string} payload.value - The new value of the field.
   */
  updateValue (state, {value}) {
    if (!state.editValue) return
    const i = findIndex(state.contacts, {id: state.editValue.id})
    state.contacts[i][state.editValue.col] = value
    state.editValue = null
    state.datasetChanged = true
    // Check if the row (still) has an error.
    validateContacts({
      contacts: state.contacts,
      validations: state.validations,
      maxFieldLengths: state.maxFieldLengths,
      index: i
    })
  },

  /**
   * Process a list of contacts from a CSV file and store it in `contacts`.
   * Strip fields that are not present in `standardColumns`, and add standard fields if necessary.
   * Add dummy ids necessary for editing values.
   * @param {Object} state - vuex state.
   * @param {Object[]} contacts - The list of contacts we got from the server.
   */
  setContacts (state, contacts) {
    // TODO v2: derive columns from CSV and set accordingly
    // for now: clear additional fields
    const c = clone(contacts)
    const allowedFields = state.standardColumns.map(col => col.key)
    for (let i = 0, j = c.length; i < j; i++) {
      // Delete extra fields.
      for (let key in c[i]) {
        if (c[i].hasOwnProperty(key) && allowedFields.indexOf(key) === -1) {
          delete c[i][key]
        }
      }
      // Add missing fields.
      for (let ii = 0, jj = allowedFields.length; ii < jj; ii++) {
        if (typeof c[i][allowedFields[ii]] === 'undefined') {
          c[i][allowedFields[ii]] = ''
        }
      }
      // Add dummy id, will be removed before saving.
      c[i].id = newId()
    }
    state.columns = clone(state.standardColumns)
    state.tableColumns.splice(0, state.tableColumns.length, ...filterTableColumns(state.columns, state.currentDataset.is_custom)) // don’t replace with a new array to keep table binding
    state.contacts.splice(0, state.contacts.length, ...c) // don’t replace with a new array to keep table binding
    state.datasetChanged = true
  },

  /**
   * Apply validations to `contacts`.
   * @param {Object} state - vuex state.
   */
  validateContacts (state) {
    validateContacts({
      contacts: state.contacts,
      validations: state.validations,
      maxFieldLengths: state.maxFieldLengths
    })
  },

  /**
   * Set the currently edited dataset’s title and the `datasetChanged` flag.
   * @param {Object} state - vuex state.
   * @param {string} title - The new title.
   */
  updateTitle (state, title) {
    state.currentDataset.title = title
    state.datasetChanged = true
  },

  /**
   * Set the currently edited dataset’s description and the `datasetChanged` flag.
   * @param {Object} state - vuex state.
   * @param {string} description - The new description.
   */
  updateDescription (state, description) {
    state.currentDataset.description = description
    state.datasetChanged = true
  }
}
