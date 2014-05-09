filenames = as.list(paste(letters[1:10],".txt",sep=""))
lapply(filenames, function(filename) {
    omni=read.csv(filename)
    plot(omni$avg_price,type="l",ylim=c(0,max(omni$avg_price)),main=paste(substr(filename,1,1),omni$consumption_rate))
    lines(omni$sd,col="blue")
    lines(omni$amtProduced_need,col="green")
    legend(7,(.95*(max(omni$avg_price))), c("Avg Price","SD","Prod/Need"), lty=c(1,1,1), lwd=c(2.5,2.5,2.5),col=c("black","blue","green"))
})
