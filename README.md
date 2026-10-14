
# HuffmanForge — Phase 1

Phase 1 is a C++17 foundation for a binary-safe Huffman compressor. It analyzes all 256 byte values, constructs a deterministic Huffman tree, and generates ordinary Huffman codes. This phase does not yet write a compressed `.huff` stream; later phases can add canonical code lengths, format serialization, compression/decompression, statistics, and a Qt 6 interface on top of the core library.

## Module map

- `include/huffman/frequency_analyzer.hpp` and `src/frequency_analyzer.cpp`: 256-entry `uint64_t` frequency table; vector and streaming binary-file analysis.
- `include/huffman/huffman_tree.hpp` and `src/huffman_tree.cpp`: deterministic priority-queue tree builder, ordinary codes, and a stable text tree view for CLI/visualization adapters.
- `src/main.cpp`: CLI demo and file analysis display.
- `tests/huffman_tests.cpp`: dependency-free tests for empty input, a one-symbol alphabet, binary values, determinism, prefix-freeness, and file reading.
- `CMakeLists.txt`: builds the reusable core library, CLI, and CTest target. Qt is intentionally not a Phase 1 dependency.

Equal-frequency nodes are ordered by their smallest symbol. The two extracted nodes become left (`0`) and right (`1`) children in that order. This makes output repeatable across runs. Empty input has no root or codes; a one-symbol input receives the one-bit code `0`.

## Windows + VS Code build

Install a C++17 compiler and CMake 3.16 or newer, then open this folder in VS Code. In a VS Code terminal (PowerShell):

```powershell
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++
cmake --build build
ctest --test-dir build --output-on-failure
```

For Visual Studio Build Tools, use a Developer PowerShell and omit `-G` and `-DCMAKE_CXX_COMPILER`:

```powershell
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Run the built-in demonstration:

```powershell
.
build\huffforge.exe --demo
```

Analyze any file (text, image, archive, or other binary data):

```powershell
.
build\huffforge.exe analyze "C:\path\to\input.bin"
```

With a Visual Studio generator, the executable is under `build\Debug\huffforge.exe` or `build\Release\huffforge.exe`.

## Direct GCC build without CMake

If GCC is installed but CMake is not, from the project root run:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src\frequency_analyzer.cpp src\huffman_tree.cpp src\main.cpp -o huffforge.exe
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src\frequency_analyzer.cpp src\huffman_tree.cpp tests\huffman_tests.cpp -o huffman_tests.exe
.\huffman_tests.exe
.\huffforge.exe --demo
```

## Current limits and next phases

This phase builds a tree and code table in memory; it does not encode payload bits or produce `.huff` files. Phase 2 can introduce a versioned file header, canonical code lengths, bit packing, and a decoder. Later phases can add compression statistics, GUI tree rendering, CLI subcommands, malformed-input validation, packaging, an academic report, and portfolio materials. The library API is separated from the CLI to keep those additions independent.
=======
# Huffman
A C++17 file compression tool using Huffman coding
>>>>>>> 06c6f4d81cb1d3023ff1f4193aeb5caf5a88cfe3
