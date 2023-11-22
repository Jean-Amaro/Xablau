# Xablau
## Overview
This repository contains the personal projects that I developed during my MSc degree. I tried to keep them as simple and modern as possible, aiming to make the most of what the C++ language has to offer. Among the developed modules, the following are presented:

## Algebra
This module has:
* My version of the `std::complex` class.
* Two tensor classes: one with dynamic and other with fixed dimensionalities. Some matrix operations are not fast enough to be used, unfortunately. I hope to improve their performance someday...

## Color
Definitions of some color models and color spaces. Also, there are some implementations of color distances and conversions.

## ComputerGraphics
Well... An abstraction of a 3D object. There is not much to it.

## Geometry
Abstractions of geometric concepts, like vertices, lines, hyperplanes and so on. Every abstraction can be used in a N-Dimensional space.

## Graph
Abstraction of Graph Theory concepts, like a graphs, digraphs, trees and so on. Both `graph` and `digraph` classes hold nodes, using associative containers.

## IO
There are too many flaws there. I do not want to talk about it.

## MachineLearning
I need to give a better name for this module, because it is not exactly a Machine Learning method.

This is my MSc project core: Using a 3D mesh, this module can check for similar objects in its knowledge base, using L1 distance between eigenvalues generated from a normalized graph.

The dissertation can be found [here](https://www.teses.usp.br/teses/disponiveis/55/55134/tde-05092023-090640/en.php). It is written in PT-BR. But, if you are interested and do not read in Portuguese, send me an e-mail.

## Testing
A bundler that permutes template parameters, facilitating tests with classes that have too many possible combinations. `UnitTesting` uses this module extensively.

## UnitTesting
Unit tests for my code. It is written using GoogleTest. Of course, code coverage is not 100%, but it is better than nothing.

## BoundaryDescriptorClassifierApplication
This application is what I used during my MSc tests and experiments. I just realized that it does not make any sense leaving it in this repository. I will move it to other repository... Someday... Maybe...