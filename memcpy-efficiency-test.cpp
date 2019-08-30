#include <iostream>
#include <chrono>

#include "libs/FastMemcpy/FastMemcpy_Avx.h"

#define NUMBER_ELEMENTS 0x10000000

int main()
{
	double* arrSrc = new double[ NUMBER_ELEMENTS ];
	double* arrDst = new double[ NUMBER_ELEMENTS ];

	double * pElem = arrSrc;
	for ( int i = 0; i < NUMBER_ELEMENTS; ++i )
	{
		*pElem = static_cast< double >(i);
		pElem++;
	}
	memset( arrDst, 0, NUMBER_ELEMENTS * sizeof( double ) / sizeof( int ) );


	auto start = std::chrono::high_resolution_clock::now();

	memcpy( arrDst, arrSrc, NUMBER_ELEMENTS * sizeof( double ) );

	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	double nSec = 1e-6 * std::chrono::duration_cast< std::chrono::microseconds >(elapsed).count();
	printf( "memset : %.3lf bytes/sec\n", NUMBER_ELEMENTS * sizeof( double ) / nSec );

	memset( arrDst, 0, NUMBER_ELEMENTS * sizeof( double ) / sizeof( int ) );

	start = std::chrono::high_resolution_clock::now();

	memcpy_fast( arrDst, arrSrc, NUMBER_ELEMENTS * sizeof( double ) );

	elapsed = std::chrono::high_resolution_clock::now() - start;
	nSec = 1e-6 * std::chrono::duration_cast< std::chrono::microseconds >(elapsed).count();
	printf( "skywind3000/FastMemcpy::memcpy_fast : %.3lf bytes/sec\n", NUMBER_ELEMENTS * sizeof( double ) / nSec );

	delete[] arrSrc;
	delete[] arrDst;
}
