
library(lattice)

dataFilename = readline("Enter filename: ")
tradeBinSize = 500
numTradesThreshold = 1000

communities = read.csv(dataFilename,header=FALSE)

colnames(communities) = c("SATISFIED","COMMUNITY","SALARY","TIMES_TRADED","INTROVERSION")

tradesHist = hist(communities$TIMES_TRADED,plot=FALSE,
    breaks=seq(0,ceiling(max(communities$TIMES_TRADED) 
                            / tradeBinSize)*tradeBinSize,
    tradeBinSize))

minmax = range(which(tradesHist$counts > numTradesThreshold))
minTradeBinNum = minmax[1]
maxTradeBinNum = minmax[2]

satisfactionHist =
    hist(communities[communities$TIMES_TRADED>=(minmax[1]-1)*tradeBinSize &
    communities$TIMES_TRADED<minmax[1]*tradeBinSize,"SATISFIED"],
	plot=FALSE,breaks=seq(-.5,10.5,1))$counts

trading=matrix(satisfactionHist,ncol=1)

for(i in (minTradeBinNum+1):maxTradeBinNum) {
	trades=hist(communities[communities$TIMES_TRADED>=(tradeBinSize*(i-1)) &
        communities$TIMES_TRADED<tradeBinSize*i,
		 "SATISFIED"],plot=FALSE,breaks=seq(-.5,10.5,1))$counts
	trading=cbind(trading,trades)
}

tradeMean=apply(trading,2,mean)
names(tradeMean) = NULL

for(i in 1:(maxTradeBinNum - minTradeBinNum + 1))
{
	for(j in 1:nrow(trading))
	{
		if(tradeMean[i]<1) {
            stop("Stephen was WRONG!!!!!!\n");
        }
        temp=trading[j,i]/tradeMean[i]
        trading[j,i]=temp
	}
}

colnames(trading) = tradeBinSize * (minTradeBinNum:maxTradeBinNum)
new=wireframe(trading,xlab=c("Commodities Satisfied"),ylab=c("Times Traded"),
	zlab=c("Agents"),scale=list(arrows=FALSE,cex=.5))
