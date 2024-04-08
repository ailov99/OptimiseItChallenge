# ParallelSorting library
This library contains implementations of various parallel sorting algorithms (using OpenMP).

## Interface
The sort function signature is as follows:

`void sort(long long *data, const in data_len)`

Where:
* `data` is a pre-allocated array of numbers. This will be sorted in-place
* `data_len` is the number of numbers in the array