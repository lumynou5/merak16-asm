# merak16-asm

A lightweight assembler library for Merak16.

Note that this is just a toy project and cannot handle hexadecimal numbers, negative numbers, etc., yet.

## Build

Please ensure CMake and Catch2v3 installed.

```sh
mkdir build
cd build
cmake ..
cmake --build . --target merak16_asm      # Build the library.
cmake --build . --target merak16_asm_test # Build the test.
```
