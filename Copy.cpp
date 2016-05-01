#include "File_Selection.h"
#include "Command.h"
#include "CommandParser.h"
#include "Exceptions.h"
using namespace std;

/*if file with same name exists*/
bool Copy::isoverwriteFile(string path) {
	string old_dir = agetPath();
	bool returnValue = false;
	if (_chdir(path.c_str()) < 0) {
		if (errno == ENOENT) { // ENOENT = entry with this path doesn't exist.
			returnValue = false;
		}
		else returnValue = true;
	}
	_chdir(old_dir.c_str());
	return returnValue;
}

void Copy::copyfile_to_Dir(string& source, string& dest, bool new_name = false) {
	string sfile = file_getname(source);
	ifstream sourcefile;
	sourcefile.open(source.c_str(), ios::binary);
	ofstream destfile;

	bool CopyAllowed = true;

	if (!new_name) { 
		// if file with same name exists
		bool overWrite = isoverwriteFile(dest + "\\" + sfile);
		if (overWrite) {
			if (DoThisForAll) CopyAllowed = true;
			else if (!SkipAll) {
				CopyAllowed = chooseOption(dest + "\\" + sfile);
			}
		}
		if(CopyAllowed) destfile.open((dest + "\\" + sfile).c_str(), ios::binary); // copy uz rename
	}
	else { // if we want to copy and rename the file
		destfile.open(dest.c_str(), ios::binary);
	}

	if (CopyAllowed) {
		destfile << sourcefile.rdbuf();
		sourcefile.close();
		destfile.close();
	}
}

//choose one of overwrite options
bool Copy::chooseOption(string dest) {
	cout << "Entry " << dest <<" exists. Do you want to overwrite it (Y-yes, A-yes for ALL, P - skip, K- skip for ALL, B- abort copy) ? " << endl;
	cout << "Your choice : ";
	char opt;
	cin >> opt;
	switch (opt) {
	case 'Y': case 'y' : return true;
	case 'A': case 'a' : DoThisForAll = true; return true;  break;
	case 'P': case 'p' : return false;
	case 'K': case 'k' : SkipAll = true; return false; break;
	case 'B': case 'b' : throw new EXCEPTION("Copy aborted.");
	}
	return true;
}

bool Copy::isoverwriteDir( string dest) {
	return isDir(dest);
}

void Copy::copydir_to_Dir(string src, string dest) {
	DIR * dir = opendir(src.c_str());
	dirent *entry;
	string sourcefolder = file_getname(src);
	dest += "\\" + sourcefolder;
	_mkdir(dest.c_str()); // make dir with same name in another dir

	if (dir == nullptr) return;
	while ((entry = readdir(dir))) {
		if (entry->d_type == DT_REG) {
			copyfile_to_Dir(src + "\\" + string(entry->d_name), dest);
		}
		else if (string(entry->d_name) == "." || string(entry->d_name) == "..") continue; // skip hidden directories ..-parent, .-current
		else {
			copydir_to_Dir(src + "\\" + entry->d_name, dest);
		}
	}
	closedir(dir); 
	return;
}

void Copy::copyLoop(Selection* s, bool selection, string old_dir) {
	list<File*>::iterator it; 
	if (selection)  it = (s->getFileList()).begin(); 
	while (1) {

		if (selection) {
			source = (*it)->getFullPath();
		}

		if (!selection) {
			if (_chdir(source.c_str()) < 0) {
				if (errno == ENOENT) {
					errno = 0; // ENOENT = NO ENTRY
					_chdir(old_dir.c_str());
					throw new EXCEPTION("Source doesn't exist");
				}
			}


			_chdir(old_dir.c_str());
		}

		if (!isDir(dest)) {
			if (!selection) {
				if (!isDir(source)) {
					copyfile_to_Dir(source, dest, true); // file to file, because of that true
				}
			}
		}
		else if (isDir(source)) {
			copydir_to_Dir(source, dest);
		}
		else {
			copyfile_to_Dir(source, dest);
		}
		if (!selection) break;
		else { 
			++it;
			if (it == (s->getFileList()).end()) break;
		}
	}
}

bool Copy::execute() {
	DoThisForAll = false, SkipAll = false; // initialize overwrite flags

	bool selection = false; // if we want to copy whole selection we put this flag on true

	CommandParser::getInstance()->get_source_and_dest(source, dest, '>');

	if (source == "" || dest == "") return false;

	string old_dir = agetPath(); // get current path


	if (CommandParser::getInstance()->isSelection(&source) || CommandParser::getInstance()->isStar(&source)) { selection = true; move_source = source; }
	else {
		if (CommandParser::getInstance()->isAlias(&source)) source = CommandParser::getInstance()->getAlias(&source).c_str();
		if (!isAbsolute(source)) source = old_dir + "\\" + source;
	}

	move_source = source;

	if (CommandParser::getInstance()->isAlias(&dest)) dest = CommandParser::getInstance()->getAlias(&dest).c_str();
	if (!isAbsolute(dest)) dest = old_dir + "\\" + dest;

	if (_chdir(dest.c_str()) < 0) {
		if (errno == ENOENT) {
			errno = 0;
			ofstream new_o_file(dest);                 //if we want to make a new file with specefic name and copy whole content in it
			if (new_o_file.is_open()) {
				new_o_file.close();
				remove(dest.c_str());
			}
			else {
				_chdir(old_dir.c_str());
				return false;
			}
		}
	}
	_chdir(old_dir.c_str()); //restauration

	Selection * s = nullptr;

	if (selection) {

		if (CommandParser::getInstance()->isStar()) {
			s = Selection::selectionFromStar(source);
		}
		else {
			list<Selection*>::iterator it = Selection::getSelList().begin();
			for (; it != Selection::getSelList().end(); ++it)
				if ((*it)->getSelName() == CommandParser::getInstance()->getSelectionName(&source)) s = *it;
		}
	}


	if (!selection || selection && s != nullptr) { // !selection -> dir normal or with alias, selection && s!=nullptr, if selection exists

		copyLoop(s, selection, old_dir);
		return true;
	}
	else return false;

}

/*Copy + delete*/
bool Move::execute(){
	 Copy copy(true);
	 if (copy.execute()) {
		 Delete del(true);
		 CommandParser::getInstance()->setLine(copy.getMoveSource());
		 del.execute();
		 cout << "Files moved successfully." << endl;
		 return true;
	 }
	 else {
		 /*if copy isn't executed don't delete files*/
		 cout << "Unable to move files" << endl;
		 return false;
	 }
}
