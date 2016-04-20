#!/bin/bash

rm -rf Makefile.in Makefile 
rm -rf aclocal.m4 autom4te.cache/ 
rm -rf autoscan.log 
rm -rf config.h.in config.h config.log config.status
rm -rf configure missing stamp-h1
rm -rf install-sh confdefs.h conftest.dir/ depcomp install-sh compile

rm -rf src/*.o

exit 0

