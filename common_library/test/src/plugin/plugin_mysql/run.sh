#!/bin/sh

export LD_LIBRARY_PATH=.:../../../../src/util:../../../../src/net:../../../../src/sys:../../../../src/plugin/plugin_mysql:$LD_LIBRARY_PATH

FILES=`ls ut_*|grep -v ".cpp"`
for file in $FILES;
do
	sh -c ./$file
done