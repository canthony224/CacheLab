/* BY RYAN HOUCK AND CALEB ANTHONY 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"
#include <stdlib.h>

void trans(int M, int N, int A[N][M], int B[M][N]);
int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	if (M == 32 && N == 32)	{
		int blockSize = 8;
		for(int outerRow = 0; outerRow < M; outerRow += blockSize){
			for(int outerCol = 0; outerCol < N; outerCol += blockSize){
                for (int innerRow = outerRow; (innerRow < outerRow + blockSize) && (innerRow < M); innerRow++){
					int temp1 = A[innerRow][outerCol];
					int temp2 = A[innerRow][outerCol+1];
					int temp3 = A[innerRow][outerCol+2];
					int temp4 = A[innerRow][outerCol+3];
					int temp5 = A[innerRow][outerCol+4];
					int temp6 = A[innerRow][outerCol+5];
					int temp7 = A[innerRow][outerCol+6];
					int temp8 = A[innerRow][outerCol+7];
					B[outerCol][innerRow] = temp1;
					B[outerCol+1][innerRow] = temp2;
					B[outerCol+2][innerRow] = temp3;
					B[outerCol+3][innerRow] = temp4;
					B[outerCol+4][innerRow] = temp5;
					B[outerCol+5][innerRow] = temp6;
					B[outerCol+6][innerRow] = temp7;
					B[outerCol+7][innerRow] = temp8;
                }
			}
		}
	}
		
	if (M == 64 && N == 64)	{
		int blockSize = 4;
		for(int outerRow = 0; outerRow < M; outerRow += blockSize){
			for(int outerCol = 0; outerCol < N; outerCol += blockSize){
                for (int innerRow = outerRow; (innerRow < outerRow + blockSize) && (innerRow < M); innerRow += 2){
					int temp1, temp2, temp3, temp4, temp12, temp22, temp32, temp42;
					temp1 = A[innerRow][outerCol];
					temp2 = A[innerRow][outerCol+1];
					temp3 = A[innerRow][outerCol+2];
					temp4 = A[innerRow][outerCol+3];
					temp12 = A[innerRow+1][outerCol];
					temp22 = A[innerRow+1][outerCol+1];
					temp32 = A[innerRow+1][outerCol+2];
					temp42 = A[innerRow+1][outerCol+3];
					B[outerCol][innerRow] = temp1;
					B[outerCol+1][innerRow] = temp2;
					B[outerCol+2][innerRow] = temp3;
					B[outerCol+3][innerRow] = temp4;
					B[outerCol][innerRow+1] = temp12;
					B[outerCol+1][innerRow+1] = temp22;
					B[outerCol+2][innerRow+1] = temp32;
					B[outerCol+3][innerRow+1] = temp42;
                }
			}
		}
	}

	if (M == 61 && N == 67)	{
		//int blockSize = 6 <-- NOT EVEN THE RIGHT MAGIC NUMBER!!!;
		for(int outerRow = 0; outerRow < N; outerRow += /*blockSize*/9){
			for(int outerCol = 0; outerCol < M; outerCol += /*blockSize*/9){
                for (int innerRow = outerRow; (innerRow < outerRow + /*blockSize*/9) && (innerRow < N); innerRow++){
					int temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8, temp9;
					temp1 = A[innerRow][outerCol];
					temp2 = A[innerRow][outerCol+1];
					temp3 = A[innerRow][outerCol+2];
					temp4 = A[innerRow][outerCol+3];
					temp5 = A[innerRow][outerCol+4];
					temp6 = A[innerRow][outerCol+5];
					temp7 = A[innerRow][outerCol+6];
					temp8 = A[innerRow][outerCol+7];
					temp9 = A[innerRow][outerCol+8];
					B[outerCol][innerRow] = temp1;
					B[outerCol+1][innerRow] = temp2;
					B[outerCol+2][innerRow] = temp3;
					B[outerCol+3][innerRow] = temp4;
					B[outerCol+4][innerRow] = temp5;
					B[outerCol+5][innerRow] = temp6;
					B[outerCol+6][innerRow] = temp7;
					B[outerCol+7][innerRow] = temp8;
					B[outerCol+8][innerRow] = temp9;
                }
			}
		}
	}
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

