#!/usr/bin/perl -Tw
    use strict;
    BEGIN { $ENV{PATH} = "/usr/bin:/bin" }
    use Socket;
    use Carp;
    my $EOL = "\015\012";
    sub logmsg { print "$0 $$: @_ at ", scalar localtime(), "\n" }
    my $port  = shift || 2345;
    die "invalid port" unless $port =~ /^ \d+ $/x;
    my $proto = getprotobyname("tcp");
    socket(Server, PF_INET, SOCK_STREAM, $proto)    || die "socket: $!";
    setsockopt(Server, SOL_SOCKET, SO_REUSEADDR, pack("l", 1))    
                                                    || die "setsockopt: $!";
    bind(Server, sockaddr_in($port, INADDR_ANY))    || die "bind: $!";
    listen(Server, SOMAXCONN)                       || die "listen: $!";
    logmsg "server started on port $port";
    my $paddr;
    $SIG{CHLD} = \&REAPER;
	$| = 1;
	open LOG,">>","/home/local/ANT/icadri/log.txt";
    for ( ; $paddr = accept(Client, Server); close Client) {
        my($port, $iaddr) = sockaddr_in($paddr);
        my $name = gethostbyaddr($iaddr, AF_INET);
        logmsg "connection from $name [",
                inet_ntoa($iaddr), "]
                                at port $port";
        print Client "Hello there, $name, it's now ",
                        scalar localtime(), $EOL;


	print LOG "$name $iaddr $port ".scalar localtime()."\n"; 
    }
	close LOG;
