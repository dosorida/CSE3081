#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

void insertion_sort(int A[], int n);

void quick_sort(int A[], int p, int r);
int partition(int A[], int p, int r);

void merge_sort(int A[], int p, int r);
void merge(int A[], int p, int q, int r);

void my_sort(int A[], int n);
void radix_sort(int A[], int n);
void heap_sort(int A[], int n);
void max_heapify(int A[], int root, int n);
void introsort_util(int A[], int p, int r, int depthLimit);
void intro_sort(int A[], int n);

int min, max;

#define SWAP(a, b) {int tmp; tmp = a; a = b; b = tmp;}
#define RADIX 10

#define INSERTION_SORT_THRESHOLD 16

int main(int argc, char* argv[]) {
    // If the number of command-line arguments is not 2
    if(argc != 3) {
        printf("usage: ./mp2_20191048 input_filename algorithm_index\n");
        exit(1);
    }

    // file open
    FILE *input;
	input = fopen(argv[1], "r");

	// If the input file does not exist
	if(input == NULL) {
		printf("The input file does not exist.\n");
		exit(1);
	}

    // read file
    int n;
    fscanf(input, "%d", &n);

    int* A = (int*)malloc((n + 1) * sizeof(int));
    for (int i = 0; i < n; i++) {
        fscanf(input, "%d ", &A[i]);
    }

    // algorithm index
    int ai;
    ai = atoi(argv[2]);

    // running time
    clock_t start_time, end_time;

    start_time = clock();

    // run algorithm
    switch (ai)
    {
    case 1:
        insertion_sort(A, n);
        break;
    case 2:
        quick_sort(A, 0, n - 1);
        break;
    case 3:
        merge_sort(A, 0, n - 1);
        break;
    case 4:
        my_sort(A, n);
        break;
    default:
        break;
    }

    end_time = clock();

    // write output file
    FILE* output;
    char fname[50] = "result_";
    strcat(fname, argv[2]);
    strcat(fname, "_");
    strcat(fname, argv[1]);
    output = fopen(fname, "w");

    fprintf(output, "%s\n", argv[1]);	           // 1st line: input file name 
	fprintf(output, "%d\n", ai);		           // 2nd line: algorithm index
	fprintf(output, "%d\n", n);                    // 3rd line: input size (the number of elements in the original list)
	fprintf(output, "%lf\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);  // 4th line: running time in seconds
	for (int i = 0; i < n; i++) {                  // 5th line: the sorted list
		fprintf(output, "%d ", A[i]);
	}

    // file close
    fclose(input);
    fclose(output);

    free(A);

    return 0;
}

// algorithm 1: insertion sort
void insertion_sort(int A[], int n) {
    int key, i, j;
    for (i = 1; i < n; i++) {
        key = A[i];
        j = i - 1;
        while ((j >= 0) && (A[j] > key)){
            A[j + 1] = A[j];
            j--;
        }
        A[j + 1] = key;
    }
}

// algorithm 2: quick sort
void quick_sort(int A[], int p, int r) {
    if (p < r) {
        int q = partition(A, p, r);
        quick_sort(A, p, q - 1);
        quick_sort(A, q + 1, r);
    }
}

int partition(int A[], int p, int r) {
    int i, pivot;
    pivot = p;
    
    for (i = p; i < r; i++) {
        if (A[i] < A[r]) {
            SWAP(A[i], A[pivot]);
            pivot++;
        }
    }
    SWAP(A[r], A[pivot]);
    return(pivot);
}

// algorithm 3: merge sort
void merge_sort(int A[], int p, int r) {
    if (p >= r) return;
    int q = (p + r) / 2;
    merge_sort(A, p, q);
    merge_sort(A, q + 1, r);
    merge(A, p, q, r);
}

void merge(int A[], int p, int q, int r) {
    int i, j, k;
    int pnum = q - p + 1;
    int nnum = r - q;

    int *L = (int *)malloc(pnum * sizeof(int));
    int *R = (int *)malloc(nnum * sizeof(int));

    for (i = 0; i < pnum; i++)
        L[i] = A[p + i];
    for (j = 0; j < nnum; j++)
        R[j] = A[q + 1 + j];

    i = 0;
    j = 0;

    while (i < pnum && j < nnum) {
        if (L[i] <= R[j]) {
            A[k] = L[i];
            i++;
        } else {
            A[k] = R[j];
            j++;
        }
    }

    while (i < pnum) {
        A[k] = L[i];
        i++;
    }
    while (j < nnum) {
        A[k] = R[j];
        j++;
    }

    free(L);
    free(R);
}

// algorithm 4: my sort(radix sort + intro sort)
void my_sort(int A[], int n) {
    int *PA = (int *)malloc((n + 1) * sizeof(int));
    int *NA = (int *)malloc((n + 1) * sizeof(int));

    int pnum = 0, nnum = 0;

    for (int i = 0; i < n; i++) {
        if (A[i] < 0) {
            NA[++nnum] = -A[i];
        }   
        else {
            PA[++pnum] = A[i];
        }           
    }

    int max = 0, max1 = 0, max2 = 0;
	for (int i = 1; i <= pnum; i++) {
		if (PA[i] > max1) {
			max1 = PA[i];
		}
	}
	for (int i = 1; i <= nnum; i++) {
		if (NA[i] > max2) {
			max2 = NA[i];
		}
	}

	if (max1 < max2) max = max2;
	else max = max1;

    if (max < n) {
        radix_sort(PA, pnum);
        radix_sort(NA, nnum);

        for (int i = 1; i <= nnum; i++)
            NA[i] = -NA[i];

        int idx = 0;
        for (int i = nnum; i >= 1; i--) {
            A[idx] = NA[i];
            idx++;
        }   
        for (int i = 1; i <= pnum; i++) {
            A[idx] = PA[i];
            idx++;
        }     
    }
    else {
        intro_sort(A, n);
    }

    free(NA);
    free(PA);
}

void radix_sort(int A[], int n) {
    int max = A[0];
    for (int i = 1; i < n; i++) {
        if (A[i] > max)
            max = A[i];
    }

    for (int exp = 1; max / exp > 0; exp *= 10) {
        int *output = (int *)malloc((n + 1) * sizeof(int));
        int count[10] = {0};

        // Counting sort
        for (int i = 1; i <= n; i++)
            count[(A[i] / exp) % 10]++;

        for (int i = 1; i < 10; i++)
            count[i] += count[i - 1];

        for (int i = n; i > 0; i--) 
			output[(--count[(A[i] / exp) % 10]) + 1] = A[i];

        for (int i = 1; i <= n; i++)
            A[i] = output[i];

        free(output);
    }
}

void heap_sort(int A[], int n) {
    for (int i = n / 2; i > 0; i--)
        max_heapify(A, i, n);
        
    for (int i = n - 1; i > 0; i--) {
        SWAP(A[1], A[i + 1]);
        max_heapify(A, 1, i);
    }
}

void max_heapify(int A[], int root, int n) {
    int value = A[root];
    int child = root * 2;

    while (child <= n) {
        if (child < n && A[child] < A[child + 1])
            child++;

        if (value > A[child])
            break;
        else {
            A[child / 2] = A[child];
            child *= 2;
        }
    }
    A[child / 2] = value;
}

void introsort_util(int A[], int p, int r, int depthLimit) {
    while (r - p > INSERTION_SORT_THRESHOLD) {
        if (depthLimit == 0) {
            heap_sort(A + p, r - p + 1);
            return;
        }

        int pivotIndex = partition(A, p, r);

        if (pivotIndex - p < r - pivotIndex) {
            introsort_util(A, p, pivotIndex - 1, depthLimit - 1);
            p = pivotIndex + 1;
        } else {
            introsort_util(A, pivotIndex + 1, r, depthLimit - 1);
            r = pivotIndex - 1;
        }
    }

    insertion_sort(A, r - p + 1);
}

void intro_sort(int A[], int n) {
    int depthLimit = 2 * (int)log2(n);

    introsort_util(A, 0, n - 1, depthLimit);

    insertion_sort(A, n);
}