javac *.java
java Model 0 50 0 > results.txt
mkdir $1
cp results.txt $1
cp deviationPlot.r $1
cp splitNew.sh $1
cd $1
sh splitNew.sh
Rscript deviationPlot.r
mv Rplots.pdf $1.pdf
