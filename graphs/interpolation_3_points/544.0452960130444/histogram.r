library(ggplot2)
library(ggthemes)

setwd("~/Documents/pw_mini/msi/MSI2-CVRP/graphs/interpolation_3_points/544.0452960130444")
# myvalues <- read.csv("fitness_distances_params.csv")

mydata <- read.csv("fitness_distances_params.csv", header = TRUE)

df <- data.frame(mydata)

print (df)

# hist(mydata$distance, 100) # Example: 100 breaks, but you can specify them at will

#ggplot(data=mydata, aes(x=factor, y=value, fill=race)) + 
#I named my dataset "mydata," which is why I said data=mydata
#I set x=factor, y=value, and the fill for the bars conditional on race.

#Don't forget to add + to the end of each line in  order to add layers.

#geom_bar(stat="identity", position="dodge")
#geom_bar adds the bars to the graph; stat="identity" identifies y as the vector reported in the graph; position="dodge" makes each race variable have its own bar side by side

ggplot2::ggplot(df, aes(distance)) + ggplot2::geom_bar()
