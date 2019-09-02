#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <locale>
#include <string>
#include "libs/FastMemcpy/FastMemcpy_Avx.h"
#include "SO_Serge_Rogatch.h"

#define NUMBER_ELEMENTS 0x10000000
#define BUFFER_ALIGNMENT_SIZE 32

class Foo
{
public:
	template<class T>
	static std::string FormatWithCommas( T value )
	{
		std::stringstream ss;
		ss.imbue( std::locale( "" ) );
		ss << std::fixed << value;
		return ss.str();
	}
};

int main()
{
	const int loopCount = 10;
	double * arrUnalignedSrc = new double[ NUMBER_ELEMENTS + BUFFER_ALIGNMENT_SIZE ]; // Create double size buffers
	double * arrUnalignedDst = new double[ NUMBER_ELEMENTS + BUFFER_ALIGNMENT_SIZE ]; // to make sure desired 

	double * arrSrc = arrUnalignedSrc;
	if ( (intptr_t( arrSrc ) & BUFFER_ALIGNMENT_SIZE - 1) != 0 )
	{
		arrSrc = reinterpret_cast< double * >((intptr_t( arrSrc ) & ~(BUFFER_ALIGNMENT_SIZE - 1)) + BUFFER_ALIGNMENT_SIZE);
	}
	double * arrDst = arrUnalignedDst;
	if ( (intptr_t( arrDst ) & BUFFER_ALIGNMENT_SIZE - 1) != 0 )
	{
		arrDst = reinterpret_cast< double * >((intptr_t( arrDst ) & ~(BUFFER_ALIGNMENT_SIZE - 1)) + BUFFER_ALIGNMENT_SIZE);
	}

	double * pElem = arrSrc;
	for ( int i = 0; i < NUMBER_ELEMENTS; ++i )
	{
		*pElem = static_cast< double >(i);
		pElem++;
	}
	memset( arrDst, 0, NUMBER_ELEMENTS * sizeof( double ) / sizeof( int ) );

	// Test 1: memcpy
	//

	// Copy once outside of loop 
	memcpy( arrDst, arrSrc, NUMBER_ELEMENTS * sizeof( double ) );

	// Do the bulk of the test
	auto start = std::chrono::high_resolution_clock::now();
	for ( int i = 0; i < loopCount; i++ )
		memcpy( arrDst, arrSrc, NUMBER_ELEMENTS * sizeof( double ) );
	auto elapsed = std::chrono::high_resolution_clock::now() - start;

	// Print result 
	double nSec = 1e-6 * std::chrono::duration_cast< std::chrono::microseconds >(elapsed).count();
	printf( "memset : %s bytes/sec\n", Foo::FormatWithCommas( NUMBER_ELEMENTS * sizeof( double ) / nSec * loopCount ).c_str() );


	// Test 2: memcpy_fast
	//

	memset( arrDst, 0, NUMBER_ELEMENTS * sizeof( double ) / sizeof( int ) );

	// Copy once outside of loop
	memcpy_fast( arrDst, arrSrc, NUMBER_ELEMENTS * sizeof( double ) );

	// Do the bulk of the test
	start = std::chrono::high_resolution_clock::now();
	for ( int i = 0; i < loopCount; i++ )
		memcpy_fast( arrDst, arrSrc, NUMBER_ELEMENTS * sizeof( double ) );
	elapsed = std::chrono::high_resolution_clock::now() - start;

	// Print result
	nSec = 1e-6 * std::chrono::duration_cast< std::chrono::microseconds >(elapsed).count();
	printf( "skywind3000/FastMemcpy::memcpy_fast : %s bytes/sec\n", Foo::FormatWithCommas( NUMBER_ELEMENTS * sizeof( double ) / nSec * loopCount ).c_str() );

	// Test 3: fastMemcpy
	//

	memset( arrDst, 0, NUMBER_ELEMENTS * sizeof( double ) / sizeof( int ) );

	// Copy once outside of loop
	fastMemcpy( arrDst, arrSrc, NUMBER_ELEMENTS * sizeof( double ) );

	// Do the bulk of the test 
	start = std::chrono::high_resolution_clock::now();
	for ( int i = 0; i < loopCount; i++ )
		fastMemcpy( arrDst, arrSrc, NUMBER_ELEMENTS * sizeof( double ) );
	elapsed = std::chrono::high_resolution_clock::now() - start;

	// Print result 
	nSec = 1e-6 * std::chrono::duration_cast< std::chrono::microseconds >(elapsed).count();
	printf( "SO Serge Rogatch's Answer : %s bytes/sec\n", Foo::FormatWithCommas( NUMBER_ELEMENTS * sizeof( double ) / nSec * loopCount ).c_str() );

	delete[] arrUnalignedSrc;
	delete[] arrUnalignedDst;
}
