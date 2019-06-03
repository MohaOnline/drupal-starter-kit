#!/usr/bin/env bash

# Copied from dehydrated/docs/examples/hook.sh

function deploy_challenge {
    local DOMAIN="${1}" TOKEN_FILENAME="${2}" TOKEN_VALUE="${3}"

    # This hook is called once for every domain that needs to be
    # validated, including any alternative names you may have listed.
    #
    # Parameters:
    # - DOMAIN
    #   The domain name (CN or subject alternative name) being
    #   validated.
    # - TOKEN_FILENAME
    #   The name of the file containing the token to be served for HTTP
    #   validation. Should be served by your web server as
    #   /.well-known/acme-challenge/${TOKEN_FILENAME}.
    # - TOKEN_VALUE
    #   The token value that needs to be served for validation. For DNS
    #   validation, this is what you want to put in the _acme-challenge
    #   TXT record. For HTTP validation it is the value that is expected
    #   be found in the $TOKEN_FILENAME file.

    # Since dehydrated does not always call us with the main domain name,
    # use a drush alias passed in from the original invocation via the
    # environment variable AEGIR_DRUSH_ALIAS to ensure we find
    # the correct context.
    local MAIN_DOMAIN="${AEGIR_DRUSH_ALIAS:-@$DOMAIN}"

    drush php-eval "d('$MAIN_DOMAIN')->service('http')->sync(d('@server_master')->aegir_root . '/config/letsencrypt.d/well-known/acme-challenge');"
}

HANDLER="$1"; shift
if [[ "${HANDLER}" =~ ^(deploy_challenge)$ ]]; then
  "$HANDLER" "$@"
fi

