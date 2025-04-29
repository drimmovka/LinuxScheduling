#!/bin/bash

for i in {1..200}; do  
    NICENESS_TO_USE=$(($RANDOM % 40 - 20))  
    nice -n $NICENESS_TO_USE ./nice_job $i &  
done

wait