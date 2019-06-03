#!/bin/bash

if [ $(id -u) != 0 ]; then
  printf "***********************************************\n"
  printf "* Error: You must run this with sudo or root. *\n"
  printf "***********************************************\n"
  exit 1
fi

DIR=$(dirname "$0")
SCRIPTS=(fix-drupal-platform-permissions fix-drupal-site-permissions)

for SCRIPT in ${SCRIPTS[@]}; do
  cp ${DIR}/${SCRIPT}.sh /usr/local/bin
  chown root:root /usr/local/bin/${SCRIPT}.sh
  chmod u+x /usr/local/bin/${SCRIPT}.sh
  echo "aegir ALL=NOPASSWD: /usr/local/bin/${SCRIPT}.sh" > /etc/sudoers.d/${SCRIPT}
  chmod 0440 /etc/sudoers.d/${SCRIPT}
done
