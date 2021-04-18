library(ggplot2)
library(ggthemes)
library(tidyverse)
library(hrbrthemes)
library(reshape2)

setwd(".")
mydata <- read.csv("fitness_distances_params.csv", header = TRUE)
df <- data.frame(mydata)
# hist(mydata$distance, 100) # Example: 100 breaks, but you can specify them at will

# basic histogram
dist_hist <- ggplot2::ggplot(df, aes(x = distance)) +
   ggplot2::geom_histogram(
      binwidth = 5,
      fill = "#69b3a2",
      color = "#e9ecef",
      alpha = .9) +
  geom_hline(yintercept = .333, linetype = 3, alpha = .8) +
  scale_fill_grey(start = .4) +
  scale_x_continuous(breaks = (breaks=seq(0, 1500, by = 5)),limits=c(530, 600)) +
  scale_y_continuous(breaks = (seq(0, 100, by = 1))) +
  ylab("count") +
  ggtitle("Sumy tras kolejnych pokoleń") +
  theme_gray() +
  theme(legend.position = "none") +
  theme(plot.title = element_text(size = rel(1))) +
  theme(panel.background = element_rect(fill = "grey90"),
        panel.grid.minor = element_blank(),
        axis.line = element_line(arrow = arrow())) +
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
alpha_graph <- ggplot2::ggplot(df, aes(x = n)) +
  geom_point(aes(x = n, y = alpha0, fill = 'alpha0', colour = 'alpha0'), data = df, size = 1, shape = 18) +
  geom_point(aes(x = n, y = alpha1, fill = 'alpha1', colour = 'alpha1'), data = df, size = .5, shape = 17) +
  geom_point(aes(x = n, y = alpha2, fill = 'alpha2', colour = 'alpha2'), data = df, size = .2, shape = 16) +
scale_fill_grey(start = .04) +
  ylab("alpha") +
  scale_x_continuous(breaks = (breaks=seq(0, 15000, by = 1000)),limits=c(0, 14410)) +
  scale_y_continuous(breaks = (breaks=seq(0, 1.7, by = 0.1)),limits=c(0, 1.7)) +
  theme_gray() +
  theme(legend.position = c(.9, .8)) +
  theme(plot.title = element_text(size = rel(1))) +
  theme(panel.background = element_rect(
    fill = "grey90"),
    panel.grid.minor = element_blank(),
    # axis.line = element_line(arrow = arrow())
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
    subtitle = "alpha0, alpha1, alpha2 to wartości początkowa, środkowa i końcowa",
    caption = "Do regresji wykorzystano funkcję kwadratową",
    fill = "Alpha")
ggsave("alpha.png")

# beta points
alpha_graph <- ggplot2::ggplot(df, aes(x = n)) +
  geom_point(aes(x = n, y = beta1, fill = 'beta1', colour = 'beta1'), data = df, size = 1, shape = 18) +
  geom_point(aes(x = n, y = beta2, fill = 'beta2', colour = 'beta2'), data = df, size = .5, shape = 17) +
  geom_point(aes(x = n, y = beta0, fill = 'beta0', colour = 'beta0'), data = df, size = .5, shape = 16) +
scale_fill_grey(start = .04) +
  ylab("beta") +
  scale_x_continuous(breaks = (breaks=seq(0, 15000, by = 1000)),limits=c(0, 14410)) +
  scale_y_continuous(breaks = (breaks=seq(0, 100, by = 0.5)),limits=c(0, 9.999)) +
  theme_gray() +
  theme(legend.position = c(.9, .7)) +
  theme(plot.title = element_text(size = rel(1))) +
  theme(panel.background = element_rect(
    fill = "grey90"),
    panel.grid.minor = element_blank(),
    # axis.line = element_line(arrow = arrow())
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
    subtitle = "betha0, betha1, betha2 to wartości początkowa, środkowa i końcowa",
    caption = "Do regresji wykorzystano funkcję kwadratową",
    fill = "Beta")
ggsave("beta.png")


# points
alpha_graph <- ggplot2::ggplot(df, aes(x = n)) +
  geom_point(aes(x = n, y = evaporation_rate0, fill = 'evap0', color = 'evap0'), data = df, size = 1., shape = 18) +
  geom_point(aes(x = n, y = evaporation_rate2, fill = 'evap2', color = 'evap2'), data = df, size = .5, shape = 17) +
  geom_point(aes(x = n, y = evaporation_rate1, fill = 'evap1', color = 'evap1'), data = df, size = .5, shape = 16) +
scale_fill_grey(start = .04) +
  ylab("Evaporation rate") +
  scale_x_continuous(breaks = (breaks=seq(0, 15000, by = 1000)),limits=c(0, 14410)) +
  scale_y_continuous(breaks = (breaks=seq(0, 2, by = 0.05)),limits=c(0, 0.999)) +
  ggtitle("Evaporation rate quadratic change over generations") +
  theme_gray() +
  theme(legend.position = c(.9, .4)) +
  theme(plot.title = element_text(size = rel(1))) +
  theme(panel.background = element_rect(
    fill = "grey90"),
    panel.grid.minor = element_blank(),
    # axis.line = element_line(arrow = arrow())
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
    subtitle = "evap0, evap1, evap2 to wartości początkowa, środkowa i końcowa",
    caption = "Do regresji wykorzystano funkcję kwadratową",
    fill = "Evaporation")
ggsave("evaporation_rate.png")
