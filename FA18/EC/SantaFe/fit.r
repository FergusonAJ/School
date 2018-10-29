library(ggplot2)

raw <- read.csv('fitness.csv')
head(raw)
print(raw$avg)
data <- data.frame(matrix(0,ncol = 3, nrow=nrow(raw) * 3), stringsAsFactors=F)
colnames(data) <- c('gen', 'type', 'value')
for(i in 1:nrow(raw)){
    data[i*3 - 2, 'gen']   <- raw[i, 'gen']
    data[i*3 - 2, 'type']  <- 'best'
    data[i*3 - 2, 'value'] <- raw[i, 'best']
    data[i*3 - 1, 'gen']   <- raw[i, 'gen']
    data[i*3 - 1, 'type']  <- 'avg'
    data[i*3 - 1, 'value'] <- raw[i, 'avg']
    data[i*3, 'gen']   <- raw[i, 'gen']
    data[i*3, 'type']  <- 'worst'
    data[i*3, 'value'] <- raw[i, 'worst']
}
print(nrow(data))
print(ncol(data))
head(data)
ggplot(data, aes(x=gen, y=value, color=type)) +
    geom_line() +
    xlab('Generation') + 
    ylab('Fitness') +
    ggtitle('Santa Fe Ant Fitness Over Time')

ggsave("fitness.png")
