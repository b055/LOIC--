#!/usr/bin/perl -w

use Socket;
$proto = getprotobyname('tcp');
my ($sock);
socket($sock,AF_INET,SOCK_STREAM,$proto) or die "could not create socket: $!";

$remote = "www.google.com";
$port = 80;

$iaddr = inet_aton($remote) or die "Unable to resolve hostname : $remote";

$paddr = sockaddr_in($port,$iaddr);

connect($sock,$paddr) or die "connect failed : $!";
print "connected to $remote on port $port\n";

send ($sock, "GET / HTTP/1.1\r\n\r\n", 0 ) or die "send faild: $!";

while($line= <$sock>)
{
print $line;
}
close($sock);
exit(0);
