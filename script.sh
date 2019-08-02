#!/bin/bash
if [ -f .log ]; then
	echo "rm .log"
	rm .log
fi

for file in `find $@ -type f`
do
	echo "-------------------------------" $file "-------------------------------" >>.log
	cat $file >> .log
	echo "-------------------------------" result : "-------------------------------" >>.log
	trap `./avm $file 2>>.log >>.log` 6
	# ./vm $file
done
