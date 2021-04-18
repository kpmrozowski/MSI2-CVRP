library(ggplot2)
library(ggthemes)
library(tidyverse)
library(hrbrthemes)
library(reshape2)

setwd(".")
mydata_hist <- read.csv("distances.csv", header = TRUE)
mydata <- read.csv("params.csv", header = TRUE)
df_hist <- data.frame(mydata_hist)
df <- data.frame(mydata)
# hist(mydata$distance, 100) # Example: 100 breaks, but you can specify them at will

# basic histogram
dist_hist <- ggplot2::ggplot(df_hist, aes(x = distance)) +
   ggplot2::geom_histogram(
      binwidth = 5,
      fill = "#69b3a2",
      color = "#e9ecef",
      alpha = .9) +
  geom_hline(yintercept = .333, linetype = 3, alpha = .8) +
  scale_fill_grey(start = .4) +
  scale_x_continuous(breaks = (breaks=seq(0, 10000, by = 10)),limits=c(NA, 800)) +
  scale_y_continuous(breaks = (seq(0, 1000, by = 10))) +
  ylab("count") +
  ggtitle("Sumy tras kolejnych pokoleń") +
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
  theme(axis.text.x = element_text(
    face = "bold",
    color = "#993333",
    size = 10,
    angle = 45,
    vjust = .4)) +
  theme(axis.text.y = element_text(size = 10, hjust = -.1)) +
  theme(plot.margin = margin(.2, .2, .2, .2, "cm"),
  plot.background = element_rect(
    fill = "white",
    colour = "black",
    size = 1
  ))
ggsave("histogram.png")

# alpha points
alpha_graph <- ggplot2::ggplot(df, aes(x = iteration)) +
  geom_point(aes(x = iteration, y = ialpha, fill = 'ialpha', colour = 'ialpha'), data = df, size = 3, shape = 18) +
  geom_point(aes(x = iteration, y = falpha, fill = 'falpha', colour = 'falpha'), data = df, size = 1.5, shape = 17) +
scale_fill_grey(start = .04) +
  ylab("alpha") +
  scale_x_continuous(breaks = (breaks=seq(0, 10000, by = 10)),limits=c(0, 100)) +
  scale_y_continuous(breaks = (breaks=seq(0, 5, by = 0.1)),limits=c(0, 2)) +
  theme_gray() +
  theme(legend.position = c(.95, .75)) +
  theme(plot.title = element_text(size = rel(1))) +
  theme(panel.background = element_rect(
    fill = "grey90"),
    panel.grid.minor = element_blank(),
    ) +
  theme(text = element_text(size = 14)) +
  theme(axis.title.y = element_text(size = 15, vjust = 2)) +
  theme(axis.title.x = element_text(size = 15, vjust = -1)) +
  theme(plot.title = element_text(size = 20, vjust = 2)) +
  theme(axis.text.x = element_text(
    face = "bold",
    color = "#993333",
    size = 10,
    angle = 45,
    vjust = .4)) +
  theme(axis.text.y = element_text(
    face = "bold",
    color = "#993333",
    size = 10, 
    hjust = -10.3)) +
  theme(plot.margin = margin(.2, .2, .2, .2, "cm"),
  plot.background = element_rect(
    fill = "white",
    colour = "993333",
    size = 0
  )) +
  labs(
    title = "Eksploatacja kolejnych pokoleń",
    subtitle = "ialpha, falpha to wartości początkowa i końcowa",
    caption = "Do regresji wykorzystano funkcję liniową",
    fill = "Alpha")
ggsave("alpha.png")

# beta points
betha_graph <- ggplot2::ggplot(df, aes(x = iteration)) +
  geom_point(aes(x = iteration, y = ibeta, fill = 'ibeta', colour = 'ibeta'), data = df, size = 3, shape = 18) +
  geom_point(aes(x = iteration, y = fbeta, fill = 'fbeta', colour = 'fbeta'), data = df, size = 1.5, shape = 17) +
scale_fill_grey(start = .04) +
  ylab("beta") +
  scale_x_continuous(breaks = (breaks=seq(0, 10000, by = 10)),limits=c(0, 100)) +
  scale_y_continuous(breaks = (breaks=seq(0, 100, by = 0.5)),limits=c(0, 10)) +
  theme_gray() +
  theme(legend.position = c(.9, .3)) +
  theme(plot.title = element_text(size = rel(1))) +
  theme(panel.background = element_rect(
    fill = "grey90"),
    panel.grid.minor = element_blank(),
    ) +
  theme(text = element_text(size = 14)) +
  theme(axis.title.y = element_text(size = 15, vjust = 2)) +
  theme(axis.title.x = element_text(size = 15, vjust = -1)) +
  theme(plot.title = element_text(size = 20, vjust = 2)) +
  theme(axis.text.x = element_text(
    face = "bold",
    color = "#993333",
    size = 10,
    angle = 45,
    vjust = .4)) +
  theme(axis.text.y = element_text(
    face = "bold",
    color = "#993333",
    size = 10, 
    hjust = 1.1)) +
  theme(plot.margin = margin(.2, .2, .2, .2, "cm"),
  plot.background = element_rect(
    fill = "white",
    colour = "993333",
    size = 0
  )) +
  labs(
    title = "Eksploracja kolejnych pokoleń",
    subtitle = "ibetha, fbetha to wartości początkowa i końcowa",
    caption = "Do regresji wykorzystano funkcję liniową",
    fill = "Beta")
ggsave("beta.png")


# points evaporation
evap_graph <- ggplot2::ggplot(df, aes(x = iteration)) +
  geom_point(aes(x = iteration, y = ievapor, fill = 'ievapor', color = 'ievapor'), data = df, size = 3, shape = 18) +
  geom_point(aes(x = iteration, y = fevapor, fill = 'fevapor', color = 'fevapor'), data = df, size = 1.5, shape = 17) +
scale_fill_grey(start = .04) +
  ylab("Evaporation rate") +
  scale_x_continuous(breaks = (breaks=seq(0, 10000, by = 10)),limits=c(0, 100)) +
  scale_y_continuous(breaks = (breaks=seq(0, 2, by = 0.05)),limits=c(0, 0.999)) +
  theme_gray() +
  theme(legend.position = c(.9, .8)) +
  theme(plot.title = element_text(size = rel(1))) +
  theme(panel.background = element_rect(
    fill = "grey90"),
    panel.grid.minor = element_blank(),
    ) +
  theme(text = element_text(size = 14)) +
  theme(axis.title.y = element_text(size = 15, vjust = 2)) +
  theme(axis.title.x = element_text(size = 15, vjust = -1)) +
  theme(plot.title = element_text(size = 20, vjust = 2, hjust = 0)) +
  theme(axis.text.x = element_text(
    face = "bold",
    color = "#993333",
    size = 10,
    angle = 45,
    vjust = .4)) +
  theme(axis.text.y = element_text(
    face = "bold",
    color = "#993333",
    size = 10, 
    hjust = -10.3)) +
  theme(plot.margin = margin(.2, .2, .2, .2, "cm"),
  plot.background = element_rect(
    fill = "white",
    colour = "993333",
    size = 0
  )) +
  labs(
    title = "Współczynnik parowania kolejnych pokoleń",
    subtitle = "ievapor, fevapor to wartości początkowa i końcowa",
    caption = "Do regresji wykorzystano funkcję liniową",
    fill = "Evap")
ggsave("evaporation_rate.png")


# distances
dist_graph <- ggplot2::ggplot(df_hist, aes(x = n)) +
  geom_point(aes(x = n, y = distance, fill = 'distance', color = 'distance'), data = df_hist, size = 2, shape = 18) +
scale_fill_grey(start = .04) +
  scale_x_continuous(breaks = (breaks=seq(0, 10000, by = 100)),limits=c(0, 2424)) +
  scale_y_continuous(breaks = (breaks=seq(0, 2000, by = 10)),limits=c(569, 675)) +
  theme_gray() +
  theme(legend.position = c(.9, .9)) +
  theme(plot.title = element_text(size = rel(1))) +
  theme(panel.background = element_rect(
    fill = "grey90"),
    panel.grid.minor = element_blank(),
    ) +
  theme(text = element_text(size = 14)) +
  theme(axis.title.y = element_text(size = 15, vjust = 2)) +
  theme(axis.title.x = element_text(size = 15, vjust = -1)) +
  theme(plot.title = element_text(size = 20, vjust = 2, hjust = 0)) +
  theme(axis.text.x = element_text(
    face = "bold",
    color = "#993333",
    size = 10,
    angle = 45,
    vjust = .4)) +
  theme(axis.text.y = element_text(
    face = "bold",
    color = "#993333",
    size = 10, 
    hjust = 0)) +
  theme(plot.margin = margin(.2, .2, .2, .2, "cm"),
  plot.background = element_rect(
    fill = "white",
    colour = "993333",
    size = 0
  )) +
  ylab("Distance") +
  labs(
    title = "Sumy długości tras kolejnych pokoleń",
    fill = "distance")
ggsave("distances.png")
