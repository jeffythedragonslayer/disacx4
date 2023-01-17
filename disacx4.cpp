#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
using namespace std;

// general and special register space
string regs[] = {
	"",		// 0x00
	"MACH", // 0x01 - multiply accumulator
	"MACL", // 0x02 - multiply accumulator
	"MBR",	// 0x03 - memory buffer register
	"",		// 0x04
	"",		// 0x05
	"",		// 0x06
	"",		// 0x07
	"ROMB", // 0x08 - immediate ROM
	"",		// 0x09
	"",		// 0x0A
	"",		// 0x0B
	"",		// 0x0C
	"",		// 0x0D
	"",		// 0x0E
	"",		// 0x0F
	"",		// 0x10
	"",		// 0x11
	"",		// 0x12
	"MAR",	// 0x13 - memory address register
	"",		// 0x14
	"",		// 0x15
	"",		// 0x16
	"",		// 0x17
	"",		// 0x18
	"",		// 0x19
	"",		// 0x1A
	"",		// 0x1B
	"DPR",	// 0x1C - RAM address
	"",		// 0x1D
	"",		// 0x1E
	"",		// 0x1F
	"IP",	// 0x20 - instruction pointer
	"",		// 0x21
	"",		// 0x22
	"",		// 0x23
	"",		// 0x24
	"",		// 0x25
	"",		// 0x26
	"",		// 0x27
	"P",	// 0x28 - page select

	"IR0",	// 0x50 - immediate register
	"IR1",	// 0x51
	"IR2",	// 0x52
	"IR3",	// 0x53
	"IR4",	// 0x54
	"IR5",	// 0x55
	"IR6",	// 0x56
	"IR7",	// 0x57
	"IR8",	// 0x58
	"IR9",	// 0x59
	"IR10",	// 0x5A
	"IR11",	// 0x5B
	"IR12",	// 0x5C
	"IR13",	// 0x5D
	"IR14",	// 0x5E
	"IR15",	// 0x5F - immediate register

	"R0",	// 0x60 - general purpose registers start here
	"R1",	// 0x61
	"R2",	// 0x62
	"R3",	// 0x63
	"R4",	// 0x64
	"R5",	// 0x65
	"R6",	// 0x66
	"R7",	// 0x67
	"R8",	// 0x68
	"R9",	// 0x69
	"R10",	// 0x6A
	"R11",	// 0x6B
	"R12",	// 0x6C
	"R13",	// 0x6D
	"R14",	// 0x6E
	"R15",	// 0x6F - last general purpose register
	"R0",	// 0x70 - mirror of general purpose registers
	"R1",	// 0x71
	"R2",	// 0x72
	"R3",	// 0x73
	"R4",	// 0x74
	"R5",	// 0x75
	"R6",	// 0x76
	"R7",	// 0x77
	"R8",	// 0x78
	"R9",	// 0x79
	"R10",	// 0x7A
	"R11",	// 0x7B
	"R12",	// 0x7C
	"R13",	// 0x7D
	"R14",	// 0x7E
	"R15",	// 0x7F - end of general purpose registers mirror 
};

string general_purpose_register_name(uint8_t reg_id)
{
	if (reg_id >= 128) throw runtime_error("register id must be 7-bit");

	return "";
}



string decode_opcode(uint16_t opcode)
{
	uint8_t opcode_hi = (opcode & 0xFF00) >> 8;	// high byte
	uint8_t opcode_lo = opcode & 0x00FF;		// low byte

	switch( opcode_hi ){
		case 0x00: // no operation
			return "NOP";

		case 0x08: // branch immediate near

			return "BRA\t\t#" + to_string(opcode_lo); 

		case 0x09: // branch immediate far

			return "BRA\t\t#" + to_string(opcode_lo); 
		case 0x0C: // branch on equal (Z=1)
			return "BEQ";

		case 0x10:	// branch on greater or equal (T=1)
			return "BGE";

		case 0x14: // branch on minus (N=1)
			return "BMI\t\t#" + to_string(opcode_lo);

		case 0x18: // branch on overflow (V=1)
			return "BVS";

		case 0x24: 
			return "SKIPVC"; // skip if no overflow (V=0)
			return "SKIPVS"; // skip if overflow (V=1)

		case 0x25:
			return "SKIPLT"; // skip if less than (T=0)
			return "SKIPGE"; // skip if greater or equal (T=1)

		case 0x26:
			return "SKIPNE"; // skip if not equal (Z=0)
			return "SKIPEQ"; // skip if equal (Z=1)

		case 0x27: // skip if plus (N=0)
			return "SKIPPL"; 

		case 0x28: // branch subroutine
			return "BSR\timm"; 

		case 0x2C: // branch subroutine on equal (Z=1)
			return "BSREQ";

		case 0x30: // branch subroutine on greater or equal (T=1)
			return "BSRGE\timm";

		case 0x34: // branch subroutine on minus (N=1)
			return "BSRMI\timm";

		case 0x38: // branch subroutine on overflow (V=1)
			return "BSRVS"; 

		case 0x3C: // return from subroutine
			return "RTS";

		case 0x40: // read external memory address
			return "RDBUS"; 

		case 0x48: // comparsion (reverse)
			return "CMPR reg";

		case 0x4C: // comparison (reverse)
			return "CMPR imm";

		case 0x50: // comparison - register
			return "CMP\treg";

		case 0x54: // comparsion - immediate 
			return "CMP\timm";

		case 0x59: // sign extension (8 bits)
			return "EXTS\tA";

		case 0x5A: // sign extension (16 bits)
			return "EXTS\tA"; 

		case 0x68: // read data RAM
			return "RDRAM\t0, A";
		
		case 0x69: // read data RAM
			return "RDRAM\t1, A";

		case 0x6C: // read data RAM
			return "RDRAM\t0, imm";

		case 0x6D: // read data RAM
			return "RDRAM\t1, imm";

		case 0x6E: // read data RAM
			return "RDRAM\t2, imm"; 

		case 0x70: // read data ROM
			return "RDROM\tA";

		case 0x74: // read data ROM
			return "RDROM\tabs";

		case 0x7C: // data transfer
			return "MOV\tPL, imm";

		case 0x7D: // data transfer
			return "MOV\tPH, imm";

		case 0x80:	// binary addition - register
			return "ADD\tA, reg";

		case 0x84:	// binary addition - immediate
			return "ADD\tA, imm";

		case 0x88: // binary subtraction (reverse) - register
			return "SUBR\tA, reg";

		case 0x8C: // binary subtraction (reverse) - immediate
			return "SUBR\tA, imm"; 

		case 0x90: // binary subtraction - register
			return "SUB reg";

		case 0x94: // binary subtraction - immediate
			return "SUB imm"; 

		case 0x98: // signed multiplication - register
			return "MUL reg";

		case 0x9C: // signed multiplication - immediate
			return "MUL imm"; 

		case 0xA0: // inverse exclusive or - register
			return "XNOR reg";

		case 0xA4: // inverse exclusive or - immediate
			return "XNOR imm"; 

		case 0xA8: // exclusive or - reg
			return "XOR reg";

		case 0xAC: // exclusive or - immediate
			return "XOR imm"; 

		case 0xB0: // logical and with register
			return "AND reg";

		case 0xB4: // logical and with immediate
			return "AND imm";

		case 0xB8: // logical or - reg
			return "OR reg";

		case 0xBC: // logical or - immediate
			return "OR imm";

		case 0xC0: // shift logical right - reg
			return "SHLR reg";

		case 0xC4: // shift logical right - imm
			return "SHLR imm";

		case 0xC8: // shift arithmetic right - reg
			return "SHAR reg";

		case 0xCC:// shift arithmetic right - imm
			return "SHAR imm";

		case 0xD0:	// rotate right - reg
			return "ROTR reg";

		case 0xD4: // rotate right - imm
			return "ROTR imm";

		case 0xD8:	// shift logical center - reg
			return "SHLL reg";

		case 0xDC: // shift logical center - imm
			return "SHLL imm";

		case 0xE0: // data transfer - register
			return "MOV\t\treg, A";

		case 0xE1: // data transfer - immediate
			return "MOV\t\treg**, MBR";

		case 0xE8: // write data ram
			return "WRRAM\t0, A"; 

		case 0xE9: // write data ram
			return "WRRAM\t1, A";

		case 0xEA: // write data ram
			return "WRRAM\t2, A";

		case 0xEC: // write data RAM
			return "WRRAM\t0, imm";

		case 0xED: // write data RAM
			return "WRRAM\t1, imm";

		case 0xEE: // write data RAM
			return "WRRAM\t2, imm";

		case 0xF0: // data transfer
			return "SWAP\tA, Rx";

		case 0xF8: // data transfer
			return "CLEAR";

		case 0xFC: // halt operation
			return "HALT"; 

		default:
			return "unknown opcode";
	}
}

vector<uint16_t> machine_code = {0x0808};

int main()
{
	uint16_t op_num = 0;
	uint16_t addr = 0;

	cout << "disacx4 - Capcom Cx4 Disassembler\n";
	cout << '\n';

	cout << "mnemonic\toperand\t\topcode\t\taddress\n";
	cout << '\n';

	while( op_num < machine_code.size() ){

		uint16_t opcode = machine_code[op_num++];

		string mnemonic = decode_opcode(opcode);

		cout << mnemonic << "\t\t" << hex << opcode << "\t\t" << addr << '\n';

		addr += 2;
	}
}