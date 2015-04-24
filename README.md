# Nod
Dependency free header only signal-slot library.

# Usage

```c_cpp
//todo: write some example usage
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
