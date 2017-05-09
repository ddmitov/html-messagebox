#!/usr/bin/perl

use strict;
use warnings;
use POSIX qw(strftime);

use AnyEvent;

my $TIMEOUT = 10;

my $elapsed_time = 0;
my $remaining_time;

my $pid = open (HTMLMSG, "|-",
  "./htmlmsg", "--width=400", "--heigth=200", "--timeout=$TIMEOUT") or
  die "Could not fork: $!\n";

select (HTMLMSG);
$|=1;

# Set the event loop:
my $event_loop = AnyEvent->condvar;

my $wait_one_second = AnyEvent->timer (
  after => 0,
  interval => 1,
  cb => sub {
      my $remaining_time = $TIMEOUT - $elapsed_time;
      $elapsed_time++;

      print HTMLMSG "Remaining seconds: $remaining_time\n";

      if ($elapsed_time == $TIMEOUT) {
        exit();
      }
  },
);

$event_loop->recv;
