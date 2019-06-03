# dataset_app

Select and edit datasets via the e2t_api.
The app can be loaded in two environments: The wizard’s target step and as a standalone dataset manager (the latter is yet to be implemented).

## Dev mode

When running `yarn dev`, an instance of json-server is launched and used as the development API. You can use the real API though by setting the `E2T_API_TOKEN` environment variable to a valid JWT token.

``` bash
E2T_API_TOKEN="<here goes the token>" yarn dev
```

## Production mode

In production mode, Webpack is configured to leave certain vendor libraries out of the bundle and take them from the `campaignion_vue` global provided by the `campaignion_vue` module.
The app uses the API url and token passed via `Drupal.settings.campaignion_email_to_target.endpoints['e2t-api']`.

## Usage

``` bash
# install dependencies
yarn install

# serve with hot reload at localhost:8080
yarn dev

# build for production with minification
yarn build

# run e2e tests in Chrome
yarn e2e
```

## Settings

The app needs the following settings in the `Drupal.settings.campaignion_email_to_target` dictionary:

| Key               | Type     | Value                                                                                                           | Example                                                          | Default |
|-------------------|----------|-----------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------|---------|
| `contactPrefix`   | string   | A prefix used to identify a contact attribute in a dataset’s attributes list.                                   | `'contact.'`                                                     | `''`    |
| `standardColumns` | Object[] | Array of objects describing the columns that have to be present in every dataset.                               | `[{key: 'email', description: '', title: 'Email address'}, ...]` | `[]`    |
| `validations`     | Object   | Validations for each column. Dictionary of regex strings, keyed by column name. Backslashes have to be escaped! | `{'first_name': '\\S+'}`                                         | `{}`    |
| `endpoints`       | Object   | For now, this is only the `e2t-api` endpoint.                                                                   | `{'e2t-api': {url: 'http://example.com', token: 'JWT token'}}`   |         |
