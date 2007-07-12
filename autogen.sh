#!/bin/sh
set -x
glib-gettextize --copy --force
libtoolize --automake --copy --force
intltoolize --automake --copy --force
aclocal-1.9
autoconf --force
autoheader --force
automake-1.9 --add-missing --copy --force-missing --foreign
