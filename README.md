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
