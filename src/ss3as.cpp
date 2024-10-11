#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <cstdint>

void exit(int code, std::string message, std::string extra = "") {
	std::cout << message << extra << std::endl;
	std::exit(code);
}

uint8_t parse(std::string line) {
	if (line.length() < 3) exit(1, "Invalid Instruction (Did you forget the condition?): ", line);
	uint8_t instruction = 0x00;
	char condition = line[0];
	std::string opcode = line.substr(1,2);
	std::stringstream operands(line.substr(3, line.length()-1));
	switch (condition) {
		case 'a':
			instruction |= 0b00000000;
			break;
		case 'z':
			instruction |= 0b01000000;
			break;
		case 'o':
			instruction |= 0b10000000;
			break;
		case 'e':
			instruction |= 0b11000000;
			break;
		default:
			exit(1,"Invalid Condition: ", line);
	}
	// Parse Instruction
	// 0b00 Prefix Instructions
	if	(opcode == "ra") instruction |= 0b000000;
	else if (opcode == "sa") instruction |= 0b000001;
	else if (opcode == "ri") instruction |= 0b000010;
	else if (opcode == "si") instruction |= 0b000011;
	else if (opcode == "rx") instruction |= 0b000100;
	else if (opcode == "sx") instruction |= 0b000101;
	else if (opcode == "ry") instruction |= 0b000110;
	else if (opcode == "sy") instruction |= 0b000111;
	else if (opcode == "rm") instruction |= 0b001000;
	else if (opcode == "sm") instruction |= 0b001001;
	else if (opcode == "rp") instruction |= 0b001010;
	else if (opcode == "sp") instruction |= 0b001011;
	else if (opcode == "cf") instruction |= 0b001100;
	else if (opcode == "fe") instruction |= 0b001101;
	else if (opcode == "ce") instruction |= 0b001110;
	else if (opcode == "cx") instruction |= 0b001111;
	// 0b01 Prefix Instructions
	else if (opcode == "ad") instruction |= 0b010000;
	else if (opcode == "ac") instruction |= 0b010001;
	else if (opcode == "sc") instruction |= 0b010010;
	else if (opcode == "sb") instruction |= 0b010011;
	else if (opcode == "nt") instruction |= 0b010100;
	else if (opcode == "ts") instruction |= 0b010101;
	else if (opcode == "an") instruction |= 0b010110;
	else if (opcode == "na") instruction |= 0b010111;
	else if (opcode == "or") instruction |= 0b011000;
	else if (opcode == "no") instruction |= 0b011001;
	else if (opcode == "xo") instruction |= 0b011010;
	else if (opcode == "nx") instruction |= 0b011011;
	else if (opcode == "sl") instruction |= 0b011100;
	else if (opcode == "il") instruction |= 0b011101;
	else if (opcode == "sr") instruction |= 0b011110;
	else if (opcode == "ir") instruction |= 0b011111;
	// vl and vh instructions (0b10 and 0b11 prefix)
	else if (opcode == "vl" || opcode == "vh") {
		if	(opcode == "vl") instruction |= 0b100000;
		else if (opcode == "vh") instruction |= 0b110000;
		// Get decimal value
		unsigned int value;
		if (operands.rdbuf()->in_avail() == 0) exit(2, "Expected number after vl/vh", line);
		operands >> value;
		if (value > 0xf) exit(2,"Value too big for vl/vh (Max: 0xf)",line);
		instruction |= (uint8_t)value;
	}

	return instruction;
}

int main(int argc, char* argv[]) {
	if (argc < 2) exit(1, "Usage: ss3as srcfile [destfile]");
	std::string srcfilename(argv[1]);
	std::ifstream srcfile(srcfilename);
	std::string destfilename("ssv3.bin");
	if (argc >= 3) destfilename = argv[2];
	std::ofstream destfile(destfilename, std::ios::binary | std::ios::out);

	std::string srcline;
	while(getline(srcfile, srcline)) {
		if (srcline[0] == '%') {	// '%' indicates a 8-bit decimal value to insert
			std::string valuestr = srcline.substr(1,srcline.length()-1);
			uint8_t value = (uint8_t)std::stoi(valuestr,nullptr,10);
			destfile << value;
		}
		else destfile << (char)parse(srcline);
	}

	srcfile.close();
	destfile.close();
	return 0;
}
