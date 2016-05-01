#include "File_Selection.h"
#include "Command.h"
#include "Globals.h"
#include "SystemControl.h"
using namespace std;

list<Selection*> Selection::selection_list;

/*add file to selection*/
bool Selection::addFile(File *f) {
	list<File*>::iterator it = this->file_list.begin();
	for (; it != this->file_list.end(); ++it) {
		if ((*it)->getFullPath() == f->getFullPath()) {
			delete f;
			return false;
		}
	}
	file_list.push_back(f);
	return true;
}

/*concatenate files and dirs from two selections*/
void Selection::concatenate_with(Selection *s) {
	list<File*>::iterator it = s->file_list.begin();
	for (; it != s->file_list.end(); ++it) {
		addFile((*it));
	}
}

void Selection::removeSelectionLists() {
	list<Selection*>::iterator it = selection_list.begin();
	for (; it != selection_list.end(); ++it) {
		(*it)->removeFileList();
	}
	selection_list.clear();
}

bool Selection::removeSelection(string& sel_name) {
	bool flag = false;
	list<Selection*>::iterator it = selection_list.begin();
	for (; it != selection_list.end(); ++it) {
		if ((*it)->getSelName() == sel_name) {
			(*it)->removeFileList();
			flag = true;
			break;
		}
	}
	if (flag) {
		selection_list.remove((*it));
		return true;
	}
	else return false;
}

/*compare star path with file_path/dir_path and return result of comparision*/
bool Selection::star(string& entry, string& rel_path_with_star) {
	stringstream pathstream(rel_path_with_star);
	string first_part = "", second_part = "";
	getline(pathstream, first_part, '*');
	getline(pathstream, second_part, '*');

	if (first_part.length() + second_part.length() > entry.length()) return false;

	string entry_first_part = entry.substr(0, first_part.length());
	string entry_second_part = entry.substr(entry.length() - second_part.length(), second_part.length());



	if (first_part == entry_first_part && second_part == entry_second_part) return true;
	else return false;
}

/*make unnamed selection*/
Selection* Selection::selectionFromStar(string& star_path) {
	Selection* starSelection = new Selection("Star selection");
	string old_dir = agetPath();
	if (!isAbsolute(star_path)) star_path = old_dir + "\\" + star_path;

	string rel_path_with_star = file_getname(star_path);
	string par_dir = star_path.substr(0, star_path.length() - rel_path_with_star.length() - 1); // -1 , zbog razdelnika direktorijuma

	DIR* dir = opendir(par_dir.c_str());
	dirent * entry;
	if (dir != nullptr) {
		while ((entry = readdir(dir)) != nullptr) {
			if (string(entry->d_name) == "." || string(entry->d_name) == "..") continue;
			if (star(string(entry->d_name), rel_path_with_star)) {
				starSelection->addFile(new File(par_dir + "\\" + entry->d_name));
			}
		}
		closedir(dir);
	}
	if (starSelection->getFileList().size() > 0) return starSelection;
	else {
		delete starSelection;
		return nullptr;
	}
}

