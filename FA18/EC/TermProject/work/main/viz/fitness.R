library("ggplot2")

data = read.csv("./output/fitness.csv")
plot = ggplot(data, aes(x=update)) +
        geom_line(aes(y=mean_fitness, color="Mean Fitness")) +
        geom_line(aes(y=min_fitness, color="Min Fitness")) +
        geom_line(aes(y=max_fitness, color="Max Fitness"))
ggsave("./output/fitness.png", height=6, width=10, units="in")
dev.off()
