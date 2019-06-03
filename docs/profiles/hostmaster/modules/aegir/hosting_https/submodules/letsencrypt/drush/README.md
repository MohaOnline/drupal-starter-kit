Let's Encrypt
=============

Maintenance
-----------

We include the [Dehydrated script](https://github.com/lukas2511/dehydrated) directly, to avoid issues with packaging scripts on drupal.org. We should strive to only use [released versions](https://github.com/lukas2511/dehydrated/tags) of this script. Either way, we should mention the tag (or, if absolutely required, the commit hash) in the commit message when updating the script.

Basically, this should look something like:

* cd /path/to/this/module/bin/
* wget https://raw.githubusercontent.com/lukas2511/dehydrated/**COMMIT_TAG**/dehydrated
* git diff   # Ensure we're making an atomic commit
* git commit -am"Update dehydrated to **COMMIT_TAG**."

