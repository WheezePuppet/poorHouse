require(lattice)

pdf("level.pdf")
rus<-read.csv("datatot1.csv",header=FALSE)
rus<-as.matrix(rus)
dim(rus)<-c(100,100)
rownames(rus)<-1:100
colnames(rus)<-1:100

levelplot(rus)
dev.off()
