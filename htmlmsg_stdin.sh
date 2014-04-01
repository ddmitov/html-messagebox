#!/bin/sh

# The position or precedence of any command line argument is of no importance at all.

./stdin | ./htmlmsg --input=stdin --width=400 --heigth=200 --timeout=8
