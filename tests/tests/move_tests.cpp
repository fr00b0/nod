#include <catch.hpp>

#include <nod/nod.hpp>


TEST_CASE( "signals can be move constructed", "[move_tests]" ) {

	using output_signal = nod::signal<void(std::ostream&)>;
	output_signal signal;
	auto connection = signal.connect(
		[](std::ostream& out){
			out << "lambda";
		});

	output_signal move_constructed_signal{std::move(signal)};

	SECTION("basic test")
	{
		std::stringstream ss;
		move_constructed_signal(ss);
		REQUIRE( ss.str() == "lambda" );
	}

	SECTION("connection test")
	{
		std::stringstream ss;
		connection.disconnect();
		move_constructed_signal(ss);
		REQUIRE( ss.str() == "" );
	}
}

TEST_CASE( "signals can be move assigned", "[move_tests]" ) {

	using output_signal = nod::signal<void(std::ostream&)>;
	output_signal signal;
	auto connection = signal.connect(
		[](std::ostream& out){
			out << "lambda";
		});

	output_signal move_assigned_signal;
	// Create a connection before move assigning the signal to test that
	// the connection object is correctly invalidated
	auto connection2 = move_assigned_signal.connect(
		[](std::ostream& out){
			out << "lambda2";
		});

	move_assigned_signal = std::move(signal);

	CHECK(connection2.connected() == false);

	SECTION("basic test")
	{
		std::stringstream ss;
		move_assigned_signal(ss);
		REQUIRE( ss.str() == "lambda" );
	}

	SECTION("connection test")
	{
		std::stringstream ss;
		connection.disconnect();
		move_assigned_signal(ss);
		REQUIRE( ss.str() == "" );
	}
}
