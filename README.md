ptxtract
========

Simple examples for PTX code extraction from CUDA and OpenCL kernels.

Requirements
------------

Besides of a common C development environment (GCC, Make, etc.), you'll need the [CUDA Toolkit](https://developer.nvidia.com/cuda-toolkit).

Building
--------

To build, simply do:

`make`

Be sure that you have the CUDA Toolkit installed under `/usr/local/cuda` or the environment variable `CUDA_PATH` set to the proper path.

Running
-------

The PTX file for the CUDA kernel will be available under the name `ptx/add_vectors_cuda.ptx`, there's no need to run anything (in fact, no executables will be generated).

For the OpenCL kernel, it's necessary to execute the host-side code, by simply typing:

`./bin/add_vectors_opencl`

The resulting PTX file is generated under the name `ptx/add_vectors_opencl.ptx`. Note that you require an NVIDIA GPU as the first OpenCL device, otherwise, the file will be there but it will NOT be a PTX file, instead, it will be a vendor-specific binary/pre-binary file.
