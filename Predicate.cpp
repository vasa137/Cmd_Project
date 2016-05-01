#include "Predicate.h"
#include "Globals.h"
#include "File_Selection.h"
#include "PredicateBuilder.h"
int Predicate::ordinalNum;

Selection* Predicate::condition_wrapper() {
	string curr_dir = agetPath();
	DIR* dir = opendir(curr_dir.c_str());
	dirent * entry;
	Selection* result = new Selection("PredicateSelection#"+ id); // generate unique name of selection as a result of composite expression
	if (dir != nullptr) {
		while ((entry = readdir(dir)) != nullptr) {
			if (string(entry->d_name) == "." || string(entry->d_name) == "..") continue;

			/*if entry is file it is okay to do this, DT_REG = file */
			if (entry->d_type == DT_REG) {
				if (check_condition(curr_dir + "\\" + string(entry->d_name))) {
					result->addFile(new File(curr_dir + "\\" + entry->d_name)); // add file to result selection because check_condition returned true
				}

			}
		}
		closedir(dir);
		PredicateBuilder::getInstance()->recRemove(); /*delete all allocated memory for temporary predicates*/
	}

	/*if selection is empty, delete it*/
	if (result->getFileList().size() == 0) {
		delete result;
		result = nullptr;
	}
	return result;
}