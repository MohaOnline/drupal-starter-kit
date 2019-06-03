Hosting DNS
===========

About the module.


Pre-installation instructions
=============================

Make sure to install `bind9` and `bind9utils` on your server. (Other DNS 
servers might be supported later, patches are welcome.)
(For RHEL based Linux these are named `bind` and `bind-utils`.)

Edit your sudoers file using `visudo` and give the aegir user permissions to 
reload `rndc` and to use `named-checkconf`. There should already be a permission 
to execute the webserver and combined it should look somewhat like this:

    Defaults: aegir !requiretty
    aegir ALL=NOPASSWD: /usr/sbin/apache2ctl, /usr/sbin/rndc, /usr/sbin/named-checkconf


Master server configuration
===========================

Edit your DNS server config file to include the aegir generated configuration. 
Append the following to `/etc/named.conf`: 

     include "/var/aegir/config/bind.conf";
     
DO NOT RELOAD BIND, it will fail because the file is not there yet.

Then add the DNS service in the frontend. This should trigger a server
verification and configure the DNS service in the backend. Creating a new site
or deploying the DNS of an existing one should then create a zone, some records
and reload the DNS server.


Testing
-------

To test if the deployment worked, you can use the `dig` program, which comes 
with the `bind9utils` package. Run the following command, and replace localhost 
with your server and example.com with your zone:

    dig @localhost example.com axfr

The response should be similar to this:

    ; <<>> DiG 9.9.4-RedHat-9.9.4-29.el7_2.4 <<>> @localhost example.com axfr
    ; (2 servers found)
    ;; global options: +cmd
    example.com.            86400   IN      SOA     vps1.com.example.com. admin.vps1.com.example.com. 2016103000 21600 3600 604800 86400
    example.com.            86400   IN      NS      vps1.com.
    example.com.            86400   IN      A       127.0.0.1
    example.com.            86400   IN      SOA     vps1.com.example.com. admin.vps1.com.example.com. 2016103000 21600 3600 604800 86400
    ;; Query time: 2 msec
    ;; SERVER: ::1#53(::1)
    ;; WHEN: Sun Oct 30 17:31:27 CET 2016
    ;; XFR size: 4 records (messages 1, bytes 184)


Slave server configuration
==========================

(Not yet fully tested and supported.)

A slave server requires the following steps:

 1. install `bind`, `sudo` and `rsync` on the server.
 2. create an aegir user on the server the usual way (including SSH key exchange 
    and the rndc reload permission in the sudoers file).
 3. create the server in the frontend with the bind_slave service.
 4. configure the master server to use the slave.

Then running the tests described in the master configuration should create a
config file in `/var/aegir/config/bind_slave.conf` that you need to include in
your bind configuration. The config file should look something like this:

    zone "foobar.com" { type slave; file "/var/hostmaster/config/server_ns4koumbitnet/bind_slave/zone.d/foobar.com.zone"; masters { 1.2.3.4; }; allow-query { any; }; };


Caveats
=======

 1. changing the master/slave relationship doesn't change the zonefiles unless 
    every zonefile is verified again.
