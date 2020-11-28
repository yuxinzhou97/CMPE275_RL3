#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include "omp.h"
using namespace std;
using namespace std::chrono;

#define NUM_THREADS 6

#define MAX_ROW_SIZE 10000
#define MAX_COL_SIZE 10000

void adaptiveMedianFilter(int pixel_matrix[MAX_ROW_SIZE][MAX_COL_SIZE], int processed[MAX_ROW_SIZE][MAX_COL_SIZE], int numrows, int numcols);
int adaptiveOptimization(int window[]);
void swap(int *a, int *b);
void quicksort(int *array, int left, int right);

int pixel_matrix[MAX_ROW_SIZE][MAX_COL_SIZE]{};
int processed[MAX_ROW_SIZE][MAX_COL_SIZE]{};

void swap(int *a, int *b)
{
    int temp;
    temp = *b;
    *b = *a;
    *a = temp;
}

void quicksort(int *array, int left, int right)
{
    if (left >= right)
    {
        return;
    }
    int pivot = (left + right) / 2;
    swap(&array[pivot], &array[right]);
    int storeldx = left;
    for (int i = left; i < right; i++)
    {
        if (array[i] < array[right])
        {
            swap(&array[storeldx++], &array[i]);
        }
    }
    swap(&array[storeldx], &array[right]);
    quicksort(array, left, storeldx - 1);
    quicksort(array, storeldx + 1, right);
}

void adaptiveMedianFilter(int pixel_matrix[MAX_ROW_SIZE][MAX_COL_SIZE], int processed[MAX_ROW_SIZE][MAX_COL_SIZE], int numrows, int numcols)
{
    // filtering
    // window example, 9 pixels, 3x9 matrix
    //          column1   column2  column3
    // row1     R G B     R G B    R G B
    // row2     R G B    |R G B|   R G B
    // row3     R G B     R G B    R G B
    // According to Appiah and co-authors, Greater Shape Arrangement gives the best result
    // Greater Shape Arrangement:
    // Group1: (row1,column1), (row2,column3), (row3,column1)
    // Group2: (row1,column3), (row2,column1), (row3,column1)
    // Group3: (row2,column2), (row2,column2), (row2,column2)

    // i.e.
    // red number of current pixel = median(median R in group1, median R in group2, median R in group3)

    int red_window[9], green_window[9], blue_window[9], row, col;
#pragma omp parallel private(row, col, red_window, green_window, blue_window) shared(processed, pixel_matrix, numrows, numcols)
    {
#pragma omp for
        for (int row = 1; row <= numrows; ++row)
        {
            // 1 pixel has 3 columns
            for (int col = 1; col <= numcols / 3; ++col)
            {
                // red
                red_window[0] = pixel_matrix[row - 1][col * 3 - 3];
                red_window[1] = pixel_matrix[row - 1][col * 3];
                red_window[2] = pixel_matrix[row - 1][col * 3 + 3];
                red_window[3] = pixel_matrix[row][col * 3 - 3];
                red_window[4] = pixel_matrix[row][col * 3];
                red_window[5] = pixel_matrix[row][col * 3 + 3];
                red_window[6] = pixel_matrix[row + 1][col * 3 - 3];
                red_window[7] = pixel_matrix[row + 1][col * 3];
                red_window[8] = pixel_matrix[row + 1][col * 3 + 3];
                int red = adaptiveOptimization(red_window);

                // green
                green_window[0] = pixel_matrix[row - 1][col * 3 - 3 + 1];
                green_window[1] = pixel_matrix[row - 1][col * 3 + 1];
                green_window[2] = pixel_matrix[row - 1][col * 3 + 3 + 1];
                green_window[3] = pixel_matrix[row][col * 3 - 3 + 1];
                green_window[4] = pixel_matrix[row][col * 3 + 1];
                green_window[5] = pixel_matrix[row][col * 3 + 3 + 1];
                green_window[6] = pixel_matrix[row + 1][col * 3 - 3 + 1];
                green_window[7] = pixel_matrix[row + 1][col * 3 + 1];
                green_window[8] = pixel_matrix[row + 1][col * 3 + 3 + 1];
                int green = adaptiveOptimization(green_window);

                // blue
                blue_window[0] = pixel_matrix[row - 1][col * 3 - 3 + 2];
                blue_window[1] = pixel_matrix[row - 1][col * 3 + 2];
                blue_window[2] = pixel_matrix[row - 1][col * 3 + 3 + 2];
                blue_window[3] = pixel_matrix[row][col * 3 - 3 + 2];
                blue_window[4] = pixel_matrix[row][col * 3 + 2];
                blue_window[5] = pixel_matrix[row][col * 3 + 3 + 2];
                blue_window[6] = pixel_matrix[row + 1][col * 3 - 3 + 2];
                blue_window[7] = pixel_matrix[row + 1][col * 3 + 2];
                blue_window[8] = pixel_matrix[row + 1][col * 3 + 3 + 2];
                int blue = adaptiveOptimization(blue_window);

                processed[row][col * 3] = red;
                processed[row][col * 3 + 1] = green;
                processed[row][col * 3 + 2] = blue;
            }
        }
    }
}

int adaptiveOptimization(int window[])
{
    int group1[3]{};
    int group2[3]{};
    int group3[3]{};
    int optimized[3]{};
    // 0 1 2
    // 3 4 5
    // 6 7 8

    group1[0] = window[0];
    group1[1] = window[5];
    group1[2] = window[6];
    quicksort(group1, 0, 2);

    group2[0] = window[2];
    group2[1] = window[3];
    group2[2] = window[8];
    quicksort(group2, 0, 2);

    group3[0] = window[1];
    group3[1] = window[4];
    group3[2] = window[7];
    quicksort(group3, 0, 2);

    optimized[0] = group1[1];
    optimized[1] = group2[1];
    optimized[2] = group3[1];
    quicksort(optimized, 0, 2);

    return optimized[2];
}

int main()
{

    steady_clock::time_point start_time = steady_clock::now();

    omp_set_num_threads(NUM_THREADS);

    int row = 0, col = 0, numrows = 0, numcols = 0, MAX = 0;
    string input_path = "image.ppm";
    string output_path = "image_output/image_after_adaptive_median_omp.ppm";
    ifstream infile(input_path);
    stringstream ss;
    string inputLine = "";

    // read first line, must check version
    getline(infile, inputLine);
    if (inputLine.compare("P3") != 0)
    {
        cerr << "Version error" << endl;
    }

    ss << infile.rdbuf();

    // read image width, height and max intensity
    ss >> numcols >> numrows >> MAX;

    // since using magic number version P3, which means RGB representation, 3 numbers represents 1 pixel
    numcols *= 3;

// initialize left bound and top bound
#pragma omp parallel for
    for (row = 0; row <= numrows; ++row)
    {
        pixel_matrix[row][0] = 0;
    }

#pragma omp parallel for
    for (col = 0; col <= numcols; ++col)
    {
        pixel_matrix[0][col] = 0;
    }

    // put input pixel information into matrix
    for (row = 1; row <= numrows; ++row)
    {
        for (col = 1; col <= numcols; ++col)
        {

            ss >> pixel_matrix[row][col];
        }
    }

    infile.close();

    adaptiveMedianFilter(pixel_matrix, processed, numrows, numcols);

    ofstream outfile;

    // write output image
    outfile.open(output_path);
    outfile << "P3" << endl;
    outfile << numcols / 3 << " " << numrows << endl;
    outfile << "255" << endl;

    for (row = 1; row <= numrows; ++row)
    {
        for (col = 1; col <= numcols; ++col)
        {
            outfile << processed[row][col] << " ";
        }
    }

    outfile.close();

    steady_clock::time_point end_time = steady_clock::now();
    long elapsed_time1 = duration_cast<microseconds>(end_time - start_time).count();
    cout << "Elapsed time: " << (double)elapsed_time1 / 1000 << " millisecond " << endl;

    return 0;
}