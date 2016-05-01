#include "Command.h"
#include "File_Selection.h"
#include "CommandParser.h"
#include "Exceptions.h"
using namespace std;

bool Cd::execute() {
		if (CommandParser::getInstance()->isAlias()) _chdir(CommandParser::getInstance()->getAlias().c_str()); // if argument is Alias, change directory to folder which it represents
		else _chdir(CommandParser::getInstance()->lineGet().c_str());
		File::changeCurrPar(); // Update embedded aliases for current and parent directory
		return true;
}

/*print directorium content on stdout, with additional informations abou timestamps and sizes*/
void Dir::writeDir(DIR * dir, string new_dir){
	dirent* entry;
	cout << endl;
	cout << new_dir << endl;
	struct stat t_stat;
	cout << endl;
	while ((entry = readdir(dir))) {
		//DT_DIR = Directory
		if (entry->d_type == DT_DIR) {
			string dirPath = new_dir + "\\" + entry->d_name;
			stat(dirPath.c_str(), &t_stat);
			struct tm * timeinfo = localtime(&t_stat.st_atime);
			cout << "<DIR>    " << setfill(' ') << setw(30) << string(entry->d_name).c_str() << setfill(' ') << setw(46) << asctime(timeinfo);
		}
		//For file print more informations
		else
		{
			stat((new_dir + "\\" + entry->d_name).c_str(), &t_stat);
			struct tm * timeinfo = localtime(&t_stat.st_atime);
			cout << "<FILE>   " << setfill(' ') << setw(30) << string(entry->d_name).c_str() << setfill(' ') << setw(20) <<
				to_string(filesize(new_dir + "\\" + entry->d_name));
			cout << " " << setfill(' ') << setw(20) << asctime(timeinfo);
		}
	}
	cout << endl;
}

void Dir::dirLoop(Selection *s, bool selection, string new_dir) {
	DIR *dir;
	list<File*>::iterator it; // iterator for moving through files in selection
	if (selection)  it = (s->getFileList()).begin(); // if we want to show all folders from selections, we need to position at first folder
	while (1) {

		if (!selection) dir = opendir(new_dir.c_str()); // opening regular directory or alias directory
		else {
			dir = opendir((*it)->getFullPath().c_str()); // opening first folder from selection
		}

		if (dir == nullptr) { //if dir is File, it will go in then branch and skip writing content
			if (!selection) throw new EXCEPTION("This directory doesn't exist."); 
		}
		else {
			writeDir(dir, new_dir);
			closedir(dir);
		}

		if (!selection) break;
		else { // move to next file in selection
			++it;
			if (it == (s->getFileList()).end()) break;
		}
	}
}

bool Dir::execute() {
	bool selection = false; // we can list selection folders or single dir

	char * buff = new char[MAX_LEN];
	_getcwd(buff, MAX_LEN);
	string old_dir = agetPath(); // get current path
	string new_dir = "";

	if (CommandParser::getInstance()->lineGet() != "" && old_dir != CommandParser::getInstance()->lineGet()) { // if we don't want to write current dir
		if (CommandParser::getInstance()->isAlias()) {
			_chdir(CommandParser::getInstance()->getAlias().c_str());
			new_dir = agetPath();
			if (!(CommandParser::getInstance()->getAlias() == old_dir) && new_dir == old_dir) return false;
		}
		else if (CommandParser::getInstance()->isSelection() || CommandParser::getInstance()->isStar()) { selection = true; }
		else {
			_chdir(CommandParser::getInstance()->lineGet().c_str());
			new_dir = agetPath();
			if (old_dir == new_dir) {
				throw new EXCEPTION("This directory doesn't exist.");
			}
		}
	}
	else new_dir = old_dir; // if we want to print current dir


	Selection* s = nullptr; // initialize selection

	if (selection) {  // if we put slash, we must search for that selection name in static selection list
		if (CommandParser::getInstance()->isStar()) {
			s = Selection::selectionFromStar(CommandParser::getInstance()->lineGet());
		}
		else {
			list<Selection*>::iterator it = Selection::getSelList().begin();
			for (; it != Selection::getSelList().end(); ++it)
				if ((*it)->getSelName() == CommandParser::getInstance()->getSelectionName()) s = *it;
		}
	}

	if (!selection || selection && s != nullptr) { // !selection -> dir regular or with alias, selection && s!=nullptr, if selection exists
	

		dirLoop(s, selection, new_dir);

	}
	_chdir(old_dir.c_str()); // restauration
	delete[] buff;
	return true;
}

bool Makedir::execute() {
	if (_mkdir(CommandParser::getInstance()->lineGet().c_str()) == 0) {
		cout << "Directory successfully created. " << endl;
		return true;
	}
	else throw new EXCEPTION("This isn't a valid path for directory");
}
