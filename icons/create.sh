#!/bin/sh
if [ "$1" = "" ] ; then
	exit 0
fi
find -maxdepth 1 -type d -exec convert -resize \{\} $1 \{\}/harbour-minidsp_remote.png \;
