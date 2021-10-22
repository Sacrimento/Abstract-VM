#!/bin/bash
if [ -f .log ]; then
	rm .log
fi

for file in $1/*
do
	echo $file
	echo "-------------------------------" $file "-------------------------------" >>.log
	cat $file >> .log
	echo "-------------------------------" result : "-------------------------------" >>.log
	./avm $file 2>>.log >>.log
done
