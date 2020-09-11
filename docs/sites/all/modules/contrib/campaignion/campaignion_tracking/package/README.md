# campaignion_tracking

Common tracking functions.

These scripts assume that the tracking snippets have already been loaded,
e.g. it expects `dataLayer` to be available in case of GTM.

## Concepts

There is shared functionality for dispatching tracking events onto a PubSub bus.
The tracker specific implementations can then subscribe to events and handle
these according to their upstream API.

There is also a fragment listener provided which checks URL fragments for
some defined tracking signals. If some are found they are consumed and
corresponding tracking events are dispatched onto the PubSub bus.

The Drupal side can provide "tracking contexts" but enriching a data
structure in the `Drupal.settings` object. The common tracking functionality
will read this and provide it as context to dispatched events.
Examples are node id, node title, donation information.

In theme specific code you can implement a callback to alter the events at
the latest possibility (just before they are sent to the upstream API).
Therefor you will need to implement a
`window.campaignion_tracking_change_msg()` function.
Currently only implemented for GTM.

The different between tracking data and tracking context:
"data" will be sent upstream as-is, a tracking "context" can be used in the
specfici implementations to generate or enhance the data sent.

## Channels

Channels implemented so far:

- `code`
- `donation`
- `webform`

## Contexts

- `node`
- `webform`
- `donation`

## Codes

- `t`: tracking event
- `w`: webform data
- `d`: donation data

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

You can enable debug verbosity of the JS by setting
`sessionStorage.setItem('campaignion_debug', 1)` (then reloading).
