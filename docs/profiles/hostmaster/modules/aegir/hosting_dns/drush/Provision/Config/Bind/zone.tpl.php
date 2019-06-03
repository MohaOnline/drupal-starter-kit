; Bind zonefile
; File managed by Aegir
; Changes here will be lost during DNS deployment, tread carefully
$TTL <?php print $server->dns_ttl; ?>

<?php
print("@\t\tIN\tSOA\t$server->remote_host $dns_email (
\t\t\t\t" . $serial . " ; serial
\t\t\t\t$server->dns_refresh; refresh
\t\t\t\t$server->dns_retry ; retry
\t\t\t\t$server->dns_expire ; expire
\t\t\t\t$server->dns_negativettl ; minimum
\t\t\t)\n");

print "@\t\tIN\tNS\t" . $server->remote_host;
if ($server->remote_host[strlen($server->remote_host)-1] != '.') {
  print '.';
}
print " ; primary DNS\n";

if (is_array($server->slave_servers_names)) {
  foreach ($server->slave_servers_names as $slave) {
    if ($slave[strlen($slave)-1] != '.') {
      $slave .= '.';
    }
    print "@\t\tIN\tNS\t" . $slave . " ; slave DNS\n";
  }
}

foreach ($records as $name => $entries) {
  foreach ($entries as $record) {
    list($type, $ttl, $destination) = array_values($record);
    print "$name\t$ttl\tIN\t$type\t$destination\n";
  }
}

foreach ($hosts as $host => $info) {
  foreach ($info['A'] as $ip) {
    print "{$info['sub']}\t\tIN\tA\t{$ip}\n";
  }
}
