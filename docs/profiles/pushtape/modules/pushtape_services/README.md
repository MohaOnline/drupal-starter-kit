# Pushtape Services

This module provides services on top of the Drupal Pushtape distribution to allow for
"headless" architecture.

Read more: https://www.getpantheon.com/blog/headless-websites-whats-big-deal

## Provides:
- Cassette.json : https://github.com/zirafa/pushtape-cassette
- .JSPF JSON output for releases (http://xspf.org/jspf/)
- Embeddable iFrame widget for releases

Menu Path | Description
--------  | -----------
pushtape/services/cassette | Outputs cassette.json file
node/[nid]/jspf | Outputs JSPF for specific release
node/[nid]/embed | HTML player markup intended to be delivered via an iFrame
node/[nid]/embed_code | IFrame/embed code wrapped in an input field for easier copying & pasting.

