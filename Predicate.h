#ifndef PREDICATE_H_
#define PREDICATE_H_

#include <string>
#include "TimeStamp.h"
using namespace std;
class Selection;

class Predicate {
	static int ordinalNum; /*pri pravljenju selekcija obezbedjujem da svaka ima razlicito ime*/
	int id = ++ordinalNum;
public:
	Selection* condition_wrapper();
	virtual bool check_condition(string& file_path) = 0;
	virtual ~Predicate() {}
};


class FileNamePredicate : public Predicate {
	string FileName;
	bool check_condition(string&) override;
	FileNamePredicate(string& fileName);
public:
	static FileNamePredicate* allocator(string&, bool Compareflag = false);
};


class FileSizePredicate : public Predicate {
	long FileSizeFrom, FileSizeTo;
	long FileSizeEquals;
	bool range = false;
	bool check_condition(string&) override;
	FileSizePredicate(long fileSizeEquals);
	FileSizePredicate(long fileSizeFrom, long fileSizeTo);
public:
	static FileSizePredicate* allocator(string&, bool Compareflag = false);
};


class FileTimeStampPredicate : public Predicate {
	TimeStamp* TimeStampFrom, *TimeStampTo;
	TimeStamp* TimeStampEquals;
	bool range = false;
	
	bool check_condition(string&) override;
	FileTimeStampPredicate(TimeStamp* timeStampFrom, TimeStamp* timeStampTo);
	FileTimeStampPredicate(TimeStamp* timeStampEquals);
public:
	static FileTimeStampPredicate* allocator(string&, bool Compareflag = false);
	~FileTimeStampPredicate() {
		delete TimeStampFrom;
		delete TimeStampTo;
		delete TimeStampEquals;
	}
};



#endif