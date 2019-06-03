import initialData from './initial-data.js'

var idCounter = 1

/** @return {integer} A new id. */
function newId () {
  return idCounter++
}

const data = {
  redirects: [
    {
      'id': newId(),
      'label': 'My internal label',
      'destination': 'node/20',
      'prettyDestination': 'Pretty title of my node (20)',
      'filters': [
        {
          'id': newId(),
          'type': 'opt-in',
          'value': true
        },
        {
          'id': newId(),
          'type': 'submission-field',
          'field': 'f_name',
          'operator': 'contains',
          'value': 'foo'
        }
      ]
    },
    {
      'id': newId(),
      'label': 'Spam haters go here',
      'destination': 'http://opt-in.com',
      'prettyDestination': 'http://opt-in.com',
      'filters': [
        {
          'id': newId(),
          'type': 'opt-in',
          'value': false
        }
      ]
    },
    {
      'id': newId(),
      'label': '',
      'destination': 'http://example.com',
      'prettyDestination': 'http://example.com',
      'filters': []
    }
  ]
}

export default Object.assign({}, initialData, data)
