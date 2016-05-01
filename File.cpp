#include "File_Selection.h"
#include "Command.h"
#include "SystemControl.h"

map<string, File*> File::alias_map;

using namespace std;

/*return name of file*/
string File::getName() {
	return file_getname(abspath);
}

/*is abspath directory path*/
bool File::isDirect() {
	return isDir(abspath);
}

bool File::deleteAlias(string& idx) {
	if (alias_map[idx] != nullptr) {
		alias_map.erase(idx);
		return true;
	}
	else return false;
}

/*Update current and parent directory after executing cd*/
void File::changeCurrPar() {
	string curr_dir = agetPath();
	_chdir("..");
	string par_dir = agetPath();
	_chdir(curr_dir.c_str());

	alias_map.erase("Curr");
	alias_map.erase("Par");

	alias_map["Curr"] = new File(curr_dir);
	alias_map["Par"] = new File(par_dir);
}
