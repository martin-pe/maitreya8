#!/bin/bash

echo "Running aclocal, autoheader, automake and autoconf ..."

aclocal && autoheader && automake -a && autoconf
# autoreconf -f -i

echo "Done"

