#!/bin/bash

#While count of arguments != 0
while [ $# != 0 ]
do 
    wc -l $1
    shift
done