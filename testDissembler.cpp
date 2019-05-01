#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <iterator>
#include <vector>


using namespace std;

#include "Dissembler.h"
#include "memory.h"

vector<string> readFile(string fileName);
vector<memory> createMem(string fileName);

int main(int argc, char* argv[]) {
	vector<string> machineCode;
	vector<memory> mem;
	if (argc == 1) {
		machineCode = readFile("test.asm");
		mem = createMem("memory.csv");
	} 
	if (argc == 3) {
		machineCode = readFile(argv[1]);
		mem = createMem(argv[2]);
	}
	Dissembler dissemble;
	if (mem.size() > 0)
		dissemble.setMemory(mem);
	dissemble.dissemble(machineCode);
	dissemble.printMIPS();
	   
	return 0;
}


vector<string> readFile(string fileName) {
	vector<string> c;
	ifstream code(fileName);
	string line;

	if (code.is_open())
	{
		while (getline(code, line))
		{
			c.push_back(line);
		}
		code.close();
	}
	else cout << "Unable to open " << fileName << endl;
	return c;
}

vector<memory> createMem(string filename) {
	vector<memory> memory1;
	ifstream mem(filename);
	string address, data, type;
	memory mtemp;
	int a;
	while (mem.good()) {
		getline(mem, address, ',');
		getline(mem, data, ',');
		getline(mem, type, '\n');
		int n = (int)stoi(address,NULL,0);
		mtemp.setAddress(n);
		if ((type == "asciiz")|| (type == "asciiz"))
			mtemp.setHexData(data);
		else {
			int d = (int)stoi(data, NULL, 2);
			mtemp.setData(to_string(d));
		}
		mtemp.setType(type);
		memory1.push_back(mtemp);
	}
	return memory1;

}

