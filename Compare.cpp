#include "Command.h"
#include "CommandParser.h"
#include "File_Selection.h"
#include "Predicate.h"
#include "PredicateBuilder.h"
#include "Exceptions.h"

Selection* Compare::intersect(Selection* sel1, Selection *sel2, string& condition, string& resultName) {
	list<File*>::iterator first, second; // we must iterate through all files in both selections
	Selection * result = new Selection(resultName);
	for (first = sel1->getFileList().begin(); first != sel1->getFileList().end(); ++first) {
		// we must set file from first selection as predicate to compare, we send file path to predicate allocator by dereferncing a map
		Predicate *predicate = (* (*PredicateBuilder::getInstance()->get_basic_map())[condition] ) ((*first)->getFullPath());

		for (second = sel2->getFileList().begin(); second != sel2->getFileList().end(); ++second) {
			if (predicate->check_condition((*first)->getFullPath())) {
				result->addFile((*first));
				break;
			}
		}
		delete predicate;
	}
	
	return result;
}

Selection* Compare::distinct(Selection* sel1, Selection* sel2, string& condition, string& resultName) {
	list<File*>::iterator first, second;
	Selection * result = new Selection(resultName);
	for (first = sel1->getFileList().begin(); first != sel1->getFileList().end(); ++first) {
		bool found = false;
		// we must set file from first selection as predicate to compare, we send file path to predicate allocator by dereferncing a map
	    Predicate *predicate = (*(*PredicateBuilder::getInstance()->get_basic_map())[condition]) ((*first)->getFullPath());
		for (second = sel2->getFileList().begin(); second != sel2->getFileList().end(); ++second) {
			if (predicate->check_condition((*second)->getFullPath())) {
				found = true; // file condition is same in second selection too
				break;
			}
		}
		if (!found) result->addFile((*first));
		delete predicate;
	}
	return result;
}


bool Compare::execute() {
	string selname1 = "", selname2 = "" , condition = "", resultName = "";
	bool intersection = true;

	CommandParser::getInstance()->checkCompareFormatAndGetArgs(selname1, selname2, condition, resultName, intersection);

	Selection* sel1 = nullptr, *sel2 = nullptr , *result = nullptr;

	bool star1 = false, star2 = false; // star1 - if first selection is unnamed, star2- second

	CommandParser::getInstance()->findCompareSelections(sel1, sel2, star1, star2, selname1, selname2);

	if (sel1 == nullptr) {
		if(star2) delete sel2; // delete unnamed selection
		throw new EXCEPTION("Selection 1 doesn't exist.");
	}
	if (sel2 == nullptr) {
		if (star1) delete sel1; // delete unnamed selection
		throw new EXCEPTION("Selection 2 doesn't exist.");
	}

	if (intersection == true) {
		result = intersect(sel1, sel2, condition, resultName); 
	}
	else {
		result = distinct(sel1, sel2, condition, resultName);
	}

	if (star1) delete sel1;
	if (star2) delete sel2;

	if (result != nullptr && result->getFileList().size() > 0) Selection::addSelection(result); // add selection to static selections list
	else {
		delete result;
		throw new EXCEPTION("Empty selection.");
	}
	return true;
}
