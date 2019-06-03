export const VALID_SPECIFICATION_TYPES = ['message-template', 'exclusion']

export const OPERATORS = {
  '==': {
    label: Drupal.t('is'),
    phrase: Drupal.t('@attribute is @value')
  },
  '!=': {
    label: Drupal.t('is not'),
    phrase: Drupal.t('@attribute is not @value')
  },
  'regexp': {
    label: Drupal.t('matches'),
    phrase: Drupal.t('@attribute matches @value')
  }
}

/**
 * Message object factory.
 * @return {Object} Object representing an empty message.
 */
export function messageObj () {
  return {
    subject: '',
    header: '',
    body: '',
    footer: ''
  }
}

/**
 * Specification factory.
 * @param {string} type - Pass the type of spec you want to create.
 * @return {Object} A new spec object with an `id` of `null`.
 */
export function emptySpec (type) {
  if (VALID_SPECIFICATION_TYPES.indexOf(type) === -1) return
  return {
    id: null,
    type: type,
    label: '',
    filters: [],
    message: messageObj(),
    url: '',
    urlLabel: '',
    errors: []
  }
}

export default {
  VALID_SPECIFICATION_TYPES,
  OPERATORS,
  messageObj,
  emptySpec
}
