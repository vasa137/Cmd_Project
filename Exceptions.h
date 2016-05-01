#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_
#include <string>
#include <iostream>

using namespace std;
class EXCEPTION {
public:
	EXCEPTION(string desc) : description(desc) {}
	friend ostream& operator<<(ostream& it,EXCEPTION& exc) {
		return it << exc.description << endl;
	}
private:
	string description;
};

#endif
