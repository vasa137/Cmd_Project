#include "LogicalOp.h"
#include "PredicateBuilder.h"

/*all allocators are called with pointers that are saved in composite predicate map, when we type XOR, map with execute it allocator and return new Predicate of this type*/

bool LogicalOp::check_condition(string& file_path) {
	return operation(file_path);
}

bool AND::operation(string& file_path) {
	return (first->check_condition(file_path) && second->check_condition(file_path));
}

AND* AND::allocator(Predicate* first, Predicate* second){
	return new AND(first, second);
}

bool OR::operation(string& file_path) {
	return (first->check_condition(file_path) || second->check_condition(file_path));
}

OR* OR::allocator(Predicate* first, Predicate* second) {
	return new OR(first, second);
}

bool XOR::operation(string& file_path) {
	bool first_condition = first->check_condition(file_path), second_condition = second->check_condition(file_path);
	return ( ( first_condition && (!second_condition)) ||  ((! first_condition) && second_condition ) );
}

XOR* XOR::allocator(Predicate* first, Predicate* second) {
	return new XOR(first, second);
}

bool NOT::operation(string& file_path) {
	return (!first->check_condition(file_path));
}

NOT* NOT::allocator(Predicate* first, Predicate* second) {
	if (second != nullptr) {
		PredicateBuilder::getInstance()->predicate_stack->push(second); /*if we take one more operand from stack, we must return it because one operand is enough for not*/
	}
	return new NOT(first); // only take first operand
}