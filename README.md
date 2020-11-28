# CMPE275_RL3

```
brew install gcc 
brew install libomp
```

```
brew install imagemagick  
convert image.png -compress none image.ppm
```


# Run OpenMP with g++ 
```
g++_location -fopenmp file_name.cpp -o output_name  
./output_name  
```

_examples with openmp_
```
g++-10 -fopenmp standard_median_filter_omp.cpp -o standard_median_filter_omp && ./standard_median_filter_omp


g++-10 -fopenmp sort_optimization_median_filter_omp.cpp -o sort_optimization_median_filter_omp && ./sort_optimization_median_filter_omp


g++-10 -fopenmp fast_median_filter_omp.cpp -o fast_median_filter_omp && ./fast_median_filter_omp


g++-10 -fopenmp adaptive_median_filter_omp.cpp -o adaptive_median_filter_omp && ./adaptive_median_filter_omp

```



_without omp example_
```
/usr/local/bin/g++-10 

g++-10 standard_median_filter.cpp -o standard_median_filter && ./standard_median_filter


g++-10 sort_optimization_median_filter.cpp -o sort_optimization_median_filter && ./sort_optimization_median_filter


g++-10 fast_median_filter.cpp -o fast_median_filter && ./fast_median_filter


g++-10 adaptive_median_filter.cpp -o adaptive_median_filter && ./adaptive_median_filter

```


