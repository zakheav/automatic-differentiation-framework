#ifndef CUDALIB_H_
#define CUDALIB_H_
#define BLOCK_SIZE 32
#define GRID_SIZE 32
void cuda_matrix_mult (float* A, float* B, float* C, int a_row, int a_col, int b_row, int b_col);
void cuda_tensor_add (float* A, float* B, float* C, int size);
void cuda_scalar_tensor_mult (float* A, float* result, float s, int size);
void cuda_element_square (float* A, int size);
float cuda_element_square_sum (float* A, int size);
float cuda_element_abs_sum (float* A, int size);
void cuda_element_mult (float* A, float* B, float* C, int size);
#endif
