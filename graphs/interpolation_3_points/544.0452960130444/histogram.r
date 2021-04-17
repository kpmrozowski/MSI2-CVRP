library(ggplot2)
library(ggthemes)
# library(tidyverse)
library(hrbrthemes)

setwd("~/Documents/pw_mini/msi/MSI2-CVRP/graphs/interpolation_3_points/544.0452960130444")
# myvalues <- read.csv("fitness_distances_params.csv")

mydata <- read.csv("fitness_distances_params.csv", header = TRUE)

df <- data.frame(mydata)

# print (df)

# hist(mydata$distance, 100) # Example: 100 breaks, but you can specify them at will

#ggplot(data=mydata, aes(x=factor, y=value, fill=race)) + 
#I named my dataset "mydata," which is why I said data=mydata
#I set x=factor, y=value, and the fill for the bars conditional on race.

#Don't forget to add + to the end of each line in  order to add layers.

#geom_bar(stat="identity", position="dodge")
#geom_bar adds the bars to the graph; stat="identity" identifies y as the vector reported in the graph; position="dodge" makes each race variable have its own bar side by side

# basic histogram
p <- ggplot2::ggplot(df, aes(x = distance)) +
   ggplot2::geom_histogram(
      binwidth = 3, 
      fill = "#69b3a2", 
      color = "#e9ecef", , 
      # geom = "crossbar", 
      # colour = NA, 
      # width = 0.75, 
      alpha = .9) + 
  geom_hline(yintercept = .333, linetype = 3, alpha = .8) +
  scale_fill_grey(start = .4) +
  scale_x_continuous(breaks = (seq(0,900,by = 50))) +
  scale_y_continuous(breaks = (seq(0,100,by = 10))) +
  ylab("Reposponse Times ms") + 
  ggtitle("Visual Condition") +
   ggplot2::ggtitle("Bin size = 3") +
  theme_dark(
  base_size = 1,
  base_family = "",
  base_line_size = 11 / 22,
  base_rect_size = 11 / 22) +
  theme(legend.position = "none") +
  theme(plot.title = element_text(size = rel(1))) +
  theme(panel.background = element_rect(fill = "grey90"),
        panel.grid.minor = element_blank()) +
  theme(panel.spacing = unit(10.2, "lines")) +
  theme(text = element_text(size = 20)) +
  theme(axis.title.y = element_text(size = 20, vjust = 0)) +
  theme(axis.title.x = element_text(size = 20, vjust = 0)) +
  theme(plot.title = element_text(size = 20, vjust = 0)) +
  theme(axis.text.y = element_text(size = 20, hjust = 0))


# p <- data %>%
#   filter( price<300 ) %>%
#   ggplot( aes(x=price)) +
#     geom_histogram( binwidth=3, fill="#69b3a2", color="#e9ecef", alpha=0.9) +
#     ggtitle("Bin size = 3") +
#     theme_ipsum() +
#     theme(
#       plot.title = element_text(size=15)
#     )
p

#p
# ggplot2::ggplot(df, aes(distance)) + ggplot2::geom_bar(position="dodge")
