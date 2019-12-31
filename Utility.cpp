#include "Utility.h"
#include "AlitaStd.h"


std::string NP(const std::string &path){
	std::string ret(path);
	const char *p = FILE_DELIMITER;
	for (auto &ch : ret) {
		if (ch == '\\' || ch == '/') {
			ch = p[0];
		}
	}
	return ret;
}

double clip(double v, double low, double high){
	if(v < low) v = low;
	if(v > high) v = high;
	return v;
}