#ifndef GLOBALS_H_
#define GLOBALS_H_


#include <iostream>
#include <sstream>
#include <string>
#include <dirent.h>
#include <cstring>
#include <direct.h>
#include <cstring>
#include <errno.h>
#include <fstream>
#include <map>
#include <list>
#include <stdio.h>
#include <iomanip>
#include <time.h>
#include <Windows.h>

#define WIN32_LEAN_AND_MEAN
#define MAX_LEN 256
#define NAME_LEN 64
#define SCRIPT_LEN 1024

using namespace std;
struct AliasF {
	char name[NAME_LEN];
	char abspath[MAX_LEN];
};

struct SelectionF {
	char name[NAME_LEN];
	int num_of_files;
};

struct AssignF{
	char name[NAME_LEN];
	char script[SCRIPT_LEN];
};

string file_getname(string& s);

bool isAbsolute(string& path);

bool isDir(string& s);

string agetPath();

long filesize(string &filename);

#endif