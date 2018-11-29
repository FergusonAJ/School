setwd('~/Documents/School/FA18/EC/TermProject/work/main/viz')
library(ggplot2)

ic_data <- read.csv('../output/ic.csv')
numGens = max(ic_data$gen)
ic_df <- data.frame(matrix(nrow=numGens, ncol=4))
colnames(ic_df) <- c('min', 'avg', 'max', 'gen')
for(i in 1:numGens){
  ic_df[i,'min'] = min(ic_data[ic_data$gen == i-1,]$value)
  ic_df[i,'avg'] = ave(ic_data[ic_data$gen == i-1,]$value)[1]
  ic_df[i,'max'] = max(ic_data[ic_data$gen == i-1,]$value)
  ic_df[i,'gen'] = i
}

ggplot(data = ic_df, aes(x=gen, y=max)) + 
  geom_line(color='green') +
  geom_line(aes(x=gen, y=min), color='red') + 
  geom_line(aes(x=gen, y=avg), color='blue') 

ruleset_data <- read.csv('../output/ruleset.csv')
numGens = max(ruleset_data$gen)
ruleset_df <- data.frame(matrix(nrow=numGens, ncol=4))
colnames(ruleset_df) <- c('min', 'avg', 'max', 'gen')
for(i in 1:numGens){
  ruleset_df[i,'min'] = min(ruleset_data[ruleset_data$gen == i-1,]$value)
  ruleset_df[i,'avg'] = ave(ruleset_data[ruleset_data$gen == i-1,]$value)[1]
  ruleset_df[i,'max'] = max(ruleset_data[ruleset_data$gen == i-1,]$value)
  ruleset_df[i,'gen'] = i
}

ggplot(data = ruleset_df, aes(x=gen, y=max)) + 
  geom_line(color='green') +
  geom_line(aes(x=gen, y=min), color='red') + 
  geom_line(aes(x=gen, y=avg), color='blue') +
  geom_line(data = ic_df, aes(x=gen, y=max), color='green', linetype='dashed') +
  geom_line(data = ic_df, aes(x=gen, y=min), color='red', linetype='dashed') +
  geom_line(data = ic_df, aes(x=gen, y=avg), color='blue', linetype='dashed') +
  xlab('Generation') + 
  ylab('Fitness') +
  ggtitle('Fitness over time for "Classic" treatment')

ggsave('../output/classic.png')
