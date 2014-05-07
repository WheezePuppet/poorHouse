fileO=readline("Enter omni file: ")
omni=read.csv(fileO)
plot(omni$avg_price,type="l",ylim=c(0,max(omni$avg_price)))
lines(omni$price_sd,col="blue")
