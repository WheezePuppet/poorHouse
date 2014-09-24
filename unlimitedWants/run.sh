javac *.java
java Model 50 > results.txt
mkdir results/$1
cp results.txt results/$1
cp deviationPlot.r results/$1
cp splitNew.sh results/$1
cd results/$1
sh splitNew.sh
Rscript deviationPlot.r
mv Rplots.pdf ../$1.pdf
