#include "PredicateBuilder.h"
#include "Exceptions.h"
#include <sstream>

PredicateBuilder* PredicateBuilder::instance = nullptr; // singleton

/*if we entered the criteria with > <*/
bool PredicateBuilder::isRange(string& conditions) {
	return conditions.find('<') != string::npos;
}

/*get limits from < >*/
void PredicateBuilder::getConditions(string& conditions, string& first_cond, string& second_cond) {
	stringstream cstream(conditions);
	getline(cstream, first_cond, '<');
	getline(cstream, second_cond);
}

void PredicateBuilder::buildOperator(string operatorString) {
	if (complex_predicate_map[operatorString] == nullptr) throw new EXCEPTION("Bad operator"); /*if we can't find operator in map*/
	Predicate* first = predicate_stack->top();
	predicate_stack->pop();
	Predicate *second = nullptr;
	
	/*if stack is empty and operator is NOT, we can't pop predicate from stack because it will be an error, NOT exists with one operand*/
	if (predicate_stack->size() > 0) {
		second = predicate_stack->top();
		predicate_stack->pop();
	}

	predicate_stack->push( (*complex_predicate_map[operatorString]) (first,second) ); // we call allocator for predicate in map and push it on stack
}

/*@name, @size, or @time, and change operandString to arguments of this operands*/
string PredicateBuilder::getConditionName(string& operandString) {
	stringstream ss(operandString);
	stringstream name("");
	char c;
	for (int i = 0; i < operandString.length(); i++) {
		ss >> c;
		if (c == '=' || c == '>') break;
		if (c != ' ') name << c;
	}
	getline(ss, operandString); /*menjam operandString da ostane samo predikat*/
	return name.str();
}

void PredicateBuilder::buildOperand(string operandString) {
	string conditionName = getConditionName(operandString);
	if (basic_predicate_map[conditionName] == nullptr) throw new EXCEPTION("Bad condition statement"); // if we can't found operand in map
	else predicate_stack->push((*basic_predicate_map[conditionName]) (operandString)); // call allocator for simple predicate(operand) and allocate memory and push on stack
}

/*we iterate through stringstream from its end so we need to reverse this string before we send it to complex procedures*/
void PredicateBuilder::strRotate(string& saver) {
	char c;
	for (int i = 0; i < saver.length() / 2; i++) {
		c = saver[i];
		saver[i] = saver[saver.length() - i - 1];
		saver[saver.length() - i - 1] = c;
	}
}

//iterate through stringstream from its end and parse operands(simple predicates) and operators(composite logical operations)
Predicate* PredicateBuilder::build(string predicateLine) {
	predicate_stack = new stack<Predicate*>();
	stringstream saver("");
	bool operand = true;
	try {
		for (int i = predicateLine.length() - 1; i >= 0; i--) {

			if (predicateLine[i] == ')') continue;

			if (predicateLine[i] == ',' || predicateLine[i] == '(') {
				string op = saver.str(); /*rotiranje stringa*/
				strRotate(op);
				if (operand) buildOperand(op);
				else buildOperator(op);
				saver.str(""); /*resetujem stream*/
				if (predicateLine[i] == '(') operand = false;
				else operand = true;
				continue;
			}

			if (predicateLine[i] == ' ' && !operand) {
				continue;
			}
			saver << predicateLine[i];
		}

		string op = saver.str();
		strRotate(op);
		if (!operand) buildOperator(op);
		else buildOperand(op);

		root = predicate_stack->top();
		predicate_stack->pop();
	}
	catch (EXCEPTION* e) {
		root = nullptr; 
	}

	delete predicate_stack; // delete memory allocated for stack

	return root; // return pointer to first operator in composite predicate
}