#!/bin/bash


for i in {1..5}
do
	cd ../../build
	name="data_$1$i.root"
	./TLStereo vis_file.mac $name > /dev/null #| grep Random:seed
	mv $name "../analysis/temp_$1"
	cd "../analysis/temp_$1"
done
	# root -l -b -q "groupMT.cc+($1$i)" > /dev/null
	# root -l -b -q "ana2MT.cc+($1$i)" > /dev/null

