#!/bin/sh
set -x
glib-gettextize --copy --force
libtoolize --automake --copy --force
intltoolize --automake --copy --force
aclocal
autoconf --force
autoheader --force
automake --add-missing --copy --force-missing --foreign
