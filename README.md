## Downloading the latest server public key
1. Ensure Guild Wars is running.
2. Use the following command:
```
> python tools\dump_key.py
```

## Building Headquarter with CMake

### With MSVC
```
> cmake -B build -G "Visual Studio 14 2015" -A Win32
> cmake --build build
```

### On Debian based system, with ninja
Ensure ninja and cmake are installed.
```
> sudo apt install ninja-build cmake
```

Generate the project and compile it.
```
> cmake -B build -G "Ninja" -DCMAKE_C_COMPILER=clang
> cmake --build build
```

### CMake options
You can customize how to build the project using the following CMake variable.
The syntax is simply:
```
> cmake -B build -DNAME_OF_THE_OPTIONS=VALUE
> cmake -B build -CBUILD_SHARED_LIBS=Off
```

- *BUILD_SHARED_LIBS:BOOL*: Control whether the library are built as shared libs
  (DLL on Windows) or static libraries.
- *HQ_ENABLE_TESTING:BOOL*: Control if the tests program are included in the
  solution.
