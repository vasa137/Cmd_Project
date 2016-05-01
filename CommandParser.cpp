#include "CommandParser.h"
#include "File_Selection.h"
#include "Command.h"
#include "SystemControl.h"
#include "Exceptions.h"
#include "PredicateBuilder.h"

using namespace std;

map<string, string> Command::command_map; // map for complex commands->scripts(multiple commands)
map<string, Command*> Command::basic_commands; // map for commands that can be executed by normal user
map<string, Command*> Command::admin_commands; // map for commands that can be executed in administrator regime
map<string, Command*>* Command::privileges_map = nullptr; // pointer to command map of current user, if we are in admin mode it will point to admin_commands,otherwise to basic_commands
CommandParser * CommandParser::instance = nullptr; // singleton

//get first word(command) and save line without it and spaces
string CommandParser::parse_and_removespace(string& line) {
	stringstream iss(line);
	string cmd;
	iss >> cmd;

	char x = ' ';
	while (x == ' ' && iss >> x);
	if (x != ' ') {
		getline(iss, CommandParser::line);
		CommandParser::line = x + CommandParser::line;
	}
	else CommandParser::line = "";
	return cmd;
}

//initialize source and dest with two strings between sep
void CommandParser::get_source_and_dest(string& source, string& dest, char sep) {
	stringstream iss(line);
	getline(iss, source, sep);
	int i = source.length() - 1;
	while (i >= 0) {
		char x = source[i];
		if (x == ' ')
			source.erase(i--);
		else break;
	}

	getline(iss, dest);
	i = 0;
	while (i <= (int)dest.length() - 1) { // cast jer ovo vraca unsigned
		char x = dest[i];
		if (x == ' ')
			dest = dest.substr(i + 1, dest.length() - i - 1);
		else break;
	}
	i = dest.length() - 1;
	while (i >= 0) {
		char x = dest[i];
		if (x == ' ')
			dest.erase(i--);
		else break;
	}
}

bool CommandParser::isAlias(string* arg) {
	string str;
	if (arg != nullptr) str = *arg;
	else str = line;
	if (str.length() > 0 && str[0] == '?') return true;
	else return false;
}
string CommandParser::getAlias(string * arg) {
	string str;
	if (arg != nullptr) str = *arg;
	else str = line;
	str = str.substr(1, str.length() - 1);
	if (File::getAliasMap()[str] != nullptr) return File::getAliasMap()[str]->getFullPath();
	else return "";
}
bool CommandParser::isSelection(string * arg) {
	string str;
	if (arg != nullptr) str = *arg;
	else str = line;
	if (str.length() > 0 && str[0] == '/') return true;
	else return false;
}
string CommandParser::getSelectionName(string* arg) {
	string str;
	if (arg != nullptr) str = *arg;
	else str = line;
	return str.substr(1, str.length() - 1);
}

/*if selection name contains star*/
bool CommandParser::isStar(string * arg) {
	string str;
	if (arg != nullptr) str = *arg;
	else str = line;
	return str.find('*') != string::npos;
}
bool CommandParser::isPredicate(string& s) {
	if (s.length() > 0 && s[0] == '@') return true;
	else return false;
}

/*get Command from stdin and command maps*/
Command* CommandParser::getCommand(string& line) {
	whole_line = line;
	string cmd = parse_and_removespace(line);
	return (*Command::privileges_map)[cmd];
}
bool CommandParser::checkTimeFormat(string& time) {
	if (time.length() != 12) return false;
	for (int i = 0; i < 12; i++) {
		if (i > time.length()) return false;
		if (i == 2 || i == 5 || i == 8) {
			if (time[i] != ':') return false;
		}
		else {
			if (time[i] < '0' || time[i] > '9') return false;
		}
	}
	return true;
}
bool CommandParser::checkDateFormat(string& date) {
	if (date.length() != 10) return false;
	for (int i = 0; i < 10; i++) {
		if (i > date.length()) return false;
		if (i == 2 || i == 5) {
			if (date[i] != '.') return false;
		}
		else {
			if (date[i] < '0' || date[i] > '9') return false;
		}
	}
	return true;
}

/*check format of selection that are compared and find both*/
void CommandParser::findCompareSelections(Selection*& sel1, Selection*& sel2, bool& star1, bool& star2, string& selname1, string& selname2) {
	bool find1 = false, find2 = false;
	CommandParser::getInstance()->setLine(selname1);
	if (CommandParser::getInstance()->isStar()) {
		sel1 = Selection::selectionFromStar(CommandParser::getInstance()->lineGet());
		star1 = true;
		find1 = true;
	}
	CommandParser::getInstance()->setLine(selname2);
	if (CommandParser::getInstance()->isStar()) {
		sel2 = Selection::selectionFromStar(CommandParser::getInstance()->lineGet());
		star2 = true;
		find2 = true;
	}

	if (!star1 || !star2) {
		list<Selection*>::iterator it = Selection::getSelList().begin();
		for (; it != Selection::getSelList().end(); ++it) {
			if (!star1) if ((*it)->getSelName() == selname1) {
				sel1 = *it;
				find1 = true;
			}
			if (!star2) if ((*it)->getSelName() == selname2) {
				sel2 = *it;
				find2 = true;
			}
			if (find1 && find2) break;
		}
	}

}
/*get arguments for compare commands*/
void CommandParser::checkCompareFormatAndGetArgs(string& selname1, string& selname2, string& condition, string& resultName, bool& intersection) {
	string sels = "", condition_space_name = "";
	CommandParser::getInstance()->get_source_and_dest(sels, condition_space_name, ' ');
	if (sels == "" || condition_space_name == "") throw new EXCEPTION("Bad command format, example: compare sel1^sel2 condition nameOfResult");
	CommandParser::getInstance()->setLine(sels);
	CommandParser::getInstance()->get_source_and_dest(selname1, selname2, '^');
	if (selname2 == "") {
		CommandParser::getInstance()->get_source_and_dest(selname1, selname2, '\\');
		intersection = false;
	}
	if (selname2 == "") throw new EXCEPTION("Bad compare operator. Type ^ for intersection, \\ for distinction.");

	CommandParser::getInstance()->setLine(condition_space_name);
	CommandParser::getInstance()->get_source_and_dest(condition, resultName, ' ');

	if ( (*(PredicateBuilder::getInstance()->get_basic_map()) ) [condition] == nullptr) throw new EXCEPTION("Condition name is not valid. Type name, size or time");
	if (resultName == "") throw new EXCEPTION("Name of result selection can't be empty string.");

}
