filenames = as.list(paste(letters[1:10],".txt",sep=""))
lapply(filenames, function(filename) {
    omni=read.csv(filename)
    plot(omni$avg_price,
		type="l",
		ylim=c(0, max(c(omni$avg_price,omni$avg_def))),
        xlab="time periods",
		main=paste(toupper(substr(filename,1,1)), "-- Consumption rate:",
            round(omni$consumption_rate[1],2))
    )
    lines(omni$sd,col="blue")
    lines(omni$avg_def,col="green")
    # lines(omni$amtProduced_need,col="green")
    legend(7,(.95*(max(omni$avg_price))), 
        c("Avg Price","SD"
            #,"Prod/Need"
        ),
        lty=c(1,1
            #,1
        ),
        lwd=c(2.5,2.5
            #,2.5
        ),
        col=c("black","blue"
            #,"green"
        )
    )
})
