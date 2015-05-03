#include "../test_helpers.hpp"
#include <nod/nod.hpp>
#include <catch.hpp>
#include <vector>

namespace
{
	// add two integers
	int add( int x, int y ) {
		return x+y;
	}

	// subtract two integers
	int subtract( int x, int y ) {
		return x-y;
	}

	// multiply two integers
	int multiply( int x, int y ) {
		return x*y;
	}
	
	// simple collector that aggregates slot return values into a container
	template <class Container>
	struct aggregate
	{
		using result_type = typename Container;
		void operator()( Container& partial_result, typename Container::value_type&& next_result ) const {
			partial_result.push_back( std::move(next_result) );
		}
	};

}

SCENARIO( "We can make a container with slot results using a collector template" ) {
	GIVEN( "A signal with integer return value and three connected slots" ) {
		nod::signal< int(int, int), aggregate<std::vector<int>> > signal;
		signal.connect( add );
		signal.connect( subtract );
		signal.connect( multiply );
		WHEN( "triggering the signal with the aggregate<std::vector> collector" ) {
			auto result = signal(12, 42);
			THEN( "we get the results of the slots in a vector as return value" ) {
				REQUIRE( typeid(result) == typeid(std::vector<int>) );
				REQUIRE( result == (std::vector<int>{add(12,42), subtract(12,42), multiply(12,42) }) );
			}
		}
	}
}