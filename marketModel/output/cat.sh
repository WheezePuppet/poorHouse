for i in `seq 0 23`
do
	cat /home/rruud/work/poorHouse/marketModel/output/datas$i.csv >> /home/rruud/work/poorHouse/marketModel/output/datatot1.csv
done
rm datas*
