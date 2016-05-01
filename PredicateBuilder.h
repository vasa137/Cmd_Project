#ifndef PREDBUILD_H_
#define PREDBUILD_H_
#include "LogicalOp.h"
#include <string>
#include <map>
#include <stack>
using namespace std;

class Predicate;

class PredicateBuilder {
public:
	static PredicateBuilder* getInstance() {
		if (instance == nullptr) {
			instance = new PredicateBuilder();
		}
		return instance;
	}
	void allocateStack() { predicate_stack = new stack<Predicate*>(); }
	Predicate* build(string predicateLine);
	bool isRange(string&);
	void getConditions(string&, string&, string&);
	void recRemove() {
		delete root;
		root = nullptr;
	} /*removes all allocated memory for Predicates*/
	map <string, Predicate* (*) (string&)>* get_basic_map() { return &basic_predicate_map; }
private:
	map <string, Predicate* (*) (Predicate*, Predicate*)> complex_predicate_map;
	map <string, Predicate* (*) (string&)> basic_predicate_map;
	stack <Predicate*>* predicate_stack;
	PredicateBuilder() { predicate_stack = nullptr; root = nullptr; }
	string predicateLine;
	static PredicateBuilder* instance;
	void buildOperator(string);
	void buildOperand(string);
	string getConditionName(string&);
	Predicate* root;
	void strRotate(string& saver);
	friend class SystemControl;
	friend class NOT;
};

#endif
