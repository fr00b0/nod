# Nod
Dependency free header only signal-slot library.

# Usage

### Simple usage
The following example creates a signal and then connects a lambda as a slot.
```cpp
// Create a signal which accepts slots with no arguments and void return value.
nod::signal<void()> signal;
// Connect a lambda slot that writes "Hello, World!" to stdout
signal.connect([](){
		std::cout << "Hello, World!" << std::endl;
	})
// Call the slots
signal();
```

### Connecting multiple slots
If multiple slots are connected to the same signal, all of the slots will be
called when the signal is invoked. The slots will be called in the same order
as they where connected.
```cpp
void endline() {
	std::cout << std::endl;
}

nod::signal<void(void)> signal;
signal.connect([](){
		std::count << "Hello, World!";
	});
signal.connect(endline);

// Call the slots
signal();

```

### Slot arguments
When a signal calls it's connected slots, any arguments passed to the signal
are propagated to the slots. To make this work, we do need to specify the 
signature of the signal to accept the arguments.
```cpp
void print_sum( int x, int y ) {
	std::cout << x << "+" << y << "=" << (x+y) << std::endl;
}
void print_product( int x, int y ) {
	std::cout << x << "*" << y << "=" << (x*y) << std::endl;
}


// We create a signal with two integer arguments.
nod::signal<void(int,int)> signal;
// Let's connect our slot
nod.connect( print_sum );
nod.connect( print_product );

// Call the slots
nod( 10, 15 );
nod(-5, 7);

```


### More usage
```cpp
//todo: write some more example usage
```


## Building the tests
The test project uses [premake5](https://premake.github.io/download.html) to 
generate make files or similiar.

### Visual Studio 2013
To build and run the tests, exeucte the following from the test directory:

```batchfile
REM Adjust paths to suite your environment
c:\path\to\premake\premake5.exe vs2013
"c:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\vsvars32.bat"
msbuild /m build\vs2013\nod_tests.sln
bin\vs2013\debug\nod_tests.exe
```
