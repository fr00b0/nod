#include "../test_helpers.hpp"
#include <nod/nod.hpp>
#include <catch.hpp>
#include <numeric>

SCENARIO( "It's possible to accumulate the return values of slots" ) {
	GIVEN( "a signal with three slots that return a double" ) {
		nod::signal<double(double,double)> signal;
		signal.connect( std::multiplies<double>{} );
		signal.connect( std::plus<double>{} );
		signal.connect( std::minus<double>{} );
		WHEN( "we create a signal accumulator with the initial value 0.0 and the std::plus<double> function, and trigger the signal through the accumulator" ) {
			auto accumulator = signal.accumulate(0.0, std::plus<double>{});
			auto result = accumulator(42,12);
			THEN( "the result is the sum of all the slots return value" ) {
				REQUIRE( result == (std::multiplies<double>{}(42,12) + std::plus<double>{}(42,12) + std::minus<double>{}(42,12)) );
			}
		}
	}

}