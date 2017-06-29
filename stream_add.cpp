//
//  main.cpp
//  Stream Benchmark
//
//  Created by Sagar Dolas on 28/06/17.
//  Copyright © 2017 Sagar Dolas. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <chrono>
#include <omp.h>


int main(int argc, const char * argv[]) {
    
	// Taking     
    int K_times = std::atoi(argv[1]) ; 
    uint64_t input_bytes = std::atol(argv[2]) ; 
    int N_bytes = 24 ; 

    double global_time = 0.0	 ; 

    uint64_t N = input_bytes / N_bytes ; 

    std::cout << input_bytes / 1000 <<" KiloBytes"<< std::endl ; 

    // Memory allocation     
    double * a = new double [N] ;
    double * b = new double [N] ;
    double * c = new double [N] ;

    // NUMA initialization 
    #pragma omp parallel for schedule(static) shared(a,b,c,N) num_threads(2)
    for (uint64_t i = 0; i < N; ++i)
    {
    	a[i] = 1.0 ; 
    	b[i] = 1.0 ; 
    	c[i] = 1.0 ; 
    }
    
    // ADD Benchmark
    for (int i = 0 ; i < K_times ; ++i){
    	#pragma omp parallel shared(a,b,c,N) num_threads(2)
    	{
    		double wtime = omp_get_wtime() ; 
    		#pragma omp for schedule(static) 
    		for (uint64_t k =0 ; k < N ; ++k){
    			a[k] = b[k] + c[k] ; 
    		}
    		wtime = omp_get_wtime() - wtime ; 
    		#pragma omp master 
    		 	global_time += wtime ; 
    	}
    }

	std::cout << "Time spend for ADD operations is :=" << global_time / (double) K_times  << std::endl ;     
 
}