# Personalized redirect app

## API

### Initial data via Drupal.settings
``` js
Drupal.settings.campaignion_wizard.campaignion_wizard--2 = {
  default_redirect_url: 'http://old-default-url.com', // This can be handed over for migration - the app saves everything in the new format. If there is a redirects array with one ore more items, default_redirect_url is ignored.
  redirects: [
    {
      id: 1,
      label: 'My internal label',
      destination: 'node/20',
      prettyDestination: 'Pretty title of my node (20)',
      filters: [
        {
          id: 1,
          type: 'opt-in',
          value: true
        },
        {
          id: 2,
          type: 'submission-field',
          field: 'f_name',
          operator: 'contains',
          value: 'foo'
        }
      ]
    },
    {
      id: 2,
      label: '',
      destination: 'http://example.com',
      prettyDestination: 'http://example.com',
      filters: []
    }
  ],
  fields: [
    // filterable fields in the form submission
    {
      id: 'f_name',
      label: 'First name'
    },
    {
      id: 'l_name',
      label: 'Last name'
    },
    {
      id: 'email',
      label: 'Email address'
    }
  ],
  endpoints: {
    nodes: '/getnodes', // GET nodes list
    redirects: '/node/8/save-my-redirects' // POST redirects
  }
}
```

### Get nodes list
`GET <nodes_endpoint>?s=<search term or nid>`

JSON Response:
``` json
{
  "values": [
    {
      "value": "node/21",
      "label": "My fancy node title (21)"
    },
    ...
  ]
}
```

### Persist data on form submit

``` json
{
  "redirects": [
    {
      "id": null,
      "label": "My internal label",
      "destination": "node/20",
      "prettyDestination": "Pretty title of my node (20)",
      "filters": [
        {
          "id": null,
          "type": "opt-in",
          "value": true
        },
        {
          "id": null,
          "type": "submission-field",
          "field": "f_name",
          "operator": "contains",
          "value": "foo"
        }
      ]
    },
    {
      "id": null,
      "label": "",
      "destination": "http://example.com",
      "prettyDestination": "http://example.com",
      "filters": []
    }
  ]
}
```

The last redirect in the list is the default one. It's not supposed to have either label or filters. Server-side validation only if the user selected the 'custom redirect' radio. If the user changes redirects and afterwards chooses 'Create new thank you page', the app wants to save the changed redirects but does not validate them. They are not used but should be preserved for the future. The server should allow for that.
The `ìd` of everything is `null` when created in the app. The backend gives ids to redirects and filters.
`prettyDestination` has to be saved in the database along with `destination`. If both fields hold the same string, the user entered a custom url. if they differ, `destination` has the format `node/<nid>`.

Operators:

* `==` is
* `!=` is not
* `contains` contains
* `!contains` does not contain
* `regexp` matches regular expression
* `!regexp` doesn’t match regular expression

## Build Setup

``` bash
# install dependencies
yarn install

# serve with hot reload at localhost:8080
yarn dev

# build for production with minification
yarn build

# build nightwatch-xhr for the e2e tests
cd node_modules/nightwatch-xhr
yarn install
cd ../..

# run e2e tests
yarn e2e

# run all tests
yarn test
```

For detailed explanation on how things work, checkout the [guide](http://vuejs-templates.github.io/webpack/) and [docs for vue-loader](http://vuejs.github.io/vue-loader).
