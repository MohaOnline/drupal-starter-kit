import initialData from './initial-data.js'

var data
var idCounter = 1

/** @return {integer} a new id. */
function newId () {
  return idCounter++
}

data = {

  messageSelection: [

    {
      'id': newId(),
      'type': 'message-template',
      'label': 'foo',
      'filters': [
        {
          'id': newId(),
          'type': 'target-attribute',
          'attributeName': 'contact.political_affiliation',
          'operator': '==',
          'value': 'Green Party'
        }
      ],
      'url': '',
      'urlLabel': '',
      'message': {
        'subject': 'Subject of first message',
        'header': 'Header of first message',
        'body': 'body of first msg',
        'footer': 'goodbye'
      }
    },

    {
      'id': newId(),
      'type': 'exclusion',
      'label': 'foo',
      'filters': [
        {
          'id': newId(),
          'type': 'target-attribute',
          'attributeName': 'contact.political_affiliation',
          'operator': '!=',
          'value': 'Labour'
        }
      ]
    },

    {
      'id': newId(),
      'type': 'message-template',
      'label': 'shares a filter with first message',
      'filters': [
        {
          'id': newId(),
          'type': 'target-attribute',
          'attributeName': 'contact.first_name',
          'operator': '!=',
          'value': 'jane'
        },
        {
          'id': newId(),
          'type': 'target-attribute',
          'attributeName': 'contact.political_affiliation',
          'operator': '==',
          'value': 'Green Party'
        }
      ],
      'url': '',
      'urlLabel': '',
      'message': {
        'subject': 'Subject of 3rd message',
        'header': 'Header of 3rd message',
        'body': 'body of 3rd msg',
        'footer': 'goodbye'
      }
    },

    {
      'id': newId(),
      'type': 'exclusion',
      'label': 'shares both filters with preceding message',
      'filters': [
        {
          'id': newId(),
          'type': 'target-attribute',
          'attributeName': 'contact.political_affiliation',
          'operator': '==',
          'value': 'Green Party'
        },
        {
          'id': newId(),
          'type': 'target-attribute',
          'attributeName': 'contact.first_name',
          'operator': '!=',
          'value': 'jane'
        }
      ]
    },

    {
      'id': newId(),
      'type': 'message-template',
      'label': 'same filter as message above, empty message',
      'filters': [
        {
          'id': newId(),
          'type': 'target-attribute',
          'attributeName': 'contact.first_name',
          'operator': '!=',
          'value': 'jane'
        }
      ],
      'url': '',
      'urlLabel': '',
      'message': {
        'subject': '',
        'header': '   ',
        'body': '\n\r',
        'footer': ''
      }
    },

    {
      'id': newId(),
      'type': 'exclusion',
      'label': 'exclusion without a filter',
      'filters': []
    },

    {
      'id': newId(),
      'type': 'message-template',
      'label': 'message with a previously used filter and a missing filter value',
      'filters': [
        {
          'id': newId(),
          'type': 'target-attribute',
          'attributeName': 'contact.first_name',
          'operator': '!=',
          'value': 'jane'
        },
        {
          'id': newId(),
          'type': 'target-attribute',
          'attributeName': 'contact.political_affiliation',
          'operator': '==',
          'value': ''
        }
      ],
      'url': '',
      'urlLabel': '',
      'message': {
        'subject': 'foo',
        'header': 'bar',
        'body': 'baz',
        'footer': 'bam'
      }
    },

    {
      'id': newId(),
      'type': 'message-template',
      'label': '',
      'filters': [],
      'url': '',
      'urlLabel': '',
      'message': {
        'subject': 'default message subject',
        'header': 'default message header',
        'body': 'default message body',
        'footer': 'default message footer'
      }
    }

  ],

  hardValidation: true

}

export default Object.assign({}, initialData, data)
