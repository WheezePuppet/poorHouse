#/bin/bash
l=0
i=1
j=1
for i in `seq 1 100`;
do
	for j in `seq 1 100`;
	do
		k=$(($i*$j))
		p=$(($l/417))
		echo 'java Model ' $i $k ' >> /home/rruud/work/poorHouse/marketModel/output/datas'$p'.csv' >> /home/rruud/work/poorHouse/marketModel/shripts/batch$p.sh
		#echo $i $j 
		l=$(($l+1))
	done
done
