# Super Seal v3 Architecture
## Registers
* SuperSeal v3 Registers store an 8-bit number
* Every register is connected to the `r` register, this means that only a source or a destination is mentioned, the rest is implied.
* List of SuperSeal v3 registers:
  1. The `r`egister (Central Register)
  2. The `a`ddress register (Ram/Port Address)
  3. The `i`nstruction register (Current Execution Address in Ram)
  4. The `x` register (Used for storage and math)
  5. The `y` register (Used for storage and math)
  6. The `m`emory register (Not really a register, just the location in Ram pointed to by the `a`ddress register)
  7. The `p`ort register (Not really a register, just an I/O port pointed to by the `a`ddress register)

## Flags
* Flags are boolean values that can be used to run code conditionally
* Flags are set as a result of certain instructions
* List of SuperSeal v3 Flags
  1. The `z`ero flag (Set when a result from the ALU is zero)
  2. The `o`verflow flag (Set when the ALU overflows)
  3. The `e`rror flag (Entirely controlled by the user)

## Conditionals
* Every instruction is conditional in SuperSeal v3
* The first 2 bits in an instruction define the condition that needs to be met to run the instruction
* List of SuperSeal Conditions:
  1. The `a`lways condition (Always runs the instruction)
  2. The `z`ero condition (Runs if the zero flag is set)
  3. The `o`verflow condition (Runs if the overflow flag is set)
  3. The `e`rror conditon (Runs if the error flag is set)

## Thats it, Fits on one page! (Shows the simplicity of it)
