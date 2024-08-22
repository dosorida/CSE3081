#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int row, col;

int MaxSubrectangleSum1(int row, int col, int arr[][col]);
int MaxSubrectangleSum2(int row, int col, int arr[][col]);
int MaxSubrectangleSum3(int row, int col, int arr[][col]);

int main(int argc, char* argv[]) {
    // If the number of command-line arguments is not 2
    if(argc != 3) {
        printf("usage: ./mp1_20191048 input_filename algorithm_index\n");
        exit(1);
    }

    // running time
    clock_t start_time, end_time;

    start_time = clock();

    // file open
    FILE *input;
	input = fopen(argv[1], "r");

	// If the input file does not exist
	if(input == NULL) {
		printf("The input file does not exist.\n");
		exit(1);
	}

    // read file
    fscanf(input, "%d %d", &row, &col);

    int arr[row][col];
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            fscanf(input, "%d", &arr[i][j]);
        }
    }

    // algorithm index
    int alidx;
    alidx = atoi(argv[2]);

    // run algorithm
    int MaxSum;
    switch (alidx)
    {
    case 1:
        MaxSum = MaxSubrectangleSum1(row, col, arr);
        break;
    case 2:
        MaxSum = MaxSubrectangleSum2(row, col, arr);
        break;
    case 3:
        MaxSum = MaxSubrectangleSum3(row, col, arr);
        break;
    default:
        break;
    }

    end_time = clock();

    // write output file
    FILE* output;
    char fname[50] = "result_";
    strcat(fname, argv[1]);
    output = fopen(fname, "w");

    fprintf(output, "%s\n", argv[1]);	           // 1st line: input file name 
	fprintf(output, "%d\n", alidx);		           // 2nd line: algorithm index
	fprintf(output, "%d\n", row);                  // 3rd line: # of rows in the given 2D array.
    fprintf(output, "%d\n", col);                  // 4th line: # of columns in the given 2D array.
	fprintf(output, "%d\n", MaxSum);	           // 5th line: sum of the maximum sum subrectangle.
	fprintf(output, "%ld",end_time - start_time);  // 6th line: running time

    // file close
    fclose(input);
    fclose(output);

    return 0;
}

// Algorithm 1: Brute-force
int MaxSubrectangleSum1(int row, int col, int arr[][col]) {
    int MaxSum = 0;
    int LargestNum = arr[0][0];

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (LargestNum < arr[i][j]) LargestNum = arr[i][j];
            for (int k = 0; k < row; k++) {
                for (int l = 0; l < col; l++) {
                    int ThisSum = 0;
                    for (int a = i; a <= k; a++) {
                        for (int b = j; b <= l; b++) {
                            ThisSum += arr[a][b];
                        }
                    }
                    if (ThisSum > MaxSum) MaxSum = ThisSum;
                }
            }
        }
    }
    if (LargestNum < 0) MaxSum = LargestNum;
    return MaxSum;
}

// Algorithm 2: Dynamic Programming
int MaxSubrectangleSum2(int row, int col, int arr[][col]) {
    int MaxSum = 0;
    int LargestNum = arr[0][0];
    int dp[row][col];

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            dp[i][j] = 0;
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (LargestNum < arr[i][j]) LargestNum = arr[i][j];
            dp[i][j] = arr[i][j];
            if (i > 0) dp[i][j] += dp[i - 1][j];
            if (j > 0) dp[i][j] += dp[i][j - 1];
            if (i > 0 && j > 0) dp[i][j] -= dp[i - 1][j - 1];

            for (int k = 0; k <= i; k++) {
                for (int l = 0; l <= j; l++) {
                    int ThisSum = dp[i][j];
                    if (k > 0) ThisSum -= dp[k - 1][j];
                    if (l > 0) ThisSum -= dp[i][l - 1];
                    if (k > 0 && l > 0) ThisSum += dp[k - 1][l - 1];

                    if (ThisSum > MaxSum) {
                        MaxSum = ThisSum;
                    }
                }
            }
        }
    }
    if (LargestNum < 0) MaxSum = LargestNum;
    return MaxSum;
}

// Algorithm 3: Kadane's algorithm
int MaxSubrectangleSum3(int row, int col, int arr[][col]) {
    int MaxSum = 0;
    int LargestNum = arr[0][0];
    int tmp[row];
    
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            tmp[j] = 0;
            if (LargestNum < arr[i][j]) LargestNum = arr[i][j];
        }
        for (int j = i; j < col; j++) {
            for (int k = 0; k < row; k++) {
                tmp[k] += arr[k][j];
            }

            int ThisSum = 0;
            int TmpSum = 0; 

            for (int k = 0; k < row; k++) {
                if (TmpSum > 0) TmpSum += tmp[k];
                else TmpSum = tmp[k];
                if (TmpSum > ThisSum) ThisSum = TmpSum;
            }
            
            if (ThisSum > MaxSum) MaxSum = ThisSum;
        }
    }  
    if (LargestNum < 0) MaxSum = LargestNum;
    return MaxSum;
}