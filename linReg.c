// Samuel Havron <havron@virginia.edu>
// https://www.github.com/samuelhavron
// MIT License

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define MAX_DATA 100

typedef struct {
  double x[MAX_DATA]; // independent points
  double y[MAX_DATA]; // dependent points
  int n; // number of data points
} DataSet;

typedef struct {
  double m; // slope
  double b; // y-intercept
  double r; // correlation constant
} LinRegResult;

// forward declarations
DataSet load_data(FILE *inputFile);
LinRegResult linear_regression(DataSet theData);
double sqr(double x);
double dotProd(double a[MAX_DATA], double b[MAX_DATA], int n);
double sum(double a[MAX_DATA], int n);

DataSet load_data(FILE *inputFile) {
  int i;
  DataSet data;
  for (i = 0; i < MAX_DATA; ++i) {
    int dataPoints = fscanf(inputFile, "%lf %lf", &data.x[i], &data.y[i]);
    if (dataPoints != 2) {
      if (dataPoints < 0 && feof(inputFile)) {
	break;
      } else {
	fprintf(stderr, "ERROR: Input does not match file format. Check input file.\n");
	exit(1);
      }
    }
    data.n = i;
  }
  printf("Loading %d data points ...\n", data.n);
  return data;
}

LinRegResult linear_regression(DataSet theData) {

  LinRegResult result;
  int n = theData.n; // number of data points
  double sumx = sum(theData.x, n); // sum of x
  double sumxx = dotProd(theData.x, theData.x, n); // sum of each x sqaured
  double sumy = sum(theData.y, n); //sum of y
  double sumyy = dotProd(theData.y, theData.y, n); // sum of each y squared
  double sumxy = dotProd(theData.x, theData.y, n); // sum of each x * y

  // Compute least-squares best fit straight line
  result.m = (n * sumxy - sumx * sumy) / (n * sumxx - sqr(sumx)); // slope
  result.b = (sumy * sumxx - sumx * sumxy) / (n * sumxx - sqr(sumx)); // y-intercept
  result.r = (sumxy - sumx * sumy / n) / sqrt((sumxx - sqr(sumx) / n) * (sumyy - sqr(sumy) / n)); // correlation

  return result;
}

int main(int argc, char *argv[]) {
  printf("Linear Regression\n");
  printf("=================\n\n");

  // Check for a filename
  if (argc == 2) {
    FILE *input = fopen(argv[1], "r");
    
    if (input == NULL) {
      perror(argv[1]);
      exit(1);

    } else {
      DataSet theData = load_data(input);
      fclose(input);
     
      LinRegResult linReg = linear_regression(theData);
      printf("\nSlope   \tm = %15.6e\n", linReg.m); // print slope
      printf("y-intercept\tb = %15.6e\n", linReg.b); // print y-intercept
      printf("Correlation\tr = %15.6e\n", linReg.r); // print correlation
    }

  } else {
    printf("ERROR: Must enter filename after ./linReg\n");

  }
  return 0;
}

double sqr(double x) {
  return x * x;
}

double dotProd(double a[MAX_DATA], double b[MAX_DATA], int n) {
  double dotProd = 0;
  int i;
  for (i = 0; i < n; i++) {
    dotProd += a[i] * b[i];
  }
  return dotProd;
}

double sum(double a[MAX_DATA], int n) {
  double sum = 0;
  int i;
  for (i = 0; i < n; i++) {
    sum += a[i];
  }
  return sum;
}
