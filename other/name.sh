#!/bin/bash

for i in $(ls .); do

    ext=${i##*.}

    if [ "$ext" == "h" ]; then

	f=${i%%.*};
	
	mv $f.h $f.hpp
    fi
    
done