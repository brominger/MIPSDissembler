#include <string>
#include "memory.h"
using namespace std;


void memory::setHexData(string d) {
	if (d == "00000000000000000000000000000000")
		data = "00000000";
	else {
		int i = stoi(d, nullptr, 2);
		int r;
		string hexdec_num = "";
		char hex[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
		while (i > 0)
		{
			r = i % 16;
			hexdec_num = hex[r] + hexdec_num;
			i = i / 16;
		}
		data = hexdec_num;
	}
}



