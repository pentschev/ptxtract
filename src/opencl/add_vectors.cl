__kernel
void add_vectors(
    __global const int* a,
    __global const int* b,
    __global       int* c,
    int n)
{
    int idx = get_global_id(0);

    if (idx >= n) return;

    c[idx] = a[idx] + b[idx];
}
