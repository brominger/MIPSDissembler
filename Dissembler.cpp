#include <string>
#include <bitset>
#include <iostream>
#include <map>
#include <sstream>
#include <algorithm>


using namespace std;

#include "Dissembler.h"
#include "memory.h"

void Dissembler::dissemble(vector<string> source) {

	for (int i = 0; i < source.size(); i++)
		parser(source[i]);
		


}


void Dissembler::parser(string source) {
	pc += 4;
	string  opcode = splitMachineCode(source, 0, 6);
	if (opcode == "000000")
		code.insert(pair<int,string>(pc, rInstruction(source)));
	else if (opcode == "000010" || opcode == "000011")
		code.insert(pair<int, string>(pc, jInstruction(source)));
	else
		code.insert(pair<int, string>(pc, iInstruction(source)));


}

string Dissembler::rInstruction(string source) {

	string rType;
	string  rs = splitMachineCode(source, 6, 11);
	string  rt = splitMachineCode(source, 11, 16);
	string   rd = splitMachineCode(source, 16, 21);
	string   function = splitMachineCode(source, 26, 32);
	//add $s0,$t0,$t1
	if (function == "001100")
		rType = getFunction(function);
	else
		rType = getFunction(function) +" " + getRegister(rd) + " " + getRegister(rs) + " " + getRegister(rt);
	return rType;
}

string Dissembler::jInstruction(string source) {
	string jType;
	
	string  opcode = splitMachineCode(source, 0, 6);
	string jumpMem = splitMachineCode(source, 6, 32);
	int n = stoi(jumpMem, nullptr, 2);
	bitset<32> b = bitset<32>(n);
	b = (b <<= 2);
	int i = (int)(b.to_ulong());
	setJump(i);
	jType = getOpcode(opcode) + " " + jumpLocation.find(i)->second;
	return jType;
}

string Dissembler::iInstruction(string source) {

	/**bgez	rs, label	000001	rt = 00001
	bgtz	rs, label	000111	rt = 00000
	blez	rs, label	000110	rt = 00000
	bltz	rs, label	000001	rt = 00000**/
	string iType;


	string  opcode = splitMachineCode(source, 0, 6);
	string  rs = splitMachineCode(source, 6, 11);
	string  rd = splitMachineCode(source, 11, 16);
	string immed = splitMachineCode(source, 16, 32);
	int n = stoi(immed, nullptr, 2);
	if ((opcode == "000100") || (opcode == "000001") || (opcode == "000111") || (opcode == "000110") || (opcode == "000101")) {
		int i = n + pc;
		setJump(i);
		iType = getOpcode(opcode) + " " + getRegister(rd) + " " + getRegister(rs) + " " + jumpLocation.find(i)->second;
	}
	else if ((opcode == "101011") || (opcode == "100011")) {

		iType = getOpcode(opcode) + " " + getRegister(rd) + " " + to_string(n) + "(" + getRegister(rs) + ")";
	}
	else {
		int memLoc = memStart + n;
		if (findMemAddress(stuffInMemory, memLoc)) {
			string name = "var" + to_string(noOfVar);
			iType = getOpcode(opcode) + " " + getRegister(rd) + " " + getRegister(rs) + " " + name;
			updateName(stuffInMemory, memLoc, name);

		} else {

			iType = getOpcode(opcode) + " " + getRegister(rd) + " " + getRegister(rs) + " " + to_string(n);
		}

	}
	return iType;
}


string Dissembler::getRegister(string  reg) {

	static string regArray[] = {
		"$zero",
		"$at",
		"$v0","$v1",
		"$a0","$a1","$a2","$a3",
		"$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
		"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7",
		"$t8","$t9",
		"$k0","$k1",
		"$gp",
		"$sp",
		"$fp",
		"$ra",
	};
	//a static array of the corresponding binary representation for each register
	static string binRegArray[] = {
		"00000",
		"00001",
		"00010","00011",
		"00100","00101","00110","00111",
		"01000","01001","01010","01011","01100","01101", "01110","01111",
		"10000","10001","10010","10011","10100","10101","10110","10111",
		"11000","11001",
		"11010","11011",
		"11100",
		"11101",
		"11110",
		"11111",
	};
	for (int i = 0; i < 32; i++) {
		if (reg == binRegArray[i])
			return regArray[i];
	}
	
}

string Dissembler::getFunction(string  code) {

	static string function[] = {
		"add",
		"addu",
		"and",
		"break",
		"div",
		"divu",
		"jalr",
		"jr",
		"mfhi",
		"mflo",
		"mthi",
		"mtlo",
		"mult",
		"multu",
		"nor",
		"or",
		"sll",
		"sllv",
		"slt",
		"sltu",
		"sra",
		"srav",
		"srl",
		"srlv",
		"sub",
		"subu",
		"syscall",
		"xo"
	
	};
	static string  binFunctionArray[] = {
	"100000",
	"100001",
	"100100",
	"001101",
	"011010",
	"011011",
	"001001",
	"001000",
	"010000",
	"010010",
	"010001",
	"010011",
	"011000",
	"011001",
	"100111",
	"100101",
	"000000",
	"000100",
	"101010",
	"101011",
	"000011",
	"000111",
	"000010",
	"000110",
	"100010",
	"100011",
	"001100",
	"100110"
	};

	for (int i = 0; i < 28; i++) {
		if (code == binFunctionArray[i])
			return function[i];
	}

	
}
string Dissembler::getOpcode(string  code) {


	static string opCode[] = {
	"j",
	"jal",
	"addi",
	"addiu",
	"andi",	
	"beq",
	"bgez",	
	"bgtz",	
	"blez",	
	"bltz",	
	"bne",	
	"lb",
	"lbu",
	"lh",	
	"lhu",	
	"lui",	
	"lw",	
	"lwc1",
	"ori",	
	"sb",	
	"slti",
	"sltiu",	
	"sh",	
	"sw",	
	"swc1",
	"xori"	
		
	};
	static string  binCodeArray[] = {
	"000010",
	"000011",
	"001000",
"001001",
"001100",
"000100",
"000001",
"000111",
"000111",
"000001",
"000101",
"100000",
"100100",
"100001",
"100101",
"001111",
"100011",
"110001",
"001101",
"101000",
"001010",
"001011",
"101001",
"101011",
"111001",
"001110"
	};

	for (int i = 0; i < 28; i++) {
		if (code == binCodeArray[i])
			return opCode[i];
	}


}
string Dissembler::splitMachineCode(string source, int beg, int end) {
	int i = beg;
	string code;
	while (i < end) {
		code += source[i];
		i++;
	}
	return code;
}

void Dissembler::setJump(int i) {

	string text = "Loc" + to_string(noOfJumps);
	jumpLocation.insert(pair<int, string>(i, text));
	noOfJumps++;
}

bool Dissembler::findMemAddress(vector<memory> m, int a) {
	//vector<memory> temp = m;

	for (memory mTemp : m) {
		if (mTemp.getAddress() == a)
			return true;
	}
	return false;
}




void Dissembler::printMIPS() {
	map<int, string>::iterator itr;
	int i = 0x00400000;
	cout << "main:" << endl << endl;
	for (itr = code.begin(); itr != code.end(); ++itr) {
		i += 4;
		if (jumpLocation.find(i) == jumpLocation.end()) {
			cout << "\t" << itr->second << endl;
		}
		else {
			cout << jumpLocation[i] << ":\t" << itr->second << endl;
		}
	}
	cout << endl << "data:" << endl << endl;
	convertToAscii();
	for (memory mTemp : stuffInMemory) {
		if (!mTemp.getName().empty()) {
			if ((mTemp.getType() == "asciiz") || (mTemp.getType() == "ascii")) {
				cout << mTemp.getName() << ": ." << mTemp.getType() << " \"" << mTemp.getData() << "\"" << endl;
			}
			else {
				cout << mTemp.getName() << ": ." << mTemp.getType() <<  " " << mTemp.getData()   << endl;
			}
		}
	}
}


void Dissembler::updateName(vector<memory> m, int a, string n) {
	for (int i = 0; i < stuffInMemory.size(); i++) {
		if (stuffInMemory.at(i).getAddress() == a)
			stuffInMemory.at(i).setName(n);
	}
}

void Dissembler::convertToAscii() {
	memory mem;
	vector<memory> temp;
	string hex;
	string name ="";
	string tempS;
	for (int i = 0; i < stuffInMemory.size(); i++) {
		mem = stuffInMemory.at(i);
		if (mem.getType() == "asciiz") {
			if (mem.getName() != "")
				name = mem.getName();
			for (int c = 0; c < 8; c += 2) {
				tempS += (stuffInMemory.at(i).getData()[c]); 
				tempS += (stuffInMemory.at(i).getData()[c + 1]);
				if (tempS != "00") {
					hex = hex + stuffInMemory.at(i).getData()[c] + stuffInMemory.at(i).getData()[c + 1];
					tempS = "";
				} else {
					string ascii;
					for (int i = 0; i < hex.length(); i += 2)
					{
						string byte = hex.substr(i, 2);
						char chr = (char)(int)strtol(byte.c_str(), NULL, 16);
						ascii.push_back(chr);
					}
					mem.setName(name);
					mem.setData(ascii);
					temp.push_back(mem);
					hex = "";
					tempS = "";
					name = "";
					c = 8;
					break;
				}

			}
		}
		temp.push_back(mem);

	}
	stuffInMemory = temp;
}

