#!/bin/bash
for i in $(ls -R *.xv);
do
echo $i
./xvc $i
done
