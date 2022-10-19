# drupal-campaignion-loqate

JavaScript for the loqate integration.

## Development

Install `nodejs` and `yarn`:

    apt install nodejs yarnpkg  # debian
    emerge -av yarn  # gentoo

… then install the JS dependencies …

    yarn install

Use the different `yarn` scripts for the development workflow:

    yarn lint
    yarn test
    yarn dev

For building a releaseable artifact (library file) use:

    yarn dist

The development files are configured to be created under `build/`, the
releaseable files are created under `dist/`.

For Drupal copy the releaseable files to `..`:

    yarn drupal
