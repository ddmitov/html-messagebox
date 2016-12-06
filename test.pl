#!/usr/bin/perl -w

use strict;
use warnings;

# The position or precedence of any command line argument is of no importance at all.
my $pid = open(HTMLMSG, "| ./htmlmsg --width=400 --heigth=200 --timeout=15") or die "Couldn't fork: $!\n";

print HTMLMSG <<HTML;
<html>
	<head>
		<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>
	</head>

	<body bgcolor='#ffffb8' link='#a03830' vlink='#a03830' alink='#ff3830'>
		<table width='100%' height='100%'>
			<tr>
				<td style='vertical-align:middle'>
					<p align='center'><font size='4' face='Sans'>
						TEST<br>
						<a href='close://'>Click here to close, press Esc or Enter </a>
					</font></p>
				</td>
			</tr>
		</table>
	</body>
</html>

HTML

close(HTMLMSG) or die "Couldn't close: $!\n";
