library(ggplot2)

data <- read.csv('fitness.csv')
head(data)
ggplot(data, aes(x=gen)) +
    geom_line(aes(y = best, color="green")) +
    geom_line(aes(y = avg, color="blue")) +
    geom_line(aes(y = worst, color="red"))

ggsave("fitness.png")
