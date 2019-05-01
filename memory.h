#include <string>
using namespace std;


#ifndef MEMORY_H
#define MEMORY_H

class memory
{
private:
	int address;
	string data;
	string type;
	string name;


public:
	void setAddress(int a) { address = a; }
	void clearData() { data = ""; }
	int getAddress() { return address; }
	void setHexData(string d);
	void setData(string d) { data = d; }
	string getData() { return data; }
	void setType(string t) { type = t; }
	string getType() { return type; }
	void setName(string n) { name = n; }
	string getName() { return name ; }
};
#endif
