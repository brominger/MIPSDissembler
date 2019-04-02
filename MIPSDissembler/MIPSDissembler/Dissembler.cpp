#include <string>
#include <iostream>
using namespace std;


class Dissembler {

private:
	string source;
	string rInstruction(string source);
	string iInstruction(string source);
	string jInstruction(string source);

public:
	void parser(string source);
};

void Dissembler::parser(string source) {
	
	string opcode;
	
	for (int i =0; i < 6; i++) {
		
		opcode += source[i];
	}
	
	cout << opcode << endl;

}

string Dissembler::rInstruction(string source) {
	return source;
}

string Dissembler::iInstruction(string source) {
	return source;
}

string Dissembler::jInstruction(string source) {
	return source;
}

int main() {

	string binarySource = "00000000000000000000000000000000";
	Dissembler dissemble;
	dissemble.parser(binarySource);

}