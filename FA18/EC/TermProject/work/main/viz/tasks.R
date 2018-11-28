library("ggplot2")

data = read.csv("task_fitness.csv")
plot = ggplot(data, aes(x=generation, y=matches, color=target)) + geom_line()
ggsave("task_fitness.png", height=6, width=10, units="in")
