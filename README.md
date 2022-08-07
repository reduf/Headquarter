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

### Running a test plugin
You can familiarize yourself with the project by compiling the client and then
running the example plugin "chat".

Assuming you created the cmake build folder at `$repo/build`, you can run the
following command:
```
> build/code/client/Debug/client.exe build\plugins\chat\Debug\chat.dll -email {email} -password {password} -character "{charname}" -mapid 449
```

### Creating your own plugin
This project unfortunately had very narrow use cases due to being private for
most of it's lifetime and especially for the part when I was still actively
working on it. Due to that, creating your own plugin might require some fiddling.

Overall, you should always create your own plugin as a new directory in `plugins/`
and add the new directory in `plugins/CMakeLists.txt`. This is somewhat a
private namespace and should be easy to make any modifications you want without
having difficulty pulling the latest changes from my repository.

Any changes in other directory, especially in `code/` are likely to create
extra works for you, if you ever choose to pull the latest changes. I would
recommend to try to upstream any of those changes to this repo.

### CMake options
You can customize how to build the project using the following CMake variable.
The syntax is simply:
```
> cmake -B build -DNAME_OF_THE_OPTIONS=VALUE
> cmake -B build -CBUILD_SHARED_LIBS=Off
```

- **BUILD_SHARED_LIBS:BOOL**: Control whether the library are built as shared libs
  (DLL on Windows) or static libraries.
- **HQ_ENABLE_TESTING:BOOL**: Control if the tests program are included in the
  solution.
