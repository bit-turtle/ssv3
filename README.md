# SuperSeal v3 Architecture
## Information/Limitations About SuperSeal v3
1. Von-Neuman Style: Ram is where instructions are run from
2. 256 bytes of ram
## Architecture
Architecture in `ARCHITECTURE.md`
## Instruction Set
Instuction Set in `INSTRUCTIONS.md`
## Building the VM and Assembler
`cmake .` and build with any build system or just compile `src/ss3vm.cpp` and `src/ss3as.cpp` with any C++ compiler
## Writing A Program
Check the `examples/` directory
## Assembling A Program
`ss3as yourprogram.as yourprogram.bin`
## Running A Program in the VM
`ss3vm yourprogram.bin`
