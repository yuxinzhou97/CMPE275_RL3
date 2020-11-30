# CMPE275_RL3
Download repo: https://github.com/yuxinzhou97/CMPE275_RL3

Dependencies
```
brew install gcc 
brew install libomp
```

To convert any image to ppm p3 format
```
brew install imagemagick  
convert image.png -compress none image.ppm
```


### Run the project (Run OpenMP with g++)

_run files that have implemented openmp_
```
g++-10 -fopenmp standard_median_filter_omp.cpp -o standard_median_filter_omp  
./standard_median_filter_omp


g++-10 -fopenmp sort_optimization_median_filter_omp.cpp -o sort_optimization_median_filter_omp  
./sort_optimization_median_filter_omp


g++-10 -fopenmp fast_median_filter_omp.cpp -o fast_median_filter_omp  
./fast_median_filter_omp


g++-10 -fopenmp adaptive_median_filter_omp.cpp -o adaptive_median_filter_omp  
./adaptive_median_filter_omp

```


_run files that do not have openmp_
```
g++-10 standard_median_filter.cpp -o standard_median_filter  
./standard_median_filter


g++-10 sort_optimization_median_filter.cpp -o sort_optimization_median_filter  
./sort_optimization_median_filter


g++-10 fast_median_filter.cpp -o fast_median_filter  
./fast_median_filter


g++-10 adaptive_median_filter.cpp -o adaptive_median_filter  
./adaptive_median_filter

```


