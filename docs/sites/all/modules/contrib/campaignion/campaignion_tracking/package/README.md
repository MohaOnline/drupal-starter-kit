# campaignion_tracking

Common tracking functions.

## Development

Install `nodejs` and `yarn`, then install the needed dependencies:

    apt install nodejs yarn
    yarn install

Use the different `yarn` scripts for the development workflow:

    yarn run lint
    yarn run test
    yarn run dev

For building a releaseable artifact (library file) use:

    yarn run dist

The development files are configured to be created under `build/`, the
releaseable files are created under `dist/`.

For Drupal copy the releaseable files to `../js`:

    yarn run drupal

