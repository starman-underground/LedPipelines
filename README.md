# LedPipelines

LedPipelines is a library meant to sit on top of FastLED and provide a way to create animation pipelines that can layer easily and fluidly. It will support animations, layers, multiple segments, and more features.

- [current worked on tickets](https://github.com/users/TheVizWiz/projects/3/views/1)
- [feature requests](https://github.com/TheVizWiz/LedPipelines/issues)


## Why?
WLED, and similar libraries, suck when you want to implement more than one effect at a time. They generally also aren't great at reactive effects, or effects that are more complex than your basic effects. LedPipelines solves these problems by using a pipeline-esque way to handle LED lighting - effects are added as stages in a pipeline, which layer on top of each other to create the final product.



Check out the examples for a clearer understanding of how this makes building complex effects easier and faster.