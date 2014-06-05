for i in `seq 0 9999`
do
	cat /home/rruud/output/datas$i.csv >> /home/rruud/output/datatot8.csv
done
rm datas*
