#include <iostream>
#include <string>
#include <fstream>

#include "argh.h"

#include <cstdint>
uint8_t ram[256] = { 0x00 };
uint8_t r = 0x00;	// R register
uint8_t i = 0x00;	// I register
uint8_t a = 0x00;	// A register
uint8_t x = 0x00;	// X register
uint8_t y = 0x00;	// Y register
bool z = false;		// Z flag
bool o = false;		// O flag
bool e = false;		// E flag

bool halted = false;
long cycles = 0;

bool debug = false;
bool step = false;

// Print State
void printstate() {
	std::cout << "Info: VM State" << std::endl;
	std::cout << "- Registers:" << std::endl;
	std::cout << "  - r = " << (int)r << std::endl;
	std::cout << "  - a = " << (int)a << std::endl;
	std::cout << "  - i = " << (int)i << std::endl;
	std::cout << "  - x = " << (int)x << std::endl;
	std::cout << "  - y = " << (int)y << std::endl;
	std::cout << "- Flags:" << std::endl;
	std::cout << "  - z = " << ((z) ? "true" : "false") << std::endl;
	std::cout << "  - o = " << ((o) ? "true" : "false") << std::endl;
	std::cout << "  - e = " << ((e) ? "true" : "false") << std::endl;
}

// Input
uint8_t input(uint8_t port) {
	int input;
	std::cout << "Input on Port " << (int)port << ": ";
	std::cin >> input;
	return (uint8_t)input;
}
// Output
void output(uint8_t port, uint8_t value) {
	std::cout << "Output on Port " << (int)port << ": " << (int)value << std::endl;
}

void update() {
	cycles++;	// Increment clock cycle counter

	bool endinc = true;	// Increment I register after instruction

	uint8_t inst = ram[i];		// Get Instruction
	uint8_t insth = (inst >> 4) & 0x0f;	// Get Top 4 bits
	uint8_t cond = (inst >> 6) & 0b11;	// Condition is top 2 bits of top 4 bits
	insth &= 0b11;			// Opcode is bottom 2 bits of top 4 bits
	uint8_t instl = inst & 0x0f;	// Get bottom 4 bits

//Debug
	if (debug) std::cout << "Debug: Running Instruction: " << (int)cond << ":" << (int)insth << ":" << (int)instl << std::endl;
//

	if (	// Run Instruction if Condition is True
			(cond == 0b00)		||	// 0b00: Always Run
			(cond == 0b01 && z)	||	// 0b01: Run If Zero
			(cond == 0b10 && o)	||	// 0b10: Run If Overflow
			(cond == 0b11 && e)		// 0b11: Run If Error
	)
	switch (insth) {
		case 0b00: {	// 0b00: Move Data and Modify Flag Instructions
			switch (instl) {
				case 0b0000:	// ra instruction
					r = a;
					break;
				case 0b0001:	// sa instruction
					a = r;
					break;
				case 0b0010:	// ri instruction
					r = i;
					break;
				case 0b0011:	// si instruction
					i = r;
					endinc = false;
					break;
				case 0b0100:	// rx instruction
					r = x;
					break;
				case 0b0101:	// sx instruction
					x = r;
					break;
				case 0b0110:	// ry instruction
					r = y;
					break;
				case 0b0111:	// sy instruction
					y = r;
					break;
				case 0b1000:	// rm instruction
					r = ram[a];
					break;
				case 0b1001:	// sm instruction
					ram[a] = r;
					break;
				case 0b1010:	// rp instruction
					r = input(a);
					break;
				case 0b1011:	// sp instruction
					output(a,r);
					break;
				case 0b1100:	// cf instruction
					z = false;
					o = false;
					e = false;
					break;
				case 0b1101:	// fe instruction
					e = true;
					break;
				case 0b1110:	// ce instruction
					e = false;
					break;
				case 0b1111:	// cx instruction
					z = false;
					o = false;
					break;
			}
			break;
		}
		case 0b01: {	// 0b01: ALU Instructions
			// Reset ALU Flags
			z = false;
			o = false;
			switch (instl) {
				case 0b0000:	// ad instruction
					r = x + y;
					if (r == 0) z = true;
					if ( (int)x + (int)y > 0xff ) o = true;
					break;
				case 0b0001:	// ac instruction
					r = 1 + x + y;
					if (r == 0) z = true;
					if ( 1 + (int)x + (int)y > 0xff ) o = true;
					break;
				case 0b0010:	// sc instruction
					r = x + ~y;
					if (r == 0) z = true;
					if ( (int)x + (int)~y > 0xff ) o = true;
					break;
				case 0b0011:	// sb instruction
					r = 1 + x + ~y;
					if (r == 0) z = true;
					if ( 1 + (int)x + (int)~y > 0xff ) o = true;
					break;
				case 0b0100:	// nt instruction
					r = ~x;
					if (r == 0) z = true;
					if (r == 0xff) o = true;
					break;
				case 0b0101:	// ts instruction
					r = x;
					if (r == 0) z = true;
					if (r == 0xff) o = true;
					break;
				case 0b0110:	// an instruction
					r = x & y;
					if (r == 0) z = true;
					if (r == 0xff) o = true;
					break;
				case 0b0111:	// na instruction
					r = ~(x & y);
					if (r == 0) z = true;
					if (r == 0xff) o = true;
					break;
				case 0b1000:	// or instruction
					r = x | y;
					if (r == 0) z = true;
					if (r == 0xff) o = true;
					break;
				case 0b1001:	// no instruction
					r = ~(x | y);
					if (r == 0) z = true;
					if (r == 0xff) o = true;
					break;
				case 0b1010:	// xo instruction
					r = x ^ y;
					if (r == 0) z = true;
					if (r == 0xff) o = true;
					break;
				case 0b1011:	// nx instruction
					r = ~(x ^ y);
					if (r == 0) z = true;
					if (r == 0xff) o = true;
					break;
				case 0b1100:	// sl instruction
					r = x >> 1;
					if (r == 0) z = true;
					if (r == 0xff) o = true;
					break;
				case 0b1101:	// il instruction
					r = x >> 1 | 0b10000000;
					if (r == 0) z = true;
					if (x << 7) o = true;
					break;
				case 0b1110:	// sl instruction
					r = x << 1;
					if (r == 0) z = true;
					if (r == 0xff) o = true;
					break;
				case 0b1111:	// il instruction
					r = x << 1 | 0b00000001;
					if (r == 0) z = true;
					if (x >> 7) o = true;
					break;
			}
			break;
		}
		case 0b10: {	// 0b10: vl instruction
			uint8_t vlr = 0x00;
			vlr |= r & 0xf0;
			vlr |= instl & 0x0f;
			r = vlr;
			break;
		}
		case 0b11: {	// 0b11: vh instruction
			uint8_t vhr = 0x00;
			vhr |= r & 0x0f;
			vhr |= (instl << 4) & 0xf0;
			r = vhr;
			break;
		}
		default:
			break;
	}
	// Finish cycle with I incrementing if I was not set by si, halt if at end of ram
	if (endinc) {
		if ((int)i + 1 > 0xff) halted = true;
		i++;
	}
}

void exit(int code, std::string message) {
	std::cout << message << std::endl;
	std::exit(code);
}

int main(int, char* argv[]) {
	argh::parser cmdl(argv);
	if (!cmdl(1)) exit(0, "Usage: ss3vm imgfile");
	std::string imgfilename;
	cmdl(1) >> imgfilename;
	std::ifstream imgfile(imgfilename, std::ios::binary | std::ios::in);
	if (!imgfile.is_open()) exit(1, "Error: Failed to open imgfile");

	if (cmdl[{"-d","--debug"}]) debug = true;
	if (cmdl[{"-s","--step"}]) step = true;

	// Read ram image into ram
	std::cout << "Info: Loading ram image...";
	for (int addr = 0; addr < 0xff; addr++) {
		char c;
		if(!imgfile.read(&c, 1))
			break;
		ram[addr] = static_cast<uint8_t>(c);
	}
	std::cout << "Done!" << std::endl;

	// Run VM
	std::cout << "Info: Starting VM" << std::endl;
	if (debug) printstate();
	while (!halted) {
		update();
		if (debug) printstate();
		if (step) {
			std::cout << "[Press Enter To Continue]";
			std::cin.ignore();
		}
	}
	std::cout << std::endl;

	// VM reached end of ram
	std::cout << "Info: VM reached end of ram and halted" <<std::endl;
	std::cout << "Info: VM ran " << cycles << " instructions" << std::endl;

	return 0;
}
