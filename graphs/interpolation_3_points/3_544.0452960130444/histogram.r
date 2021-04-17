library(ggplot2)
library(ggthemes)
library(tidyverse)
library(hrbrthemes)

setwd(".")
mydata <- read.csv("fitness_distances_params.csv", header = TRUE)
df <- data.frame(mydata)
# hist(mydata$distance, 100) # Example: 100 breaks, but you can specify them at will

# basic histogram
p <- ggplot2::ggplot(df, aes(x = distance)) +
   ggplot2::geom_histogram(
      binwidth = 5, 
      fill = "#69b3a2", 
      color = "#e9ecef",
      alpha = .9) + 
  geom_hline(yintercept = .333, linetype = 3, alpha = .8) +
  scale_fill_grey(start = .4) +
  scale_x_continuous(breaks = (seq(0,900,by = 20))) +
  scale_y_continuous(breaks = (seq(0,200,by = 5))) +
  ylab("count") + 
  ggtitle("Distances over generations") +
  theme_gray(
  base_size = 1,
  base_family = "",
  base_line_size = 11 / 22,
  base_rect_size = 11 / 22) +
  theme(legend.position = "none") +
  theme(plot.title = element_text(size = rel(1))) +
  theme(panel.background = element_rect(fill = "grey90"),
        panel.grid.minor = element_blank(),
        axis.line = element_line(arrow = arrow())) +
  theme(panel.spacing = unit(10.2, "lines")) +
  theme(text = element_text(size = 20)) +
  theme(axis.title.y = element_text(size = 15, vjust = 2)) +
  theme(axis.title.x = element_text(size = 15, vjust = -1)) +
  theme(plot.title = element_text(size = 20, vjust = 2)) +
  theme(axis.text.x = element_text(size = 10, vjust = -1)) +
  theme(axis.text.y = element_text(size = 10, hjust = -.1)) +
  theme(plot.margin = margin(.2, .2, .2, .2, "cm"),
  plot.background = element_rect(
    fill = "white",
    colour = "black",
    size = 1
  ))
ggsave("histogram.png")