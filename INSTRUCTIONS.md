# Super Seal v3 Instructions
## Structure of Super Seal v3 Instructions
Each 8-bit instruction is made up of a 2-bit condition and a 6-bit instruction  
## Conditions:
In ss3asm every instruction is prefixed with the condition letter
|Condition|Test                   |Binary|
|---------|-----------------------|------|
|a        |Always True            |0b00  |
|z        |If Zero Flag is Set    |0b01  |
|o        |If Overflow Flag is Set|0b10  |
|e        |If Error Flag is Set   |0b11  |

## `0b00` prefixed instructions
Data Moving and Flag Mainpulation Instructions:
|Instruction|Action               |Binary|
|-----------|---------------------|------|
|ra         |r = a                |0b0000|
|sa         |a = r                |0b0001|
|ri         |r = i                |0b0010|
|si         |i = r                |0b0011|
|rx         |r = x                |0b0100|
|sx         |x = r                |0b0101|
|ry         |r = y                |0b0110|
|sy         |y = r                |0b0111|
|rm         |r = m                |0b1000|
|sm         |m = r                |0b1001|
|rp         |r = p                |0b1010|
|sp         |p = r                |0b1011|
|cf         |Clear Flags          |0b1100|
|fe         |Set Error Flag       |0b1101|
|ce         |Clear Error Flag     |0b1110|
|cx         |Clear Non-Error Flags|0b1111|

## `0b01` prefixed instructions
ALU Math and Logic Instructions:
* Set the Zero Flag if the result is zero
* Set the Overflow flag based on rules in the "Overflow" collumn. Overflow Flag Rule Types:
  1. Binary: Result is too big to represent in 8-bits
  2. Max: Result is equal to `0xff`, every bit turned on
  3. Shift: The bit that was shifted out of the number

|Instruction|Action                    |Overflow|Binary|
|-----------|--------------------------|--------|------|
|ad         |r = x + y                 |Binary  |0b0000|
|ac         |r = 1 + x + y             |Binary  |0b0001|
|sc         |r = x - y - 1 (r = x + ~y)|Binary  |0b0010|
|sb         |r = x - y (r = 1 + x + ~y)|Binary  |0b0011|
|nt         |r = ~x                    |Max     |0b0100|
|ts         |r = x                     |Max     |0b0101|
|an         |r = x and y               |Max     |0b1000|
|na         |r = x nand y              |Max     |0b1000|
|or         |r = x or y                |Max     |0b1000|
|no         |r = x nor y               |Max     |0b1000|
|xo         |r = x xor y               |Max     |0b1000|
|nx         |r = x nxor y              |Max     |0b1000|
|sl         |r = 0 -> x                |Shift   |0b1000|
|il         |r = 1 -> x                |Shift   |0b1000|
|sr         |r = x <- 0                |Shift   |0b1000|
|ir         |r = x <- 1                |Shift   |0b1000|

# `0b10` prefixed `vl` instruction
Sets the lower 4 bits of r
|Instruction|Binary|
|-----------|------|
|vl         |0b????|


# `0b11` prefixed `vh` instruction
Sets the higher 4 bits of r
|Instruction|Binary|
|-----------|------|
|vh         |0b????|
