Hosting CiviCRM tests
=====================

This is the test suite of hosting_civicrm. It runs using phpunit and is intended
to run against an already installed Aegir instance. This is work-in-progress.

Configuration
-------------

You might want to put this in your /var/aegir/config/includes/global.inc:

```
<?php

@ini_set('mbstring.http_output', 'pass');
@ini_set('mbstring.http_input', 'pass');
```

For more information:  
https://docs.acquia.com/articles/php-56-and-mbstringhttpinput-errors

Running the tests
-----------------

For example, this is what hosting_civicrm uses with Jenkins. Change HOSTMASTER_SITE for your hostmaster uri:

```
HOSTMASTER_SITE="erbil.bidon.ca"
HOSTMASTER_ROOT=`sudo -u aegir grep root /var/aegir/config/server_master/nginx/vhost.d/${HOSTMASTER_SITE} | awk '{print $2}' | grep hostmaster | sed "s/;//"`

sudo -u aegir rm -fr ${HOSTMASTER_ROOT}/profiles/hostmaster/modules/aegir/hosting_civicrm/

# so that jenkins (in the aegir group) can clone/update files.
sudo -u aegir chmod g+w ${HOSTMASTER_ROOT}/profiles/hostmaster/modules/aegir

cd ${HOSTMASTER_ROOT}/profiles/hostmaster/modules/aegir
sudo -u aegir git clone https://github.com/mlutfy/hosting_civicrm.git

cd hosting_civicrm
sudo -u aegir git fetch origin +refs/pull/*:refs/remotes/origin/pr/*
sudo -u aegir git checkout "$sha1"

sudo -i -u aegir drush @hm cc drush
sudo -i -u aegir drush @hm cc all

# NB: for xml results, jenkins expects things to be in its workspace (/home/jenkins/workspace)
# Allow the aegir user to write the tests (nb: adduser aegir jenkins)
chmod g+w ${WORKSPACE}/tests

cd ${HOSTMASTER_ROOT}/profiles/hostmaster/modules/aegir/hosting_civicrm
sudo -u aegir composer install
sudo -u aegir phpunit --configuration tests --log-junit ${WORKSPACE}/tests/results.xml
```
