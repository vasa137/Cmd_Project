#include"Globals.h"

//get current path
string agetPath() {
	char * buff = new char[MAX_LEN];
	_getcwd(buff, MAX_LEN);
	string dir = buff;
	delete[] buff;
	return dir;
}

bool isAbsolute(string& path) {
	if (path.length() >= 1) if (path[1] == ':') return true;
	return false;
}

bool isDir(string& s) {
	DIR * dir = opendir(s.c_str());
	if (dir == nullptr) return false;
	else {
		closedir(dir);
		return true;
	}
}

string file_getname(string& s) {
	int i = s.length() - 1;
	while (i >= 0) {
		if (s[i] == '\\') break;
		i--;
	}
	if (i < 0) return s; // vec je relativno 
	else return s.substr(i + 1, s.length() - i); // iseci relativnu
}


long filesize(string &filename) {
	ifstream in(filename.c_str(), ifstream::ate | ifstream::binary);
	long size = long(in.tellg());
	in.close();
	return size;
}

