__global__ void add_vectors(
    const int* a,
    const int* b,
    int *c,
    const int n)
{
    const int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx >= n) return;

    c[idx] = a[idx] + b[idx];
}
