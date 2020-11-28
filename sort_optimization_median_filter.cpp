#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;

#define MAX_ROW_SIZE 10000
#define MAX_COL_SIZE 10000

void sortOptimizationMedianFilter(int pixel_matrix[MAX_ROW_SIZE][MAX_COL_SIZE], int processed[MAX_ROW_SIZE][MAX_COL_SIZE], int numrows, int numcols);
int sortOptimization(int window[]);
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

void sortOptimizationMedianFilter(int pixel_matrix[MAX_ROW_SIZE][MAX_COL_SIZE], int processed[MAX_ROW_SIZE][MAX_COL_SIZE], int numrows, int numcols)
{
    // filtering
    // window example, 9 pixels, 3x9 matrix
    // column1   column2  column3
    //  R G B     R G B    R G B
    //  R G B    |R G B|   R G B
    //  R G B     R G B    R G B

    // i.e.
    // red number of current pixel = median(min R in column1, median R in column2, max R in column3)

    int red_window[9], green_window[9], blue_window[9];

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
            int red = sortOptimization(red_window);

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
            int green = sortOptimization(green_window);

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
            int blue = sortOptimization(blue_window);

            processed[row][col * 3] = red;
            processed[row][col * 3 + 1] = green;
            processed[row][col * 3 + 2] = blue;
        }
    }
}

int sortOptimization(int window[])
{
    int column1[3]{};
    int column2[3]{};
    int column3[3]{};
    int optimized[3]{};

    column1[0] = window[0];
    column1[1] = window[3];
    column1[2] = window[6];
    quicksort(column1, 0, 2);

    column2[0] = window[1];
    column2[1] = window[4];
    column2[2] = window[7];
    quicksort(column2, 0, 2);

    column3[0] = window[2];
    column3[1] = window[5];
    column3[2] = window[8];
    quicksort(column3, 0, 2);

    optimized[0] = column1[0];
    optimized[1] = column2[1];
    optimized[2] = column3[2];
    quicksort(optimized, 0, 2);

    return optimized[2];
}

int main()
{

    steady_clock::time_point start_time = steady_clock::now();

    int row = 0, col = 0, numrows = 0, numcols = 0, MAX = 0;
    string input_path = "image.ppm";
    string output_path = "image_output/image_after_sort_opt.ppm";
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
    for (row = 0; row <= numrows; ++row)
    {
        pixel_matrix[row][0] = 0;
    }

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

    sortOptimizationMedianFilter(pixel_matrix, processed, numrows, numcols);

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