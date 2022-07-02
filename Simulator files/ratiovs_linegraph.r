library(ggplot2)
df <- read.csv( file = 'plots0.csv', header = FALSE)
colnames(df) <- c('v1', 'v2', 'ratio', 'steps')
qplot(ratio, data=df, geom="density") + xlab("ratio")
