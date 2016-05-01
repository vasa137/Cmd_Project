#ifndef LOGICALOP_H_
#define LOGICALOP_H_
#include "Predicate.h"

class LogicalOp : public Predicate {
protected:
	bool check_condition(string& file_path) override;
	Predicate* first, *second;
public:
	LogicalOp(Predicate* fir, Predicate * sec = nullptr ) : first(fir), second(sec) {}
	virtual bool operation(string& file_path) = 0;
	~LogicalOp() { delete first; delete second; }
};

class AND : public LogicalOp {
	bool operation(string& file_path) override;
public:
	AND(Predicate* first, Predicate * second) : LogicalOp(first, second) {}
	static AND* allocator(Predicate*, Predicate*);
};

class OR : public LogicalOp {
	bool operation(string& file_path) override;
public:
	OR(Predicate* first, Predicate * second) : LogicalOp(first, second) {}
	static OR* allocator(Predicate*, Predicate*);
};

class XOR : public LogicalOp {
	bool operation(string& file_path) override;
public:
	XOR(Predicate* first, Predicate * second) : LogicalOp(first, second) {}
	static XOR* allocator(Predicate*, Predicate*);
};

class NOT : public LogicalOp {
	bool operation(string& file_path) override;
public:
	NOT(Predicate* first) : LogicalOp(first) {}
	static NOT* allocator(Predicate*, Predicate* = nullptr);
};



#endif
