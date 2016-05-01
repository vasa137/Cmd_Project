#include "Predicate.h"
#include "Globals.h"
#include "CommandParser.h"
#include "File_Selection.h"
#include "Exceptions.h"
#include "Globals.h"
#include "PredicateBuilder.h"
#include "TimeStamp.h"
using namespace std;

/*FileSizePredicate*/

FileSizePredicate::FileSizePredicate(long fileSizeEquals) : FileSizeEquals(fileSizeEquals) {} // first solution, with =

//second solution with range > <
FileSizePredicate::FileSizePredicate(long fileSizeFrom, long fileSizeTo) : FileSizeFrom(fileSizeFrom), FileSizeTo(fileSizeTo), range(true) {}

/*compare filesize with size in condition*/
bool FileSizePredicate::check_condition(string& file_path)   {
	if (range) {
		if (filesize(file_path) < FileSizeTo && filesize(file_path) > FileSizeFrom) return true;
	}
	else {
		if (filesize(file_path) == FileSizeEquals) return true;
	}
	return false;
}

/*static allocator that is referenced by map pointers if we enter @size in complex predicate*/
// CompareFlag indicates that we called this form compare execution and in conditions we have full path of file and we must transform it in its size
FileSizePredicate* FileSizePredicate::allocator(string& conditions, bool Compareflag) {
	if (Compareflag) {
		return new FileSizePredicate(filesize(conditions));
	}
	else {
		if (PredicateBuilder::getInstance()->isRange(conditions)) {
			string first_cond = "", second_cond = "";
			PredicateBuilder::getInstance()->getConditions(conditions, first_cond, second_cond);
			return new FileSizePredicate(atoi(first_cond.c_str()), atoi(second_cond.c_str()));
		}
		return new FileSizePredicate(atoi(conditions.c_str()));
	}
}

/*FileNamePredicate*/

FileNamePredicate::FileNamePredicate(string& fileName)  : FileName(fileName)  {}
/*compare file name with name in condition*/
bool FileNamePredicate::check_condition(string& file_path) {
	if (CommandParser::getInstance()->isStar(&FileName)) return Selection::star(file_getname(file_path), FileName);
	return (FileName == file_getname(file_path));
}
/*static allocator that is referenced by map pointers if we enter @name in complex predicate*/
// CompareFlag indicates that we called this form compare execution and in conditions we have full path of file and we must transform it in its name
FileNamePredicate* FileNamePredicate::allocator(string& conditions, bool Compareflag) {
	if (Compareflag) {
		return new FileNamePredicate(file_getname(conditions));
	}
	else return new FileNamePredicate(conditions);
}

/*FileTimeStampPredicate*/

/*compare file timestamp of last modification with timestamp in condition*/
bool FileTimeStampPredicate::check_condition(string& file_path) {
	TimeStamp* fileTimeStamp = TimeStamp::getTimeStamp(file_path);
	bool returnValue;
	if (range) {
		if (*fileTimeStamp < *TimeStampTo && *fileTimeStamp > *TimeStampFrom) returnValue = true;
		else returnValue = false;
	}
	else {
		if (*fileTimeStamp == *TimeStampEquals) returnValue = true;
		else returnValue = false;
	}
	delete fileTimeStamp;
	return returnValue;
}

FileTimeStampPredicate::FileTimeStampPredicate(TimeStamp* timeStampFrom, TimeStamp* timeStampTo) : TimeStampFrom(timeStampFrom), TimeStampTo(timeStampTo), range(true),TimeStampEquals(nullptr) {}

FileTimeStampPredicate::FileTimeStampPredicate(TimeStamp* timeStampEquals) : TimeStampEquals(timeStampEquals), TimeStampFrom(nullptr), TimeStampTo(nullptr) {}

/*static allocator that is referenced by map pointers if we enter @time in complex predicate*/
// CompareFlag indicates that we called this form compare execution and in conditions we have full path of file and we must transform it in its timestamp
FileTimeStampPredicate* FileTimeStampPredicate::allocator(string& conditions, bool Compareflag) {
	if (Compareflag) {
		return new FileTimeStampPredicate(TimeStamp::getTimeStamp(conditions));
	}
	else {
		if (PredicateBuilder::getInstance()->isRange(conditions)) {
			string first_cond = "", second_cond = "";
			PredicateBuilder::getInstance()->getConditions(conditions, first_cond, second_cond);
			TimeStamp* first_ts = TimeStamp::DateTimeParse(first_cond);
			TimeStamp* second_ts = TimeStamp::DateTimeParse(second_cond);
			return new FileTimeStampPredicate(first_ts, second_ts);
		}
		TimeStamp* ts = TimeStamp::DateTimeParse(conditions);
		return new FileTimeStampPredicate(ts);
	}
}
