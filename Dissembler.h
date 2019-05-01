#include <string>
#include <map>
#include <vector>
#include "memory.h"

#ifndef DISSEMBLER_H
#define DISSEMBLER_H



class Dissembler
{
private:
	int pc = 0x00400000;
	int memStart = 0x00c00000;
	map<int, string> code;
	map<int, string> jumpLocation;
	vector<memory> stuffInMemory;
	int noOfJumps = 0;
	int noOfVar = 0;
	
	string rInstruction(string source);
	string iInstruction(string source);
	string jInstruction(string source);
	string getRegister(string  reg);
	string splitMachineCode(string source, int beg, int end);
	string getOpcode(string  op);
	string getFunction(string f);
	void parser(string source);
	void setJump(int i);
	void convertToAscii();
	bool findMemAddress(vector<memory> m, int address);
	void updateName(vector<memory> m, int address, string n);

public:
	void setMemory(vector<memory> m) { stuffInMemory = m; }
	void dissemble(vector<string> source);
	void printMIPS();
};
#endif
