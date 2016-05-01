#include "Command.h"
#include "File_Selection.h"
#include "CommandParser.h"
#include "Exceptions.h"
#include "PredicateBuilder.h"
#include "Predicate.h"
using namespace std;

void Select::selectLoop(Selection *s) {
	bool flag = false;
	cout << endl << "Select files\\dirs (each file\\directory in one row, enter for exit ) : " << endl << endl;
	string entry_list = "", file_path = "";


	while (1) {
		bool selection = false;
		getline(cin, file_path);

		if (file_path == "") {
			if (!flag) {
				delete s;
				throw new EXCEPTION("Error : Empty selection");
			}
			else break;
		}

		if (CommandParser::getInstance()->isAlias(&file_path)) file_path = CommandParser::getInstance()->getAlias(&file_path);
		if (CommandParser::getInstance()->isSelection(&file_path) || CommandParser::getInstance()->isStar(&file_path)) { selection = true; }
		if (CommandParser::getInstance()->isPredicate(file_path)) { 
			selection = true; 
		}

		Selection *inner_selection = nullptr;

		if (selection) {
			if (CommandParser::getInstance()->isPredicate(file_path)) { 
				file_path = file_path.substr(1, file_path.length() - 1); /*delete @ from beggining of complex predicate*/
				Predicate* predicate = PredicateBuilder::getInstance()->build(file_path);

				/*if we don't recognize an error, make complex selection with predicates*/
				if (predicate != nullptr) {
					inner_selection = predicate->condition_wrapper();
				}
			}
			else if (CommandParser::getInstance()->isStar(&file_path)) {
				inner_selection = Selection::selectionFromStar(file_path);
			}
			else {
				list<Selection*>::iterator it = Selection::getSelList().begin();
				for (; it != Selection::getSelList().end(); ++it)
					if ((*it)->getSelName() == CommandParser::getInstance()->getSelectionName(&file_path)) inner_selection = *it;
			}

			if (inner_selection != nullptr) {
				s->concatenate_with(inner_selection); /*select in select allowed*/
				flag = true;
			}
		}
		else {
			string old_dir = agetPath();
			if (!isAbsolute(file_path)) file_path = old_dir + "\\" + file_path;
			if (_chdir(file_path.c_str()) < 0) {
				if (errno == ENOENT) {
					errno = 0;
					_chdir(old_dir.c_str());
					continue;
				}
			}

			if (s->addFile(new File(file_path))) flag = true; ;
			_chdir(old_dir.c_str());
		}
	}
}

bool Select::execute() {
	if (CommandParser::getInstance()->lineGet() == "") {
		throw new EXCEPTION("This isn't valid name for selection.");
	}

	if (CommandParser::getInstance()->lineGet() == "deleteall") {
		Selection::removeSelectionLists();
		cout << "All selections succesfully removed." << endl;
		return true;
	}
	if (CommandParser::getInstance()->lineGet().length() >= 8) {
		if (CommandParser::getInstance()->lineGet().substr(0, 7) == "delete ") {
			string toDelete = CommandParser::getInstance()->lineGet().substr(7, CommandParser::getInstance()->lineGet().length() - 7);
			if (Selection::removeSelection(toDelete)) {
				cout << "Selection successfully removed." << endl; return true;
			}
			else {
				throw new EXCEPTION("This selection doesn't exist.");
			}
		}
	}


	Selection::removeSelection(CommandParser::getInstance()->lineGet()); //if selection with same name exists delete it and make nwew

	Selection* s = new Selection(CommandParser::getInstance()->lineGet());

	selectLoop(s);
	
	Selection::addSelection(s); // add selection to list
	cout << "Selection succesfully created" << endl;
	return true;
}
