#!/bin/bash
#crop
target="/Volumes/data_RAID/james.kong/of_v0.7.4_osx_release/apps/myApps/Zone4Main/bin/data/images/origin/layer2.png"
a=0
size=1024
y=0
# for y in $(seq 0 3)
# do
	for x in $(seq 0 15)
	do
		new=$(printf "%04d.png" ${a}) 
		dx=$((x*size))
		dy=0
		echo $dx $dy
		echo $new
		convert $target -crop 1024x768+$dx+$dy  $new
		let a=a+1
	done
# done
