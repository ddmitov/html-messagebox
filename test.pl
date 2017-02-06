#!/usr/bin/perl

use strict;
use warnings;

# The position or precedence of any command line argument is of no importance.
my $pid = open (HTMLMSG, "| ./htmlmsg --width=400 --heigth=200 --timeout=15") or
  die "Could not fork: $!\n";

print HTMLMSG <<HTML;
<html>

  <head>
    <meta http-equiv='Content-Type' content='text/html; charset=utf-8'>
    <style type='text/css'>
      body {
        color: #ffffff;
        background-color: #222222;
        -webkit-text-size-adjust: 100%;
      }
      a {
        color: #ffffff;
      }
      a:visited {
        color: #ffffff;
      }
    </style>
  </head>

  <body>
    <table width='100%' height='100%'>
      <tr>
        <td style='vertical-align: middle'>
          <p align='center'><font size='4' face='Sans'>
             HTML Message Box Test Screen<br>
             <a href='close://'>Click here to close</a>
          </font></p>
        </td>
      </tr>
    </table>
  </body>

</html>
HTML

close (HTMLMSG) or die "Could not close: $!\n";
