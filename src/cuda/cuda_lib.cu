#include "../../include/cuda/cuda_lib.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

__global__ void matrix_mult (float* A, float* B, float* C, int a_row, int a_col, int b_row, int b_col) {
    // share memory 缓存A和B中对应的一对子矩阵，大小为BLOCK_SIZE * BLOCK_SIZE
    __shared__ float A_sub[BLOCK_SIZE * BLOCK_SIZE];
    __shared__ float B_sub[BLOCK_SIZE * BLOCK_SIZE];
    // 获取当前线程所在的block和thread的id
    int block_id_row = blockIdx.x;
    int block_id_col = blockIdx.y;
    int thread_id_row = threadIdx.x;
    int thread_id_col = threadIdx.y;
    // 计算当前线程对应A矩阵的行号和B矩阵的列号，也就是C矩阵的行号和列号
    int c_row_id = block_id_row * BLOCK_SIZE + thread_id_row;
    int c_col_id = block_id_col * BLOCK_SIZE + thread_id_col;
    
    int sbmtx_begin = 0;
    float c = 0.0;
    float compensation = 0.0;
    for (sbmtx_begin = 0; sbmtx_begin < a_col; sbmtx_begin += BLOCK_SIZE) {// 遍历每一对A，B矩阵c_row_id，c_col_id所在行列的子区间
        // 当前线程加载A，B矩阵中对应子矩阵的指定元素，保证当前block中的线程同时加载完一对A，B子矩阵
        A_sub[thread_id_row * BLOCK_SIZE + thread_id_col] = (c_row_id < a_row && sbmtx_begin + thread_id_col < a_col) ? A[c_row_id * a_col + sbmtx_begin + thread_id_col] : 0;
        B_sub[thread_id_row * BLOCK_SIZE + thread_id_col] = (c_col_id < b_col && sbmtx_begin + thread_id_row < b_row) ? B[(sbmtx_begin + thread_id_row) * b_col + c_col_id] : 0;
        // 等待同一个block中的线程加载完毕
        __syncthreads ();
        // 计算A矩阵c_row_id行和B矩阵c_col_id列一个区间的内积，并将每个区间结果累计
        #pragma unroll
        for (int i = 0; i < BLOCK_SIZE; ++i) {
            // c += A_sub[thread_id_row * BLOCK_SIZE + i] * B_sub[i * BLOCK_SIZE + thread_id_col];
            // Kahan's Summation Formula
            float y = A_sub[thread_id_row * BLOCK_SIZE + i] * B_sub[i * BLOCK_SIZE + thread_id_col] - compensation;
            float t = c + y;// 发生舍入
            compensation = (t - c) - y;// 记录下舍入误差
            c = t;
        }
        __syncthreads ();
    }
    if (c_row_id < a_row && c_col_id < b_col) {
        C[c_row_id * b_col + c_col_id] = c;
    }
}
void cuda_matrix_mult (float* A, float* B, float* C, int a_row, int a_col, int b_row, int b_col) {// A*B=C
    int size_a = a_row * a_col;
    int size_b = b_row * b_col;
    int size_c = a_row * b_col;
    // 在显存上分配空间
    float* dev_A, *dev_B, *dev_C;
    cudaMalloc ((void**) &dev_A, sizeof (float) * size_a);
    cudaMalloc ((void**) &dev_B, sizeof (float) * size_b);
    cudaMalloc ((void**) &dev_C, sizeof (float) * size_c);
    // copy数据到显存
    cudaMemcpy (dev_A, A, sizeof (float) * size_a, cudaMemcpyHostToDevice);
    cudaMemcpy (dev_B, B, sizeof (float) * size_b, cudaMemcpyHostToDevice);
    // 把结果C矩阵分割成grid_row * grid_col个BLOCK_SIZE * BLOCK_SIZE尺寸的block，可以认为C矩阵对应一个Grid
    int grid_row = a_row / BLOCK_SIZE + (a_row % BLOCK_SIZE == 0 ? 0 : 1);
    int grid_col = b_col / BLOCK_SIZE + (b_col % BLOCK_SIZE == 0 ? 0 : 1);
    dim3 grid (grid_row, grid_col);
    dim3 block (BLOCK_SIZE, BLOCK_SIZE);
    // 运行kernal函数
    matrix_mult <<<grid, block>>> (dev_A, dev_B, dev_C, a_row, a_col, b_row, b_col);
    // 把显存数据copy回内存
    cudaMemcpy (C, dev_C, sizeof (float) * size_c, cudaMemcpyDeviceToHost);
    // 释放显存
    cudaFree (dev_A);
    cudaFree (dev_B);
    cudaFree (dev_C);
}

__global__ void tensor_add (float* A, float* B, float* C, int size) {
    int thread_id = threadIdx.x;
    int block_id = blockIdx.x;
    int begin_idx = block_id * BLOCK_SIZE + thread_id;
    int read_offset = GRID_SIZE * BLOCK_SIZE;
    for (int i = begin_idx; i < size; i += read_offset) {// 这种方式尽可能保证显存数据的连续读取
        C[i] = A[i] + B[i];
    }
}
void cuda_tensor_add (float* A, float* B, float* C, int size) {
    float* dev_A, *dev_B, *dev_C;
    cudaMalloc ((void**) &dev_A, sizeof (float) * size);
    cudaMalloc ((void**) &dev_B, sizeof (float) * size);
    cudaMalloc ((void**) &dev_C, sizeof (float) * size);

    cudaMemcpy (dev_A, A, sizeof (float) * size, cudaMemcpyHostToDevice);
    cudaMemcpy (dev_B, B, sizeof (float) * size, cudaMemcpyHostToDevice);
    tensor_add <<<GRID_SIZE, BLOCK_SIZE>>> (dev_A, dev_B, dev_C, size);
    cudaMemcpy (C, dev_C, sizeof (float) * size, cudaMemcpyDeviceToHost);
    // 释放显存
    cudaFree (dev_A);
    cudaFree (dev_B);
    cudaFree (dev_C);
}

__global__ void scalar_tensor_mult (float* A, float* result, float s, int size) {
    int thread_id = threadIdx.x;
    int block_id = blockIdx.x;
    int begin_idx = block_id * BLOCK_SIZE + thread_id;
    int read_offset = GRID_SIZE * BLOCK_SIZE;
    for (int i = begin_idx; i < size; i += read_offset) {// 这种方式尽可能保证显存数据的连续读取
        result[i] = A[i] * s;
    }
}
void cuda_scalar_tensor_mult (float* A, float* result, float s, int size) {
    float* dev_A, *dev_result;
    cudaMalloc ((void**) &dev_A, sizeof (float) * size);
    cudaMalloc ((void**) &dev_result, sizeof (float) * size);

    cudaMemcpy (dev_A, A, sizeof (float) * size, cudaMemcpyHostToDevice);
    scalar_tensor_mult <<<GRID_SIZE, BLOCK_SIZE>>> (dev_A, dev_result, s, size);
    cudaMemcpy (result, dev_result, sizeof (float) * size, cudaMemcpyDeviceToHost);
    // 释放显存
    cudaFree (dev_A);
    cudaFree (dev_result);
}

__global__ void element_abs_sum (float* A, int size, float* results) {
    __shared__ float sub_results[BLOCK_SIZE];
    int thread_id = threadIdx.x;
    int block_id = blockIdx.x;

    int begin_idx = block_id * BLOCK_SIZE + thread_id;
    int read_offset = GRID_SIZE * BLOCK_SIZE;
    if (begin_idx >= size) {
        sub_results[thread_id] = 0;
    } else {
        float r = 0;
        for (int i = begin_idx; i < size; i += read_offset) {
            r += fabs(A[i]);
        }
        sub_results[thread_id] = r;
    }
    // 将同一个block中得到的结果汇总到global存储中的results中
    __syncthreads ();
    int merge_offset = 1;
    int mask = 2;
    while (merge_offset <= BLOCK_SIZE) {
        if (thread_id % mask == 0 && thread_id + merge_offset < BLOCK_SIZE) {
            sub_results[thread_id] += sub_results[thread_id + merge_offset];
        }
        merge_offset = merge_offset * 2;
        mask = mask * 2;
        __syncthreads ();
    }
    if (thread_id == 0) {
        results[block_id] = sub_results[0];
    }
}
float cuda_element_abs_sum (float* A, int size) {
    float* results = (float*) malloc (sizeof (float) * GRID_SIZE);
    float* dev_A;
    float* dev_results;
    cudaMalloc ((void**) &dev_A, sizeof (float) * size);
    cudaMalloc ((void**) &dev_results, sizeof (float) * GRID_SIZE);

    cudaMemcpy (dev_A, A, sizeof (float) * size, cudaMemcpyHostToDevice);

    // 运行kernal函数
    element_abs_sum <<<GRID_SIZE, BLOCK_SIZE>>> (dev_A, size, dev_results);

    cudaMemcpy (results, dev_results, sizeof (float) * GRID_SIZE, cudaMemcpyDeviceToHost);
    cudaFree (dev_results);
    cudaFree (dev_A);
    float abs_sum = 0;
    // 在cpu端将显卡传回的数据汇总
    for (int i = 0; i < GRID_SIZE; ++i) {
        abs_sum += results[i];
    }
    free (results);
    return abs_sum;
}

__global__ void element_square_sum (float* A, int size, float* results) {
    __shared__ float sub_results[BLOCK_SIZE];
    int thread_id = threadIdx.x;
    int block_id = blockIdx.x;
    
    int begin_idx = block_id * BLOCK_SIZE + thread_id;
    int read_offset = GRID_SIZE * BLOCK_SIZE;
    if (begin_idx >= size) {
        sub_results[thread_id] = 0;
    } else {
        float r = 0;
        for (int i = begin_idx; i < size; i += read_offset) {
            r += A[i] * A[i];
        }
        sub_results[thread_id] = r;
    }
    // 将同一个block中得到的结果汇总到global存储中的results中
    __syncthreads ();
    int merge_offset = 1;
    int mask = 2;
    while (merge_offset <= BLOCK_SIZE) {
        if (thread_id % mask == 0 && thread_id + merge_offset < BLOCK_SIZE) {
            sub_results[thread_id] += sub_results[thread_id + merge_offset];
        }
        merge_offset = merge_offset * 2;
        mask = mask * 2;
        __syncthreads ();
    }
    if (thread_id == 0) {
        results[block_id] = sub_results[0];
    }
}
float cuda_element_square_sum (float* A, int size) {
    float* results = (float*) malloc (sizeof (float) * GRID_SIZE);
    float* dev_A;
    float* dev_results;
    cudaMalloc ((void**) &dev_A, sizeof (float) * size);
    cudaMalloc ((void**) &dev_results, sizeof (float) * GRID_SIZE);

    cudaMemcpy (dev_A, A, sizeof (float) * size, cudaMemcpyHostToDevice);
    
    // 运行kernal函数
    element_square_sum <<<GRID_SIZE, BLOCK_SIZE>>> (dev_A, size, dev_results);

    cudaMemcpy (results, dev_results, sizeof (float) * GRID_SIZE, cudaMemcpyDeviceToHost);
    cudaFree (dev_results);
    cudaFree (dev_A);
    float square_sum = 0;
    // 在cpu端将显卡传回的数据汇总
    for (int i = 0; i < GRID_SIZE; ++i) {
        square_sum += results[i];
    }
    free (results);
    return square_sum;
}

__global__ void element_square (float* A, int size) {
    int thread_id = threadIdx.x;
    int block_id = blockIdx.x;
    int begin_idx = block_id * BLOCK_SIZE + thread_id;
    int read_offset = GRID_SIZE * BLOCK_SIZE;
    for (int i = begin_idx; i < size; i += read_offset) {
        A[i] = A[i] * A[i];
    }
}
void cuda_element_square (float* A, int size) {
    float* dev_A;
    cudaMalloc ((void**) &dev_A, sizeof (float) * size);

    cudaMemcpy (dev_A, A, sizeof (float) * size, cudaMemcpyHostToDevice);
    element_square <<<GRID_SIZE, BLOCK_SIZE>>> (dev_A, size);
    cudaMemcpy (A, dev_A, sizeof (float) * size, cudaMemcpyDeviceToHost);
    // 释放显存
    cudaFree (dev_A);
}

__global__ void element_mult (float* A, float* B, float* C, int size) {
    int thread_id = threadIdx.x;
    int block_id = blockIdx.x;
    int begin_idx = block_id * BLOCK_SIZE + thread_id;
    int read_offset = GRID_SIZE * BLOCK_SIZE;
    for (int i = begin_idx; i < size; i += read_offset) {
        C[i] = A[i] * B[i];
    }
}
void cuda_element_mult (float* A, float* B, float* C, int size) {
    float* dev_A, *dev_B, *dev_C;
    cudaMalloc ((void**) &dev_A, sizeof (float) * size);
    cudaMalloc ((void**) &dev_B, sizeof (float) * size);
    cudaMalloc ((void**) &dev_C, sizeof (float) * size);

    cudaMemcpy (dev_A, A, sizeof (float) * size, cudaMemcpyHostToDevice);
    cudaMemcpy (dev_B, B, sizeof (float) * size, cudaMemcpyHostToDevice);
    element_mult <<<GRID_SIZE, BLOCK_SIZE>>> (dev_A, dev_B, dev_C, size);
    cudaMemcpy (C, dev_C, sizeof (float) * size, cudaMemcpyDeviceToHost);
    // 释放显存
    cudaFree (dev_A);
    cudaFree (dev_B);
    cudaFree (dev_C);
}
