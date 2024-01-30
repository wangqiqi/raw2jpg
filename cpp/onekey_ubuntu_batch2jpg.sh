#!/bin/bash
set -e

if [ $# -eq 0 ]; then
	im_path=`pwd`
else
	im_path=$1
fi

echo "batch convert the raw image to jpg images in path: $im_path"
i=0
for im in $(ls $im_path)
do 
	if [ "${im##*.}" = "raw" ]; then
		if [ -f $im_path/$im ];then
			echo "convert $file to jpg image..."
            ./raw2jpg $im_path/$im
			let i+=1
		fi
	fi
done 
echo "DONE: $i pictures been gennerated!"s