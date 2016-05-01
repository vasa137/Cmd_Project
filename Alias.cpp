#include "Command.h"
#include "File_Selection.h"
#include "CommandParser.h"
#include "Exceptions.h"
using namespace std;

bool Alias::execute() {
	string source = "";
	string dest = "";
	CommandParser::getInstance()->get_source_and_dest(source, dest, '|');

	if (source == "deleteall" && dest == "") { // delete all existing aliases
		File::removeAliasMap(); return true;
	}

	if (dest == "delete") {
		if (File::deleteAlias(source)) { // delete specific alias
			cout << "Alias sucessfully deleted." << endl;
			return true;
		}
		else {
			throw new EXCEPTION("This alias doesn't exist.");
		}
	}

	string old_dir = agetPath(); // get current path
	if (!isAbsolute(dest)) dest = old_dir + "\\" + dest; // if path isn't absolute add it to current folder


	if (source == "" || dest == "") {
		throw new EXCEPTION("Problem with creating alias. Name must contain characters, delimiter and alias.");
	}

	if (_chdir(dest.c_str()) < 0) {
		if (errno == ENOENT) {
			errno = 0;
			_chdir(old_dir.c_str());
			throw new EXCEPTION("This file\\directory doesn't exist");
		}
	}

	File::getAliasMap()[source] = new File(dest); // add new alias to map
	_chdir(old_dir.c_str());
	cout << endl << "Alias succesfully created." << endl;
	return true;
}