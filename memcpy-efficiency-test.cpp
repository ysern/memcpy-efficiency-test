#include <iostream>
#include <chrono>

#define NUMBER_ELEMENTS 10000000

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
	printf( "memset: %.3lf bytes/sec\n", NUMBER_ELEMENTS * sizeof( double ) / nSec );

	
	
    //std::cout << "Hello World!\n";

	delete[] arrSrc;
	delete[] arrDst;
}
