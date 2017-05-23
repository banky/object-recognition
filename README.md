# Object Recognition

A library that provides an interface for training generic deep neural networks written in C/C++. This library was developed both as a research project and as a way to allow training of complex networks on any generic system. Example code is provided for an object recognition application that aims at using neural networks to describe images based content. Training data is sourced from Google's Open Images library, and openCV is used for reading and writing image files.

# Implementation
Initially, a class was provided for performing matrix manipulation, however, this was swapped out to utilize the Eigen library. This switch provided speed improvements of about 30%. 

Each layer of the network is represented as a standard vector of matrices. These matrices are manipulated to fit the data set using back propagation. A gradient check function was also provided to ensure the back propagation functionality worked as expected.

A sigmoid cost function was utilized, however, this can be changed easily by swapping out a signle function. 

# In Future
Currently, GPU support is being implemented using the NVIDIA CUDA library. It is expected that this will greatly improve performance for matrix manipulation operations. 
