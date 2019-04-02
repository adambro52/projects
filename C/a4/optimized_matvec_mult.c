#include "matvec.h"
#include <stdlib.h>

int optimized_matrix_trans_mult_vec(matrix_t mat, vector_t vec, vector_t res){
    if(mat.cols != vec.len){
        printf("mat.cols (%ld) != vec.len (%ld)\n",mat.cols,vec.len);
        return 1;
    }

    if(mat.rows != res.len){
        printf("mat.rows (%ld) != res.len (%ld)\n",mat.rows,res.len);
        return 2;
    }

    for (int k = 0; k < res.len; ++k) {
        VSET(res,k,0);
    }


    int j;

    for(int i=0; i<mat.rows; i++){

        for(j=0; j<mat.cols-4; j+=4){
            int prod1 = MGET(mat,i,j) * VGET(vec,i);
            int prod2 = MGET(mat,i,j+1) * VGET(vec,i);
            int prod3 = MGET(mat,i,j+2) * VGET(vec,i);
            int prod4 = MGET(mat,i,j+3) * VGET(vec,i);
            int curr1 = VGET(res,j);
            int curr2 = VGET(res,j+1);
            int curr3 = VGET(res,j+2);
            int curr4 = VGET(res,j+3);
            int next1 = prod1 + curr1;
            int next2 = prod2 + curr2;
            int next3 = prod3 + curr3;
            int next4 = prod4 + curr4;
            VSET(res,j, next1);
            VSET(res,j+1, next2);
            VSET(res,j+2, next3);
            VSET(res,j+3, next4);
        }

        for (int n = j; n < mat.cols; n++) {
            int prod = MGET(mat,i, n) * VGET(vec,i);
            int curr = VGET(res,n);
            VSET(res,n, curr+prod);

        }
    }

  return 0;
}
