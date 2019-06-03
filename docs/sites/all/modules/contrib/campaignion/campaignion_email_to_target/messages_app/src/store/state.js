const state = {
  specs: [],              /** {Object[]} Specifications for specific targets, these are either of type `message-template` or `exclusion`. */
  currentSpecIndex: null, /** {(integer|null)} The index of the item in the specs array that is currently being edited or `-1` for a new item, or `null`, if no item is being edited. */
  defaultMessage: {},     /** {Object} The spec object representing the message sent to all remaining targets. */
  targetAttributes: [],   /** {Object[]} Collection of objects describing the target attributes: {name: 'contact.email', label: 'Email address', description: ''} */
  tokenCategories: [],    /** {Object[]} Collection of categories with a `title`, a `description` and a collection of `tokens`. The `tokens` each have a `title`, a `description` and a `token`. */
  hardValidation: false,  /** {boolean} Don’t persist the data if there are validation errors. */
  initialData: {}         /** {Object} The `defaultMessage` and `specs` that were initially loaded. */
}

export default state
