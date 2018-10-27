library(ggplot2)

raw <- read.csv('select.txt')
data <- data.frame(matrix(ncol=2, nrow=0))
for(i in 1:1000){
    data <- rbind(data, c(i, sum(raw[,1] == i)))
}
colnames(data) <- c('i', 'count')
ggplot(data, aes(x=i, y=count)) + geom_point()
ggsave('select.png')
