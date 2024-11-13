#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 

// Structure to pass thread arguments
typedef struct {
    int thread_id;
    int start_row;
    int end_row;
} ThreadArgs;

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void* computeSum(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    int start_row = thread_args->start_row;
    int end_row = thread_args->end_row;
    
    // Process assigned rows
    for(int i = start_row; i < end_row; i++) {
        for(int j = 0; j < MAX; j++) {
            matSumResult[i][j] = matA[i][j] + matB[i][j];
        }
    }
    
    free(args);
    return NULL;
}

void* computeDiff(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    int start_row = thread_args->start_row;
    int end_row = thread_args->end_row;
    
    // Process assigned rows
    for(int i = start_row; i < end_row; i++) {
        for(int j = 0; j < MAX; j++) {
            matDiffResult[i][j] = matA[i][j] - matB[i][j];
        }
    }
    
    free(args);
    return NULL;
}

void* computeProduct(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    int start_row = thread_args->start_row;
    int end_row = thread_args->end_row;
    
    // Process assigned rows
    for(int i = start_row; i < end_row; i++) {
        for(int j = 0; j < MAX; j++) {
            matProductResult[i][j] = 0;
            for(int k = 0; k < MAX; k++) {
                matProductResult[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    
    free(args);
    return NULL;
}

int main() {
    srand(time(0));
    
    // 1. Fill the matrices with random values
    fillMatrix(matA);
    fillMatrix(matB);
    
    // 2. Print the initial matrices
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread objects
    pthread_t threads[10];
    int num_threads = 10;
    int rows_per_thread = MAX / num_threads;
    
    // 4. Create threads for matrix addition
    for(int i = 0; i < num_threads; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->thread_id = i;
        args->start_row = i * rows_per_thread;
        args->end_row = (i == num_threads - 1) ? MAX : (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, computeSum, (void*)args);
    }
    
    // 5. Wait for addition threads to finish
    for(int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Repeat for subtraction
    for(int i = 0; i < num_threads; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->thread_id = i;
        args->start_row = i * rows_per_thread;
        args->end_row = (i == num_threads - 1) ? MAX : (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, computeDiff, (void*)args);
    }
    
    for(int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Repeat for matrix multiplication
    for(int i = 0; i < num_threads; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->thread_id = i;
        args->start_row = i * rows_per_thread;
        args->end_row = (i == num_threads - 1) ? MAX : (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, computeProduct, (void*)args);
    }
    
    for(int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // 6. Print results
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    
    return 0;
}