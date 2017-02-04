#!/bin/bash

echo "Running aclocal, autoheader, automake and autoconf ..."

aclocal -I m4 && autoheader && automake -a && autoconf
# autoreconf -f -i

echo "Done"

