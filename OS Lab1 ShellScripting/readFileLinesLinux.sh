#!/bin/bash

#Read file name from user.
read -p 'Enter a filename: ' filename
echo "Reading from file $filename"
cat filename | while read line; do
  echo $line
done