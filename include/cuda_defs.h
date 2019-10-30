#pragma once

#ifdef __CUDA_ARCH__
#define CUDA_HOST_AND_DEVICE __host__ __device__
#else // __CUDA_ARCH__
#define CUDA_HOST_AND_DEVICE
#endif
