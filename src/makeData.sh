#!/bin/bash
# Author: Noah Albright
# File: makeData.sh
# Purpose: This shell script is meant to get
# the test and training data for the ML pipeline.


$( gcc -g makeData.c -lm -o makeData )
for i in {1..6};
do
	$( ./makeData ${i} 750 >../data/phase${i}Train )
	$( ./makeData ${i} 250 >../data/phase${i}Test )
done
