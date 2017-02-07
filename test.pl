#!/usr/bin/perl

use strict;
use warnings;
use AnyEvent;
use POSIX qw(strftime);

my $TIMEOUT = 10;
my $current_time;
my $elapsed_time = 0;

my $pid = open (HTMLMSG, "|-",
  "./htmlmsg", "--width=400", "--heigth=200", "--timeout=12") or
  die "Could not fork: $!\n";

# Set the event loop:
my $event_loop = AnyEvent->condvar;

my $wait_one_second = AnyEvent->timer (
  after => 0,
  interval => 1,
  cb => sub {
      $current_time = time;
      select (HTMLMSG);
      $|=1;
      print HTMLMSG "Seconds from the Unix epoch:<br>$current_time\n";

      $elapsed_time++;
      if ($elapsed_time == $TIMEOUT) {
        exit();
      }
  },
);

$event_loop->recv;
