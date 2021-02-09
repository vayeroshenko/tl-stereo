#!/bin/bash

rm dataNew_*

rm log.txt
cd ../build
cmake ../ 
make 
cd ../analysis

rm dataNew*
rm data_*

	# cp groupMTopt.cc "temp_$i"
	# cp ana2MT.cc "temp_$i"

for i in {1..7}
do
	mkdir "temp_$i"
	cp runMT.sh "temp_$i"
	cd "temp_$i"
	pwd
	ls
	screen -S "simu_$i" -L -d -m ./runMT.sh $i
	cd ../
done



while [ $(screen -ls | grep simu | wc -l) -ne 0 ]
do
	sleep 1
done

cd temp_1
nFile=$(ls | grep data_ | wc -l)
cd -

touch log.txt

for i in {1..8}
do
	# cat "temp_$i/log.txt">>log.txt
	cp "temp_$i/"data_* ./
done


root -l -b -q "mergeMT.cc+(7,$nFile)" > /dev/null

rm -rf temp*
rm data_*

rm *.d *.so *.pcm

# python2 draw.py

