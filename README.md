## Downloading the latest server public key
1. Ensure Guild Wars is running.
2. Use the following command:
```
> python tools\dump_key.py
```

## Compiling the project

### With MSVC
```
cmake -B build -G "Visual Studio 14 2015" -A Win32
cmake --build build
```

### On Debian based system, with ninja
Ensure ninja and cmake are installed.
```
sudo apt install ninja-build cmake
```

Generate the project and compile it.
```
cmake -B build -G "Ninja"
cmake --build build
```