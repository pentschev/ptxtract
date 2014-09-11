#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opencl_error.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

int main(int argc, char* argv[])
{
    const char cl_file[] = "kernel/add_vectors.cl";
    const char ptx_file[] = "../ptx/add_vectors_opencl.ptx";

    // Determine program path
    char* last = strrchr(argv[0],'/');
    if (last == NULL)
        last = strrchr(argv[0],'\\');
    if (last == NULL) {
        fprintf(stderr, "Could not determine program's path.\n");
        exit(1);
    }

    size_t path_sz = (size_t)(last - argv[0] + 2);
    char* path = (char*)malloc(path_sz);
    memcpy(path, argv[0], path_sz-1);
    path[path_sz-2] = '/';
    path[path_sz-1] = '\0';

    // Determine OpenCL kernel and PTX file locations
    char* cl_file_path = (char*)malloc(path_sz + sizeof(cl_file));
    char* ptx_file_path = (char*)malloc(path_sz + sizeof(ptx_file));
    strcpy(cl_file_path, path);
    strcat(cl_file_path, cl_file);
    strcpy(ptx_file_path, path);
    strcat(ptx_file_path, ptx_file);

    // Loads add_vectors.cl
    FILE* fp;
    fp = fopen(cl_file_path, "r");
    if (!fp) {
        fprintf(stderr, "Error loading kernel.\n");
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    size_t kernel_sz = ftell(fp);
    rewind(fp);

    char* kernel_str = (char*)malloc(kernel_sz);
    fread(kernel_str, 1, kernel_sz, fp);
    fclose(fp);

    // Query platforms and devices
    cl_platform_id platform;
    cl_device_id device;
    cl_uint num_devices, num_platforms;
    cl_int err;
    OPENCL_CHECK(clGetPlatformIDs(1, &platform, &num_platforms));
    OPENCL_CHECK(clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1,
                         &device, &num_devices));

    // Create OpenCL context
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    OPENCL_CHECK(err);

    // Create OpenCL command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device, 0, &err);
    OPENCL_CHECK(err);

    // Create OpenCL program for add_vectors.cl
    cl_program program = clCreateProgramWithSource(context, 1,
            (const char **)&kernel_str, (const size_t *)&kernel_sz, &err);
    OPENCL_CHECK(err);

    // Build OpenCL program
    OPENCL_CHECK(clBuildProgram(program, 1, &device, NULL, NULL, NULL));

    // Create OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "add_vectors", &err);
    OPENCL_CHECK(err);

    // Queries binary (PTX file) size
    size_t binsz;
    OPENCL_CHECK(clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, sizeof(size_t), &binsz, NULL));

    // Reads binary (PTX file) to memory buffer
    unsigned char *bin = (unsigned char *)malloc(binsz);
    OPENCL_CHECK(clGetProgramInfo(program, CL_PROGRAM_BINARIES, sizeof(unsigned char *), &bin, NULL));

    // Saves PTX to add_vectors_ocl.ptx
    fp = fopen(ptx_file_path, "wb");
    fwrite(bin, sizeof(char), binsz, fp);
    fclose(fp);
    free(bin);

    // Flush/Release OpenCL resources
    clFlush(command_queue);
    clFinish(command_queue);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);
    return 0;
}
