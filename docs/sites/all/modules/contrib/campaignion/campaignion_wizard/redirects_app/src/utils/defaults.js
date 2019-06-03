export const OPERATORS = {
  '==': {
    label: Drupal.t('is'),
    phrase: Drupal.t('@attribute is @value')
  },
  '!=': {
    label: Drupal.t('is not'),
    phrase: Drupal.t('@attribute is not @value')
  },
  'contains': {
    label: Drupal.t('contains'),
    phrase: Drupal.t('@attribute contains @value')
  },
  '!contains': {
    label: Drupal.t('does not contain'),
    phrase: Drupal.t('@attribute doesn’t contain @value')
  },
  'regexp': {
    label: Drupal.t('matches'),
    phrase: Drupal.t('@attribute matches @value')
  },
  '!regexp': {
    label: Drupal.t('does not match'),
    phrase: Drupal.t('@attribute doesn’t match @value')
  }
}

/**
 * Redirect factory.
 * @return {Object} A new redirect object with an `id` of `null`.
 */
export function emptyRedirect () {
  return {
    id: null,
    label: '',
    destination: '',
    prettyDestination: '',
    filters: []
  }
}

export default {
  OPERATORS,
  emptyRedirect
}
