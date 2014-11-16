
# Top-level utilities to run the market simulation.
#
# Useful functions:
# For full parameter sweeps (looking at d.v.'s over many param values):
#   results <- param.sweep(switch.percentages,nums.trading.partners)
#   display.heatmap(results)
#   pngify.heatmap(results,filename)
# For single runs (looking at price performance over time for single choice of
#     param values):
#   results <- track.commodities(num.agents,switch.percentage,
#     num.trading.partners, mean.cons.rate, mean.salary)
#   display.commodity.prices(single.run.results)

require(doParallel)
require(lattice)

registerDoParallel(24)

SIM.FILES.BASE.DIR <- "/tmp"
SIM.STATS.FILE <- paste0(SIM.FILES.BASE.DIR,"/","sim_statsSIMTAG.csv")
COMM.STATS.FILE <- paste0(SIM.FILES.BASE.DIR,"/","comm_statsSIMTAG.csv")


# Run a single simulation with the parameters passed (scalars, not vectors)
#   and return a list containing:
# sp - the switch.percentage parameter (passed)
# nt - the num.trading.partners parameter (passed)
# na - total number of agents
# comm.df - a data frame with one row per commodity per year, giving various
#   parameters for that commodity in that time period
# cons.df - a data frame with one row per year, giving the total actual, and
#   theoretical, consumption rates for that year.
# total.cons - the total consumption rate of the entire model, in $/year
# total.sal - the total salary of the entire model in $/year

#track.commodities <- function(num.agents, switch.percentage, 
track.commodities <- function(switch.percentage,num.years,mean.amount.produced,money,simTag) { 
    #num.trading.partners, mean.cons.rate, mean.salary) {

    #numeric.col.nums <- c(1,3:7)
    numeric.col.nums <- c(1:4)
    num.agents = 100;

    stdout <- 
        system(paste("java edu.umw.poorhouse.Model",
                "-switchPerc",
                switch.percentage,
                "-numYears",
                num.years,
                "-avgProd",
                mean.amount.produced,
                "-money",
                money,
                "-simtag",
                simTag,
            "true 2> /dev/null"),
            intern=TRUE)

    total.cons <- as.numeric(strsplit(
        grep("^Total consumption:",stdout,value=TRUE),
        ": ")[[1]][2])
    total.sal <- as.numeric(strsplit(
        grep("^Total amount produced:",stdout,value=TRUE),
        ": ")[[1]][2])

    comm.df <- read.csv(sub("SIMTAG",simTag,COMM.STATS.FILE))

    cons.rows <- strsplit(grep("^TOTAL",stdout,value=TRUE),": ")
    cons.df <- data.frame(
        year=1:length(cons.rows),
        actual=rep(NA,length(cons.rows)),
        theoretical=rep(NA,length(cons.rows)),
        row.names=NULL)
    lapply(1:length(cons.rows), function(row.num) {
            vals <- as.numeric(strsplit(cons.rows[[row.num]][2]," of ")[[1]])
            cons.df[row.num,2:3] <<-
                list(actual=vals[1],theoretical=vals[2])
        }
    )

    list(sp=switch.percentage,
        na=num.agents,
        map=mean.amount.produced,
        m=money,
        comm.df=comm.df,
        cons.df=cons.df,
        total.cons=total.cons,
        total.sal=total.sal)
}


display.single.run <- function(single.run.results) {
    display.commodity.prices(single.run.results)
    readline()
    display.total.consumption(single.run.results)
}

# Show a plot of commodity prices over time.
display.commodity.prices <- function(single.run.results,file=NULL) {
    if(!is.null(file)) {
        png(paste(file,".png",sep=""))
    }
    comm.df <- single.run.results$comm.df
    plot(comm.df$year,comm.df[[2]],type="n",
        main=paste("Commodity prices\n",
            "number of agents = ",single.run.results$na,"\n",
            "switch rate = ",single.run.results$sp,"%\n",
            "initial money = ",single.run.results$m,"\n",
            "mean production = ",single.run.results$map,"\n",
                sep=""),
        xlab="time",ylab="Price",
        font.main=1,
        cex.main=.8)
    commodities <- unique(comm.df$commodity)
    for (commodity.num in 1:length(commodities)) {
        commodity <- commodities[[commodity.num]]
        comm.results <- 
            comm.df[comm.df$commodity == commodity,]
        lines(comm.results$year,comm.results[[2]],
            col=palette()[commodity.num %% length(palette())])
    }
    legend("bottomleft",title="Commodity",fill=palette(),legend=commodities)
    if(!is.null(file)) {
        dev.off()
    }
}

# Show a plot of a commodity, its price, and its standard deviation over time
display.commodity.individual <- function(single.run.results,file=NULL) {
    if(!is.null(file)) {
        png(paste(file,".png",sep=""))
    }
    comm.df <- single.run.results$comm.df
    plot(comm.df$year,comm.df[[2]],type="n",
        main=paste("Commodity prices\n",
            "number of agents = ",single.run.results$na,"\n",
            "switch rate = ",single.run.results$sp,"%\n",
            "initial money = ",single.run.results$m,"\n",
            "mean production = ",single.run.results$map,"\n",
                sep=""),
        xlab="time",ylab="Price",
        #ylim=c(0,12),
        font.main=1,
        cex.main=.8)
    commodities <- unique(comm.df$commodity)
    for (commodity.num in 1:length(commodities)) {
        commodity <- commodities[[commodity.num]]
        comm.results <- 
            comm.df[comm.df$commodity == commodity,]
        lines(comm.results$year,(comm.results[[3]]),
            col=palette()[commodity.num %% length(palette())])
        lines(comm.results$year,comm.results[[2]],
            col=palette()[commodity.num %% length(palette())])
    #    readline()
    }
    legend("bottomleft",title="Commodity",fill=palette(),legend=commodities)
    if(!is.null(file)) {
        dev.off()
    }
}

# Show a plot of total consumption over time.
display.total.consumption <- function(single.run.results,file=NULL) {
    if(!is.null(file)) {
        png(paste(file,".png",sep=""))
    }
    cons.df <- single.run.results$cons.df
    plot(cons.df$year,cons.df[[2]],type="n",
        main=paste("Total consumption\n",
            "number of agents = ",single.run.results$na,"\n",
            "switch rate = ",single.run.results$sp,"%\n",
                sep=""),
        xlab="time",ylab="Units",
        ylim=c(0,max(cons.df[2:3])+1),
        font.main=1,
        cex.main=.8)
    lines(cons.df$year,cons.df[[2]],col="blue")
    lines(cons.df$year,cons.df[[3]],col="green")
    best.fit.line <- lm(cons.df[[2]]~cons.df$year)
    abline(best.fit.line,col="red",lty="dashed")
    legend("bottomleft",fill=c("blue","green","red"),
        legend=c("total consumption", "theoretical possible consumption",
            "consumption trend"))
    if(!is.null(file)) {
        dev.off()
    }
}
