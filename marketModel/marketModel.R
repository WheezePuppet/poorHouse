
# Top-level utilities to run the market simulation.
#
# Useful functions:
# For full parameter sweeps (looking at d.v.'s over many param values):
#   results <- param.sweep(switch.percentages,nums.trading.partners)
#   display.heatmap(results)
#   pngify.heatmap(results,filename)
# For single runs (looking at price performance over time for single choice of
#     param values):
#   results <- track.commodities(switch.percentage,num.trading.partners)
#   display.commodity.prices(single.run.results)

require(doParallel)
require(lattice)

registerDoParallel(8)

# Run a full-factorial parameter sweep of simulations, with the "percent
# likelihoods that an agent will switch to producing a different good" (each
# value in the range 0-100) and the number of trading partners per agent per
# time period (each value a non-negative integer) given as arguments, each as
# vectors of values.
# Returns a data frame with the total consumption for each param combo.
# (Don't forget to save the return value!)
param.sweep <- function(switch.percentages=seq(0,100,10), 
    nums.trading.partners=seq(0,100,10)) {

    results <- foreach (switch.percentage=switch.percentages,
        .combine=rbind) %dopar% {
        msg <- paste("Running switch percentage ",switch.percentage,
            "%...\n",sep="")
        cat(msg)
        res <- data.frame(
            switch=numeric(length(nums.trading.partners)),
            partners=numeric(length(nums.trading.partners)),
            total.cons=numeric(length(nums.trading.partners)))
        for (i in 1:length(nums.trading.partners)) {
            res[i,] <- 
                as.numeric(strsplit(
                system(paste("java Model",
                        switch.percentage,
                        nums.trading.partners[i],
                    "2> /dev/null"),
                    intern=TRUE),",")[[1]])
        }
        res
    }
    results
}

# Run a single simulation with the two parameters passed (scalars, not
# vectors) and return a data frame giving the
track.commodities <- function(switch.percentage, num.trading.partners) {
    numeric.col.nums <- c(1,3:7)
    rows <- strsplit(
        system(paste("java Model",
                switch.percentage,
                num.trading.partners,
            "true 2> /dev/null"),
            intern=TRUE),",")
    header <- gsub("\"","",strsplit(rows[[1]],","))
    rows <- rows[-1]    # Discard header row.
    weird.pseudo.matrix <- sapply(rows, function(row) {
        row <- as.list(row)
        row[numeric.col.nums] <- as.numeric(row[numeric.col.nums])
        row
        }
    )
    df <- as.data.frame(t(weird.pseudo.matrix))
    names(df) <- header
    df
}

# Show a plot of commodity prices over time.
display.commodity.prices <- function(single.run.results) {
    plot(single.run.results$year,single.run.results[[4]],type="n",
        main="Commodity prices",xlab="time",ylab="Price")
    commodities <- unique(single.run.results$commodity)
    for (commodity.num in 1:length(commodities)) {
        commodity <- commodities[[commodity.num]]
        comm.results <- 
            single.run.results[single.run.results$commodity == commodity,]
        lines(comm.results$year,comm.results[[4]],
            col=palette()[commodity.num %% length(palette())])
    }
    legend("bottomleft",title="Commodity",fill=palette(),legend=commodities)
}

# Create a file containing a heatmap of the results passed, with the filename
# passed.
pngify.heatmap <- function(results,filename="/tmp/heatmap.png") {
    png(filename)
    display.heatmap(results)
    dev.off()
}

# Display a heatmap of the results passed.
display.heatmap <- function(results) {
    row.vals <- unique(results[,1])
    col.vals <- unique(results[,2])
    mat <- matrix(nrow=length(row.vals),
                  ncol=length(col.vals))
    # This is the sexy way, and SHOULD work I swear:
    #   mat[results[,1]+1,results[,2]+1] <- results[,3]
    # Instead I'll do it the sucky old C++ way... :'(
    for (row in 1:nrow(results)) {
        results[row,1]
        mat[which(results[row,1]==row.vals),
            which(results[row,2]==col.vals)] <- results[row,3]
    }
    levelplot(mat,xlab="switch probability",ylab="num traders",
        main="Total consumption")
}
