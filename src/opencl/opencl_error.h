#ifndef __OPENCL_ERROR_H__
#define __OPENCL_ERROR_H__

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

const char* get_opencl_error(cl_int err);

#define OPENCL_CHECK(err) {                                     \
    if (err != CL_SUCCESS) {                                    \
        printf("Error %d (%s) in %s:%d.\n",                     \
               err, get_opencl_error(err), __FILE__, __LINE__); \
        return -1;                                              \
    }                                                           \
}

#endif
